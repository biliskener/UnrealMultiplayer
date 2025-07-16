// Copyright 2025 (c) Tangha Technologies, LLC. All Rights Reserved.

#include "SessionVoiceHelper.h"
#include "SteamSession.h"

#include "Online.h"
#include "Online/CoreOnline.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/VoiceInterface.h"
#include "UObject/UObjectIterator.h"
#include "GameFramework/OnlineReplStructs.h"


void USessionVoiceHelper::IsHeadsetPresent(UObject* WorldContextObject, bool & bHasHeadset, uint8 LocalPlayerNum)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		bHasHeadset = false;
		UE_LOG(LogSteamSession, Warning, TEXT("Check For Headset couldn't get the voice interface!"));
		return;
	}

	bHasHeadset = VoiceInterface->IsHeadsetPresent(LocalPlayerNum);
}

void USessionVoiceHelper::StartNetworkedVoice(UObject* WorldContextObject, uint8 LocalPlayerNum)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Start Networked Voice couldn't get the voice interface!"));
		return;
	}

	VoiceInterface->StartNetworkedVoice(LocalPlayerNum);
}

void USessionVoiceHelper::StopNetworkedVoice(UObject* WorldContextObject, uint8 LocalPlayerNum)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Start Networked Voice couldn't get the voice interface!"));
		return;
	}

	VoiceInterface->StopNetworkedVoice(LocalPlayerNum);
}

bool USessionVoiceHelper::RegisterLocalTalker(UObject* WorldContextObject, uint8 LocalPlayerNum)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Register Local Talker couldn't get the voice interface!"));
		return false;
	}

	return VoiceInterface->RegisterLocalTalker(LocalPlayerNum);
}

void USessionVoiceHelper::UnRegisterLocalTalker(UObject* WorldContextObject, uint8 LocalPlayerNum)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Unregister Local Talker couldn't get the voice interface!"));
		return;
	}

	VoiceInterface->UnregisterLocalTalker(LocalPlayerNum);
}

bool USessionVoiceHelper::RegisterRemoteTalker(UObject* WorldContextObject, FUniqueNetIdRepl RemotePlayerNetId)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Register Local Talker couldn't get the voice interface!"));
		return false;
	}

	return VoiceInterface->RegisterRemoteTalker(*RemotePlayerNetId.GetUniqueNetId());
}

void USessionVoiceHelper::RegisterAllLocalTalkers(UObject* WorldContextObject)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Register Local Talkers couldn't get the voice interface!"));
		return;
	}

	VoiceInterface->RegisterLocalTalkers();
}

bool USessionVoiceHelper::UnRegisterRemoteTalker(UObject* WorldContextObject, FUniqueNetIdRepl RemotePlayerNetId)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Register Local Talker couldn't get the voice interface!"));
		return false;
	}

	return VoiceInterface->RegisterRemoteTalker(*RemotePlayerNetId.GetUniqueNetId());
}

void USessionVoiceHelper::UnRegisterAllLocalTalkers(UObject* WorldContextObject)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("UnRegister All Local Talkers couldn't get the voice interface!"));
		return;
	}

	VoiceInterface->UnregisterLocalTalkers();
}

void USessionVoiceHelper::RemoveAllRemoteTalkers(UObject* WorldContextObject)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Remove All Remote Talkers couldn't get the voice interface!"));
		return;
	}

	VoiceInterface->RemoveAllRemoteTalkers();
}

bool USessionVoiceHelper::MuteRemoteTalker(UObject* WorldContextObject, uint8 LocalUserNum, FUniqueNetIdRepl RemotePlayerNetId, bool bIsSystemWide)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Register Local Talker couldn't get the voice interface!"));
		return false;
	}

	return VoiceInterface->MuteRemoteTalker(LocalUserNum, *RemotePlayerNetId.GetUniqueNetId(), bIsSystemWide);
}

bool USessionVoiceHelper::UnMuteRemoteTalker(UObject* WorldContextObject, uint8 LocalUserNum, FUniqueNetIdRepl RemotePlayerNetId, bool bIsSystemWide)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Register Local Talker couldn't get the voice interface!"));
		return false;
	}

	return VoiceInterface->UnmuteRemoteTalker(LocalUserNum, *RemotePlayerNetId.GetUniqueNetId(), bIsSystemWide);
}

bool USessionVoiceHelper::IsLocalPlayerTalking(UObject* WorldContextObject, uint8 LocalPlayerNum)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return false;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		UE_LOG(LogSteamSession, Warning, TEXT("Is Local Player Talking couldn't get the voice interface!"));
		return false;
	}

	return VoiceInterface->IsLocalPlayerTalking(LocalPlayerNum);
}

void USessionVoiceHelper::GetNumLocalTalkers(UObject* WorldContextObject, int32 & NumLocalTalkers)
{

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);

	if (!VoiceInterface.IsValid())
	{
		NumLocalTalkers = 0;
		UE_LOG(LogSteamSession, Warning, TEXT("Unmute Remote Talker couldn't get the voice interface!"));
		return;
	}

	NumLocalTalkers = VoiceInterface->GetNumLocalTalkers();
}