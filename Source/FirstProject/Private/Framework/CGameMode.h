// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericTeamAgentInterface.h"
#include "CGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRol, const FString& Options) override;
	
	FGenericTeamId GetTeamIdForPlayer(const APlayerController* PlayerController);
	
	private:
	AActor* FindNextStartSpotForTeam(const FGenericTeamId& TeamId);
	
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TMap<FGenericTeamId, FName> TeamPlayerStartTagMap;
};
