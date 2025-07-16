// Copyright 2025 (c) Tangha Technologies, LLC. All Rights Reserved.

#include "SteamSessionSubSystem.h"
#include "SteamSession.h"


#include "ThirdParty/Steamworks/Steamv157/sdk/public/steam/steam_api.h"

#include "OnlineSubsystemSessionSettings.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerController.h"
#include "Online.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"



void USteamSessionSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InitSession();

	UE_LOG(LogSteamSession, Log, TEXT("%s()"), *FString(__FUNCTION__));
}

void USteamSessionSubSystem::Deinitialize()
{
	UE_LOG(LogSteamSession, Log, TEXT("%s() Deinitialize Steam Session GameInstance Subsystem."), *FString(__FUNCTION__));

	Super::Deinitialize();
}

void USteamSessionSubSystem::GetJoinAddress(const FName& SessionName, FString& JoinAddress)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
}

bool USteamSessionSubSystem::FindRoomSession(
	const bool bIsLanQuery,
	const int32 MaxSearchResults,
	const bool bIsPresence)
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = bIsLanQuery;
	SessionSearch->MaxSearchResults = MaxSearchResults;
	if (bIsPresence)
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	}

	return SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void USteamSessionSubSystem::OnFindSessionComplete(bool Succeeded)
{
	if (!Succeeded || !SessionSearch)
	{
		OnFindSession.Broadcast(false, TArray<FFindSessionResult>());
		return;
	}

	SearchResults.Empty();
	SearchResults = SessionSearch->SearchResults;

	AvailableSearchResults.Empty();
	for (FOnlineSessionSearchResult& Result : SearchResults)
	{
		if (!Result.IsValid())
		{
			continue;
		}
		// Temp for 5.5, force the values if epic isn't setting them, lobbies should always have these true
		Result.Session.SessionSettings.bUseLobbiesIfAvailable = true;
		Result.Session.SessionSettings.bUsesPresence = true;
		FFindSessionResult OneResult;
		OneResult.PingInMs = Result.PingInMs;
		OneResult.OwningUserName = Result.Session.OwningUserName;
		OneResult.NumOpenPrivateConnections = Result.Session.NumOpenPrivateConnections;
		OneResult.NumOpenPublicConnections = Result.Session.NumOpenPublicConnections;

		for (auto OneSessionSettingKey : SessionSettingKeys)
		{
			FString OneSessionSettingValue;
			Result.Session.SessionSettings.Get(OneSessionSettingKey, OneSessionSettingValue);
			OneResult.SessionSettings.Add({ OneSessionSettingKey , OneSessionSettingValue });
		}
		AvailableSearchResults.Add(OneResult);
	}
	OnFindSession.Broadcast(true, AvailableSearchResults);
}

bool USteamSessionSubSystem::CreateRoomSession(
	const TArray<FSessionSettingPairKeyValue>& Pairs,
	const bool bIsLANMatch, 
	const int32 NumPublicConnections, 
	const bool bAllowJoinInProgress, 
	const bool bAllowJoinViaPresence, 
	const bool bShouldAdvertise, 
	const bool bUsesPresence, 
	const bool bUseLobbiesIfAvailable)
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	// 创建一个存放 Session 设置的对象指针
	CreateSessionSetting = MakeShareable(new FOnlineSessionSettings());
	CreateSessionSetting->bIsLANMatch = bIsLANMatch; // 设置为 外部玩家可见的非局域网匹配
	CreateSessionSetting->NumPublicConnections = NumPublicConnections; // 可以有 3 个玩家连接
	CreateSessionSetting->bAllowJoinInProgress = bAllowJoinInProgress; // 允许加入正在运行的游戏
	CreateSessionSetting->bAllowJoinViaPresence = bAllowJoinViaPresence; // 允许区域玩家加入
	CreateSessionSetting->bShouldAdvertise = bShouldAdvertise; // 在在线服务上公开发布
	CreateSessionSetting->bUsesPresence = bUsesPresence;  // 显示用户状态信息
	CreateSessionSetting->bUseLobbiesIfAvailable = bUseLobbiesIfAvailable; // 支持 Lobbies Api，不开启可能无法找到 Session
	CreateSessionSetting->bUseLobbiesVoiceChatIfAvailable = true;
	for (auto OnePair : Pairs)
	{
		CreateSessionSetting->Set(OnePair.SessionPairKey, OnePair.SessionPairValue, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}

	// NAME_GameSession 是个常量，表示当前会话名
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) 
	{
		// 如果存在，则销毁 Session
		SessionInterface->DestroySession(NAME_GameSession);
		return true;
	}

	return SessionInterface->CreateSession(0, NAME_GameSession, *CreateSessionSetting);
}

void USteamSessionSubSystem::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	OnCreateSession.Broadcast(Succeeded, SessionName);
}

bool USteamSessionSubSystem::StartRoomSession(const FName& SessionName)
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	return SessionInterface->StartSession(SessionName);
}

bool USteamSessionSubSystem::EndRoomSession()
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (!ExistingSession)
	{
		return false;
	}

	return SessionInterface->EndSession(NAME_GameSession);
}

bool USteamSessionSubSystem::DestroyRoomSession(const FName& SessionName)
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);
	if (!ExistingSession)
	{
		return true;
	}

	return SessionInterface->DestroySession(SessionName);
}

void USteamSessionSubSystem::OnStartSessionComplete(FName SessionName, bool Succeeded)
{
	OnStartSession.Broadcast(Succeeded, SessionName);
}

void USteamSessionSubSystem::OnEndSessionComplete(FName SessionName, bool Succeeded)
{
	OnEndSession.Broadcast(Succeeded, SessionName);
}

bool USteamSessionSubSystem::JoinRoomSession(const int32 SessionIndex)
{
	if (!SessionInterface.IsValid())
	{
		return false;
	}

	if (SessionIndex >= SearchResults.Num())
	{
		return false;
	}

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
	{
		return false;
	}

	return SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResults[SessionIndex]);
}

void USteamSessionSubSystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		// Client travel to the server
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PC)
		{
			return;
		}

		FString ConnectString;
		if (SessionInterface->GetResolvedConnectString(NAME_GameSession, ConnectString))
		{
			UE_LOG_ONLINE_SESSION(Log, TEXT("Join session: traveling to %s"), *ConnectString);
			PC->ClientTravel(ConnectString, TRAVEL_Absolute);
		}
	}

	OnJoinSession.Broadcast((uint8)Result, SessionName);
}

void USteamSessionSubSystem::OnDestroySessionComplete(FName SessionName, bool Succeeded)
{
	OnDestroySession.Broadcast(Succeeded, SessionName);
}

bool USteamSessionSubSystem::GetSessionInfoBySessionIndex(const int32 SessionIndex, FFindSessionResult& Result)
{
	if (SessionIndex < 0 || SessionIndex >= AvailableSearchResults.Num())
	{
		return false;
	}

	Result = AvailableSearchResults[SessionIndex];
	return true;
}

bool USteamSessionSubSystem::GetSessionFullInfoBySessionIndex(const int32 SessionIndex, FOnlineSessionSearchResult& Result)
{
	if (SessionIndex < 0 || SessionIndex >= SearchResults.Num())
	{
		return false;
	}

	Result = SearchResults[SessionIndex];
	return true;
}

void USteamSessionSubSystem::SetSessionSettingKeys(const TArray<FName>& Keys)
{
	SessionSettingKeys = Keys;
}

TArray<FName> USteamSessionSubSystem::GetSessionSettingKeys() const
{
	return SessionSettingKeys;
}

bool USteamSessionSubSystem::IsExistedSession(const FName& SessionName)
{
	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);
	if (ExistingSession != nullptr)
	{
		return true;
	}
	return false;
}

FString USteamSessionSubSystem::GetSteamIdAsString()
{
	FString returnvalue;
	if (SteamAPI_Init())
	{
		uint64 Sid{ 0 };
		CSteamID InSteamID = SteamUser()->GetSteamID();
		Sid = InSteamID.ConvertToUint64();
		if (Sid == 0)
		{
			returnvalue = "BadSteamId";
		}
		else
		{
			returnvalue = FString::Printf(TEXT("%llu"), Sid);
		}
	}
	else
	{

		returnvalue = "NoSteamConnection";
	}

	return returnvalue;
}


void USteamSessionSubSystem::InitSession()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return;
	}

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid() || !GEngine)
	{
		return;
	}

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);
	SessionInterface->OnStartSessionCompleteDelegates.AddUObject(this, &ThisClass::OnStartSessionComplete);
	SessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &ThisClass::OnEndSessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionComplete);


	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			-1, // 不会替换之前打印的消息
			15.f, // 打印的内容停留 15 秒
			FColor::Blue, // 文字颜色为蓝色
			FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()) // 获取子系统名字
		);
	}
}
