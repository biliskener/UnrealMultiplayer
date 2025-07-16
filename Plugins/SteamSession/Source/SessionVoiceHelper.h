// Copyright 2025 (c) Tangha Technologies, LLC. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SessionVoiceHelper.generated.h"


UCLASS()
class USessionVoiceHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	//********* Voice Library Functions *************//

	// Get if a headset is present for the specified local user
	UFUNCTION(BlueprintPure, Category = "Online|VoiceHelper|VoiceInfo", meta = (WorldContext = "WorldContextObject"))
	static void IsHeadsetPresent(UObject* WorldContextObject, bool & bHasHeadset, uint8 LocalPlayerNum = 0);

	// Starts networked voice, allows push to talk in coordination with StopNetworkedVoice
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static void StartNetworkedVoice(UObject* WorldContextObject, uint8 LocalPlayerNum = 0);

	// Stops networked voice, allows push to talk in coordination with StartNetworkedVoice
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static void StopNetworkedVoice(UObject* WorldContextObject, uint8 LocalPlayerNum = 0);

	// Registers a local player as someone interested in voice data
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static bool RegisterLocalTalker(UObject* WorldContextObject, uint8 LocalPlayerNum = 0);

	// UnRegisters local player as a local talker
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static void UnRegisterLocalTalker(UObject* WorldContextObject, uint8 LocalPlayerNum = 0);

	// Registers a remote player as a talker
	// This is already done automatically, only do it manually if you unregistered someone
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static bool RegisterRemoteTalker(UObject* WorldContextObject, FUniqueNetIdRepl RemotePlayerNetId);

	// UnRegisters a remote player as a talker
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static bool UnRegisterRemoteTalker(UObject* WorldContextObject, FUniqueNetIdRepl RemotePlayerNetId);

	// UnRegisters all remote players as talkers
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static void RemoveAllRemoteTalkers(UObject* WorldContextObject);

	// Mutes the player associated with the uniquenetid for the specified local player, if IsSystemWide is true then it will attempt to mute globally for the player
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static bool MuteRemoteTalker(UObject* WorldContextObject, uint8 LocalUserNum, FUniqueNetIdRepl RemotePlayerNetId, bool bIsSystemWide = false);

	// UnMutes the player associated with the uniquenetid for the specified local player, if IsSystemWide is true then it will attempt to unmute globally for the player
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static bool UnMuteRemoteTalker(UObject* WorldContextObject, uint8 LocalUserNum, FUniqueNetIdRepl RemotePlayerNetId, bool bIsSystemWide = false);


	// Registers all signed in players as local talkers
	// This is already done automatically, only do it manually if you unregistered someone
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static void RegisterAllLocalTalkers(UObject* WorldContextObject);

	// UnRegisters all signed in players as local talkers
	UFUNCTION(BlueprintCallable, Category = "Online|VoiceHelper", meta = (WorldContext = "WorldContextObject"))
	static void UnRegisterAllLocalTalkers(UObject* WorldContextObject);

	// Returns whether a local player is currently talking
	UFUNCTION(BlueprintPure, Category = "Online|VoiceHelper|VoiceInfo", meta = (WorldContext = "WorldContextObject"))
	static bool IsLocalPlayerTalking(UObject* WorldContextObject, uint8 LocalPlayerNum);

	// Gets the number of local talkers for this system
	UFUNCTION(BlueprintPure, Category = "Online|VoiceHelper|VoiceInfo", meta = (WorldContext = "WorldContextObject"))
	static void GetNumLocalTalkers(UObject* WorldContextObject, int32 & NumLocalTalkers);
};	
