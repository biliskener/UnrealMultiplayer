// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealStartGameMode.h"

void AUnrealStartGameMode::HostLanGame()
{
	GetWorld()->ServerTravel("/Game/_Game/Maps/MainLevel?Listen");
}

void AUnrealStartGameMode::JoinLanGame(const FString& address)
{
	APlayerController* LocalPlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (IsValid(LocalPlayerController)) {
		LocalPlayerController->ClientTravel(address, ETravelType::TRAVEL_Absolute);
	}
}
