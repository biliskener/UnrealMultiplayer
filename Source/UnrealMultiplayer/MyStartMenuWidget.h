// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStartMenuWidget.generated.h"

class UMyMultiplayerSubsystem;

UCLASS()
class UNREALMULTIPLAYER_API UMyStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadonly, Category = "Subsystem")
	TObjectPtr<UMyMultiplayerSubsystem> MultiplayerSubsystem;

protected:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable)
	void OnCreateServerBtnClick();

	UFUNCTION(BlueprintCallable)
	void OnJoinGameBtnClick();

	UFUNCTION(BlueprintCallable)
	void OnQuitGameBtnClick();
};
