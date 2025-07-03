// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMultiplayerSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


UMyMultiplayerSubsystem::UMyMultiplayerSubsystem() {

}

void UMyMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Print("UMyMultiplayerSubsystem::Initialize");
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		FString SystemName = Subsystem->GetSubsystemName().ToString();
		Print("~~~ SystemName: " + SystemName);
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface && SessionInterface.IsValid()) {
			Print("~~~ OnlineSession Is Valid");
		}
		else {
			Print("~~~ OnlineSession Is Not Valid");
		}
	}
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

bool UMyMultiplayerSubsystem::CreateServer(const FString& ServerName)
{
	if (ServerName.TrimStartAndEnd().IsEmpty()) {
		Print("~~~ ServerName is Empty");
		return false;
	}

	FName MySessionName = FName("Multiplayer Session");
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true; // 是否允许加入
	SessionSettings.bIsDedicated = false; // 是否为专用服务器
	SessionSettings.bShouldAdvertise = true; // 是否广播
	SessionSettings.NumPrivateConnections = 10; // 连接数量
	SessionSettings.bUseLobbiesVoiceChatIfAvailable = true; 
	SessionSettings.bUsesPresence = true; // 是否跨区域
	SessionSettings.bAllowJoinViaPresence = true; // 是否允许通过在线状态加入

	bool isLan = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
		isLan = true;
	}
	SessionSettings.bIsLANMatch = isLan; // 是否允许局域网

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);

	Print("~~~ Server Create Finished");
	return true;
}


