// Copyright 2025 (c) Tangha Technologies, LLC. All Rights Reserved.


#include "SessionInterface.h"
#include "SteamSession.h"
#include "SteamSessionSubSystem.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"



void ISessionInterface::InitSession(const UObject* WorldContextObject)
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(CurrentWorld));
	if (!GI)
	{
		return;
	}

	USteamSessionSubSystem* SteamSession = GI->GetSubsystem<USteamSessionSubSystem>();
	if (!SteamSession)
	{
		return;
	}

	// SteamSession->InitSessionSubSystem();

	SteamSession->OnFindSession.AddDynamic(this, &ThisClass::FindSessionCallback);
	SteamSession->OnJoinSession.AddDynamic(this, &ThisClass::JoinSessionCallback);
	SteamSession->OnCreateSession.AddDynamic(this, &ThisClass::CreateSessionCallback);
	SteamSession->OnStartSession.AddDynamic(this, &ThisClass::StartSessionCallback);
	SteamSession->OnDestroySession.AddDynamic(this, &ThisClass::DestroySessionCallback);

	TArray<FName> SessionKeys;
	SessionKeys.Add(SESSION_ROOM_NAME);
	SteamSession->SetSessionSettingKeys(SessionKeys);
}

bool ISessionInterface::GetJoinAddressFromSession(const UObject* WorldContextObject, const FName& SessionName, const uint8 ResultType, FString& JoinAddress)
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(CurrentWorld));
	if (!GI)
	{
		return false;
	}

	USteamSessionSubSystem* SteamSession = GI->GetSubsystem<USteamSessionSubSystem>();
	if (!SteamSession)
	{
		return false;
	}

	EOnJoinSessionCompleteResult::Type Result = (EOnJoinSessionCompleteResult::Type)ResultType;
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogSteamSession, Error, TEXT("%s Join Session Success! session name: %s "), ANSI_TO_TCHAR(__FUNCTION__), *SessionName.ToString());

		SteamSession->GetJoinAddress(SessionName, JoinAddress);
		return true;
	}

	UE_LOG(LogSteamSession, Error, TEXT("%s Join Session Failed! session name: %s "), ANSI_TO_TCHAR(__FUNCTION__), *SessionName.ToString());
	return false;
}

void ISessionInterface::FindSession(const UObject* WorldContextObject)
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(CurrentWorld));
	if (!GI)
	{
		return;
	}

	USteamSessionSubSystem* SteamSession = GI->GetSubsystem<USteamSessionSubSystem>();
	if (!SteamSession)
	{
		return;
	}

	if (SteamSession->IsExistedSession())
	{
		SteamSession->DestroyRoomSession();
		return;
	}

	bool bIsLanQuery = false;
	int32 MaxSearchResults = 10000;
	if (!SteamSession->FindRoomSession(bIsLanQuery, MaxSearchResults, true))
	{
		UE_LOG(LogSteamSession, Error, TEXT("%s Search Session Failed!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void ISessionInterface::JoinSession(const UObject* WorldContextObject, const int32 SessionID)
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(CurrentWorld));
	if (!GI)
	{
		return;
	}

	USteamSessionSubSystem* SteamSession = GI->GetSubsystem<USteamSessionSubSystem>();
	if (!SteamSession)
	{
		return;
	}

	if (!SteamSession->JoinRoomSession(SessionID))
	{
		UE_LOG(LogSteamSession, Error, TEXT("%s Create Session Failed!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void ISessionInterface::DestroySession(const UObject* WorldContextObject)
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(CurrentWorld));
	if (!GI)
	{
		return;
	}

	USteamSessionSubSystem* SteamSession = GI->GetSubsystem<USteamSessionSubSystem>();
	if (!SteamSession)
	{
		return;
	}

	SteamSession->DestroyRoomSession();
}

void ISessionInterface::CreateSession(const UObject* WorldContextObject, FString RoomName, const int32 RoomLimits)
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(CurrentWorld));
	if (!GI)
	{
		return;
	}

	USteamSessionSubSystem* SteamSession = GI->GetSubsystem<USteamSessionSubSystem>();
	if (!SteamSession)
	{
		return;
	}

	if (SteamSession->IsExistedSession())
	{
		SteamSession->DestroyRoomSession();
		return;
	}

	if (RoomName.IsEmpty())
	{
		FDateTime Time = FDateTime::UtcNow();
		int32 CreateSessionID = Time.ToUnixTimestamp() % 100000;			// 如果是创建房间的， 直接就是房间名字
		RoomName = FString::FromInt(CreateSessionID);		// 暂时 以时间戳为房间名
	}

	TArray<FSessionSettingPairKeyValue> Pairs;
	Pairs.Add(FSessionSettingPairKeyValue(SESSION_ROOM_NAME, RoomName));
	if (!SteamSession->CreateRoomSession(Pairs, false, RoomLimits))		// use default parameter
	{
		UE_LOG(LogSteamSession, Error, TEXT("%s Req Create Session failed!"), ANSI_TO_TCHAR(__FUNCTION__));
	}

	// GI->SetChoosedRoomName(RoomName);
}

void ISessionInterface::StartSession(const FName& SessionName, const UObject* WorldContextObject)
{
	UWorld* CurrentWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UGameInstance* GI = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(CurrentWorld));
	if (!GI)
	{
		return;
	}

	USteamSessionSubSystem* SteamSession = GI->GetSubsystem<USteamSessionSubSystem>();
	if (!SteamSession)
	{
		return;
	}

	if (!SteamSession->StartRoomSession(SessionName))
	{
		UE_LOG(LogSteamSession, Error, TEXT("%s Start Session Failed! %s "), ANSI_TO_TCHAR(__FUNCTION__), *SessionName.ToString());
	}
}
