// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CCharacter.h"

#include "AbilitySystemTestAttributeSet.h"
#include "AbilitySystem/CAbilitySystemComponent.h"
#include "AbilitySystem/CAttributeSet.h"
#include "AbilitySystem/CAbilitySystemNativeTags.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

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
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_CameraBoom, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Target, ECR_Ignore);
	
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AI perception Stimuli Source Component");
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
	
	AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
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
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_STAT_AIMING).AddUObject(this, &ACCharacter::AimTagUpdated);
	AbilitySystemComponent->RegisterGameplayTagEvent(TAG_STAT_STUN).AddUObject(this, &ACCharacter::StunTagUpdated);
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

void ACCharacter::AimTagUpdated(const FGameplayTag Tag, int32 Count)
{
	SetIsAiming(Count != 0);
}

void ACCharacter::StunTagUpdated(const FGameplayTag Tag, int32 Count)
{
	if (bIsDead())
	{
		return;
	}
	
	bool bIsStun = Count != 0;
	if (bIsStun)
	{
		PlayAnimMontage(StunMontage);
	}
	else
	{
		StopAnimMontage(StunMontage);
	}
	SetInputEnabledFromPlayerController(!bIsStun);
}

void ACCharacter::SetIsAiming(bool bIsAiming)
{
	bUseControllerRotationYaw = bIsAiming;
	GetCharacterMovement()->bOrientRotationToMovement = !bIsAiming;
	OnAimStatChanged(bIsAiming);
}

void ACCharacter::OnAimStatChanged(bool bNewIsAiming)
{
	
}

// Called to bind functionality to input
void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACCharacter::SetInputEnabledFromPlayerController(bool bNewInputEnabled)
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		if (bNewInputEnabled)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);
		}
	}
}

UAbilitySystemComponent* ACCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACCharacter::StartDeathSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Death Sequence"))
	PlayDeathMontage();
	
	SetAIPerceptionStimuliSourceEnabled(false);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SetInputEnabledFromPlayerController(false);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
	}
}

void ACCharacter::Respawn()
{
	StopAnimMontage(DeathMontage);
	SetAIPerceptionStimuliSourceEnabled(true);
	SetRagdollEnabled(false);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	SetInputEnabledFromPlayerController(true);
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

void ACCharacter::SetAIPerceptionStimuliSourceEnabled(bool bIsStimuliSourceEnabled)
{
	if (!AIPerceptionStimuliSourceComponent)
		return;
	if (bIsStimuliSourceEnabled)
	{
		AIPerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();
	}
	else
	{
		AIPerceptionStimuliSourceComponent->UnregisterFromPerceptionSystem();
	}
}

