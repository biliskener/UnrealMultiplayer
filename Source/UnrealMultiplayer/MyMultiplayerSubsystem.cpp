// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMultiplayerSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameUserSettings.h"
#include "Online/OnlineSessionNames.h"


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
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyMultiplayerSubsystem::OnFindSessionComplete);
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
	//SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true; // 是否跨区域
	SessionSettings.bAllowJoinViaPresence = true; // 是否允许通过在线状态加入
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL"; // 是否允许局域网

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), MySessionName, SessionSettings);

	Print("~~~ Server Create Finished");
	return true;
}

bool UMyMultiplayerSubsystem::FindServer(const FString& ServerName)
{
	if (ServerName.TrimStartAndEnd().IsEmpty()) {
		Print("~~~ Server Name is Empty");
		return false;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	ServerNameToFind = ServerName;
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	bool isSuccess = SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	Print(isSuccess ? "~~~ FindServer OK" : "~~~ FindSErver Failed");
	return isSuccess;
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

void UMyMultiplayerSubsystem::OnFindSessionComplete(bool isSuccess)
{
	if (!isSuccess) {
		Print(FString::Printf(TEXT("~~~ Session Search Failed")));
		return;
	}

	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	Print(FString::Printf(TEXT("~~~ Session Search Results %d"), Results.Num()));
	for (auto& Result : Results) {
		if (Result.IsValid()) {
			FString ServerName;
			Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);
			Print(FString::Printf(TEXT("~~~ Found Server: %s"), *ServerName), FColor::Red, 5.0f);
			if (ServerName == ServerNameToFind) {
				Print("~~~ Server Matched!!!", FColor::Green, 5.0f);
				break;
			}
		}
	}
}
