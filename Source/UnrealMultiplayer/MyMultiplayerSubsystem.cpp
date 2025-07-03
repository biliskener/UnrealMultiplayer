// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMultiplayerSubsystem.h"


UMyMultiplayerSubsystem::UMyMultiplayerSubsystem() {

}

void UMyMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Print("UMyMultiplayerSubsystem::Initialize");
}

void UMyMultiplayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	Print("UMyMultiplayerSubsystem::Deinitialize");
}

void UMyMultiplayerSubsystem::Print(const FString& Message, const FColor& Color, float ShowTime, int32 inKey)
{
	if (IsValid(GEngine)) {
		GEngine->AddOnScreenDebugMessage(inKey, ShowTime, Color, Message);
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}


