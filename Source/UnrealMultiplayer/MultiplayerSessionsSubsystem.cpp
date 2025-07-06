// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include <OnlineSubsystem.h>
#include <Blueprint/UserWidget.h>
#include <Online/OnlineSessionNames.h>

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
	: CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
	, FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete))
	, JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
	, DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete))
	, StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

FName UMultiplayerSessionsSubsystem::GetOnlineSubsystemName() const {
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		return Subsystem->GetSubsystemName();
	}
	else {
		return FName();
	}
}

bool UMultiplayerSessionsSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	if (!SessionInterface.IsValid()) {
		return false;
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		SessionInterface->DestroySession(NAME_GameSession);
	}

	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	/*
	SessionSettings.bAllowJoinInProgress = true; // 是否允许加入
	SessionSettings.bIsDedicated = false; // 是否为专用服务器
	SessionSettings.bShouldAdvertise = true; // 是否广播
	SessionSettings.NumPrivateConnections = 10; // 连接数量
	//SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true; // 是否跨区域
	SessionSettings.bAllowJoinViaPresence = true; // 是否允许通过在线状态加入
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	*/

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings)) {
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		MultiplayerOnCreateSessionComplete.Broadcast(false);
		return false;
	}

	return true;
}

bool UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid()) {
		return false;
	}

	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	LastSearchResults.Empty();

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef())) {
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		MultiplayerOnFindSessionsComplete.Broadcast(LastSearchResults, false);
		return false;
	}
	return true;
}

bool UMultiplayerSessionsSubsystem::JoinSession(const FMultiplayerSessionsSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid()) {
		MultiplayerOnJoinSessionComplete.Broadcast("UnknownError");
		return false;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult.Result)) {
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast("UnknownError");
		return false;
	}
	return true;
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

FString UMultiplayerSessionsSubsystem::GetResolvedConnectString()
{
	if (SessionInterface != nullptr) {
		FString Address;
		SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
		return Address;
	}
	return FString();
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface != nullptr) {
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful) {
		for (const auto& SearchResult : LastSessionSearch->SearchResults) {
			FMultiplayerSessionsSearchResult Result;
			Result.Result = SearchResult;
			LastSearchResults.Add(Result);
		}
	}

	if (SessionInterface != nullptr) {
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	MultiplayerOnFindSessionsComplete.Broadcast(LastSearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface != nullptr) {
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	switch (Result) {
	case EOnJoinSessionCompleteResult::Type::Success:
		MultiplayerOnJoinSessionComplete.Broadcast("Success");
		break;
	case EOnJoinSessionCompleteResult::Type::SessionIsFull:
		MultiplayerOnJoinSessionComplete.Broadcast("SessionIsFull");
		break;
	case EOnJoinSessionCompleteResult::Type::SessionDoesNotExist:
		MultiplayerOnJoinSessionComplete.Broadcast("SessionDoesNotExist");
		break;
	case EOnJoinSessionCompleteResult::Type::CouldNotRetrieveAddress:
		MultiplayerOnJoinSessionComplete.Broadcast("CouldNotRetrieveAddress");
		break;
	case EOnJoinSessionCompleteResult::Type::AlreadyInSession:
		MultiplayerOnJoinSessionComplete.Broadcast("AlreadyInSession");
		break;
	case EOnJoinSessionCompleteResult::Type::UnknownError:
		MultiplayerOnJoinSessionComplete.Broadcast("UnknownError");
		break;
	default:
		MultiplayerOnJoinSessionComplete.Broadcast("UnknownError");
		break;
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionsSubsystem::SetInputModeUIOnly(APlayerController* PlayerController, UUserWidget* Widget)
{
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Widget->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
}

void UMultiplayerSessionsSubsystem::SetInputModeGameOnly(APlayerController* PlayerController, UUserWidget* Widget)
{
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(false);
}

void UMultiplayerSessionsSubsystem::ServerTravel(APlayerController* PlayerController, const FString& FURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	PlayerController->GetWorld()->ServerTravel(FURL, bAbsolute, bShouldSkipGameNotify);
}

void UMultiplayerSessionsSubsystem::ClientTravel(APlayerController* PlayerController, const FString& URL, ETravelType TravelType, bool bSeamless)
{
	PlayerController->ClientTravel(URL, TravelType, bSeamless);
}
