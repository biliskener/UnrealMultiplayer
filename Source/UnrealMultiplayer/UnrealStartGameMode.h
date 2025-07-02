// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UnrealStartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMULTIPLAYER_API AUnrealStartGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "LanGame")
	void HostLanGame();

	UFUNCTION(BlueprintCallable, Category = "LanGame")
	void JoinLanGame(const FString& address);
};
