// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyMultiplayerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREALMULTIPLAYER_API UMyMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMyMultiplayerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	void Print(const FString& Message, const FColor& Color = FColor::MakeRandomColor(), float ShowTime = 2.0f, int32 inKey = -1);

	bool CreateServer(const FString& ServerName);

	UFUNCTION(BlueprintCallable, Category = "Screen")
	void SetGameScreen(int x, int y);

protected:
	IOnlineSessionPtr SessionInterface;

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool isSuccess);
};
