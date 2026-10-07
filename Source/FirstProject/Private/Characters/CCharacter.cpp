// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CCharacter.h"

#include "AbilitySystemTestAttributeSet.h"
#include "AbilitySystem/CAbilitySystemComponent.h"
#include "AbilitySystem/CAttributeSet.h"
#include "AbilitySystem/CAbilitySystemNativeTags.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "FirstProject/FirstProject.h"
#include "Widgets/OverheadStatusGauge.h"
#include "GameFramework/CharactermovementComponent.h"
#include "Player/CPlayerController.h"
#include "Net/UnrealNetwork.h"

void ACCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACCharacter, TeamId);
}


// Sets default values
ACCharacter::ACCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UCAbilitySystemComponent>("AbilitySystemComponent");
	CAttributeSet = CreateDefaultSubobject<UCAttributeSet>("CAttributeSet");

	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Overhead Widget Component");
	OverheadWidgetComponent-> SetupAttachment(GetRootComponent());
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_CameraBoom, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_CameraBoom, ECR_Ignore);
}

void ACCharacter::ServerSideInit()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->ApplyInitialEffects();
	AbilitySystemComponent->GiveInitialAbilities();
	
}

void ACCharacter::ClientSideInit()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}


// Called when the game starts or when spawned
void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverheadWidgetComponent();
	BindGASDelegates();
}

// Called every frame
void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACCharacter::BindGASDelegates()
{
	if (bGASDelegateBound || !AbilitySystemComponent)
		return;
	
	bGASDelegateBound = true;
	
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_STAT_DEAD).AddUObject(this, &ACCharacter::DeathTagUpdated);
}

void ACCharacter::DeathTagUpdated(const FGameplayTag Tag, int32 Count)
{
	if (Count != 0)
	{
		StartDeathSequence();
	}
	else
	{
		
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		Respawn();
	}
}

// Called to bind functionality to input
void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACCharacter::StartDeathSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Death Sequence"))
	PlayDeathMontage();
	
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}
}

void ACCharacter::Respawn()
{
	StopAnimMontage(DeathMontage);
	SetRagdollEnabled(false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}
	CAttributeSet->SetHealth(CAttributeSet->GetMaxHealth());
	StopAnimMontage(DeathMontage);
	if (HasAuthority() && GetController()->StartSpot.IsValid())
	{
	SetActorTransform(GetController()->StartSpot->GetActorTransform());
	}
	
}

bool ACCharacter::bIsDead() const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TAG_STAT_DEAD);
}

void ACCharacter::SetRagdollEnabled(bool bIsEnabled)
{
	if (bIsEnabled)
	{	SkeletalMeshRelativeTransform = GetMesh()->GetRelativeTransform();
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(SkeletalMeshRelativeTransform);
	}
	
}

void ACCharacter::PlayDeathMontage()
{
	if (DeathMontage)
	{
		float DeathAnimationDuration = PlayAnimMontage(DeathMontage);
		GetWorldTimerManager().SetTimer(DeathAnimationTimerHandle, this, &ACCharacter::DeathAnimationFinished, DeathAnimationDuration - DeathAnimationTimeOffset);
	}
}

void ACCharacter::DeathAnimationFinished()
{
	if (bIsDead())
	{
		SetRagdollEnabled(true);
	}
}

void ACCharacter::ConfigureOverheadWidgetComponent()
{
	if (!OverheadWidgetComponent)
	{
		return;
	}
	
	if (IsLocallyControlledByPlayer())
	{
		OverheadWidgetComponent->SetHiddenInGame(true);
		return;
	}
	
	UOverheadStatusGauge* OverheadStatusGauge = Cast<UOverheadStatusGauge>(OverheadWidgetComponent->GetUserWidgetObject());
	if (OverheadStatusGauge)
	{
		OverheadStatusGauge->ConfigureWithAbilitySystemComponent(GetAbilitySystemComponent());
	}
	OverheadWidgetComponent->SetHiddenInGame(false);
}

void ACCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamId = NewTeamID;
}

FGenericTeamId ACCharacter::GetGenericTeamId() const
{
	return TeamId;
}


bool ACCharacter::IsLocallyControlledByPlayer() const
{
	return IsLocallyControlled() && GetController()->IsPlayerController();
}

void ACCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
	}
}

