// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMultiplayerSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameUserSettings.h"


UMyMultiplayerSubsystem::UMyMultiplayerSubsystem()
	: CreateServerAfterDestroy(false)
{

}

void UMyMultiplayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SetGameScreen(1280, 720);

	Print("UMyMultiplayerSubsystem::Initialize");
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) {
		FString SystemName = Subsystem->GetSubsystemName().ToString();
		Print("~~~ SystemName: " + SystemName);
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface && SessionInterface.IsValid()) {
			Print("~~~ OnlineSession is Valid");
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyMultiplayerSubsystem::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMyMultiplayerSubsystem::OnDestroySessionComplete);
		}
		else {
			Print("~~~ OnlineSession is not Valid");
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

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

bool UMyMultiplayerSubsystem::CreateServer(const FString& ServerName)
{
	if (CreateServerAfterDestroy) {
		Print("~~~ Server is Destroying");
		return false;
	}

	if (ServerName.TrimStartAndEnd().IsEmpty()) {
		Print("~~~ ServerName is Empty");
		return false;
	}

	FName MySessionName = FName(ServerName); //FName("Multiplayer Session");

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession) {
		Print(FString::Printf(TEXT("~~~ Session With Name %s Already Exists, Will Destroy"), *MySessionName.ToString()));
		CreateServerAfterDestroy = true;
		DestroyServerName = ServerName; // 必须放在DestroySession前面
		SessionInterface->DestroySession(MySessionName);
		return false;
	}

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

void UMyMultiplayerSubsystem::SetGameScreen(int x, int y)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	UserSettings->SetScreenResolution(FIntPoint(x, y));
	UserSettings->SetFullscreenMode(EWindowMode::Windowed);
	UserSettings->ApplySettings(true);
}

void UMyMultiplayerSubsystem::OnCreateSessionComplete(FName SessionName, bool isSuccess)
{
	if (!isSuccess) {
		Print("~~~ Session Create Failed");
		return;
	}

	GetWorld()->ServerTravel("/Game/_Game/Maps/MultiplayerLevel?Listen");
}

void UMyMultiplayerSubsystem::OnDestroySessionComplete(FName SessionName, bool isSuccess)
{
	Print("~~~ OnDestroySessionComplete");
	if (CreateServerAfterDestroy) {
		CreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}
