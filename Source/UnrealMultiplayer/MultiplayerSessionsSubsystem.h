#pragma once

#include "CoreMinimal.h"
#include "ScriptGameInstanceSubsystemEx.h"
#include <Interfaces/OnlineSessionInterface.h>
#include <OnlineSessionSettings.h>
#include "MultiplayerSessionsSubsystem.generated.h"

USTRUCT(BlueprintType)
struct UNREALMULTIPLAYER_API FMultiplayerSessionsSearchResult {
	GENERATED_BODY()
public:
	FOnlineSessionSearchResult Result;
};

UCLASS(Meta = (ScriptMixin = "FMultiplayerSessionsSearchResult"))
class UNREALMULTIPLAYER_API UMultiplayerSessionsSearchResultMixinLibrary : public UObject {
	GENERATED_BODY()

public:
	UFUNCTION(ScriptCallable)
	static int32 GetPingInMs(const FMultiplayerSessionsSearchResult& Object) {
		return Object.Result.PingInMs;
	}

	UFUNCTION(ScriptCallable)
	static FString OwningUserName(const FMultiplayerSessionsSearchResult& Object) {
		return Object.Result.Session.OwningUserName;
	}

	UFUNCTION(ScriptCallable)
	static int32 GetNumOpenPrivateConnections(const FMultiplayerSessionsSearchResult& Object) {
		return Object.Result.Session.NumOpenPrivateConnections;
	}

	UFUNCTION(ScriptCallable)
	static int32 GetNumOpenPublicConnections(const FMultiplayerSessionsSearchResult& Object) {
		return Object.Result.Session.NumOpenPublicConnections;
	}

	UFUNCTION(ScriptCallable)
	static FString GetSettingsStrValue(const FMultiplayerSessionsSearchResult& Object, const FString& Key, const FString& Default = "") {
		FString Value = Default;
		Object.Result.Session.SessionSettings.Get(FName(Key), Value);
		return Value;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FMultiplayerSessionsSearchResult>&, SearchResults, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, FString, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

UCLASS(Blueprintable)
class UNREALMULTIPLAYER_API UMultiplayerSessionsSubsystem : public UScriptGameInstanceSubsystemEx
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();

public:
	UFUNCTION(BlueprintCallable)
	bool CreateSession(int32 NumPublicConnections, FString MatchType);

	UFUNCTION(BlueprintCallable)
	bool FindSessions(int32 MaxSearchResults);

	UFUNCTION(BlueprintCallable)
	bool JoinSession(const FMultiplayerSessionsSearchResult& SessionResult);

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	FString GetResolvedConnectString();

	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;

	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;

	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;

	UPROPERTY(BlueprintAssignable)
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;
	TArray<FMultiplayerSessionsSearchResult> LastSearchResults;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

public:
	UFUNCTION(BlueprintCallable)
	static void SetInputModeUIOnly(APlayerController* PlayerController, UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	static void SetInputModeGameOnly(APlayerController* PlayerController, UUserWidget* Widget);
};
