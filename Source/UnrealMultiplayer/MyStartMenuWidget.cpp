// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStartMenuWidget.h"
#include "MyMultiplayerSubsystem.h"

bool UMyStartMenuWidget::Initialize()
{
	if (!Super::Initialize()) {
		return false;
	}

	if (GetGameInstance()) {
		MultiplayerSubsystem = GetGameInstance()->GetSubsystem<UMyMultiplayerSubsystem>();
	}

	return true;
}

void UMyStartMenuWidget::OnCreateServerBtnClick()
{
	if (MultiplayerSubsystem) {
		MultiplayerSubsystem->Print("~~~ Create Server");
	}
}

void UMyStartMenuWidget::OnJoinGameBtnClick()
{
	if (MultiplayerSubsystem) {
		MultiplayerSubsystem->Print("~~~ Join Game");
	}
}

void UMyStartMenuWidget::OnQuitGameBtnClick()
{
}
