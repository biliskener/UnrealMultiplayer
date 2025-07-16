// Copyright 2025 (c) Tangha Technologies, LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SteamSessionSubSystem.generated.h"


/*
* Sessiong Setting, 的成对的属性的设置
*/
USTRUCT(BlueprintType)
struct FSessionSettingPairKeyValue
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSetting")
	FName SessionPairKey = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSetting")
	FString SessionPairValue = "";


	FSessionSettingPairKeyValue()
	: SessionPairKey("")
	, SessionPairValue("")
	{}

	FSessionSettingPairKeyValue(FName Key, FString Value)
	{
		SessionPairKey = Key;
		SessionPairValue = Value;
	}

	bool operator==(const FName& OtherSessionSettingKey) const
	{
		return OtherSessionSettingKey == SessionPairKey;
	}
};

/*
* 查找 Session 的结果
*/
USTRUCT(BlueprintType)
struct FFindSessionResult
{
	GENERATED_BODY()

	FFindSessionResult()
	: PingInMs(0)
	, OwningUserName("")
	, NumOpenPrivateConnections(0)
	, NumOpenPublicConnections(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearch")
	int32 PingInMs = 0;

	/** Owner name of the session */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearch")
	FString OwningUserName = "";

	/** The number of private connections that are available (read only) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearch")
	int32 NumOpenPrivateConnections = 0;

	/** The number of publicly available connections that are available (read only) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearch")
	int32 NumOpenPublicConnections = 0;

	/** 回调获取到客户端设置的 KEY VALUE */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SessionSearch")
	TArray<FSessionSettingPairKeyValue> SessionSettings;

};


// Search Session Delegate 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomizeFindSessionComplete, const bool, bIsSucceed, const TArray<FFindSessionResult>&, AllResults);
// Create Session Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomizeCreateSessionComplete, const bool, bIsSucceed, const FName&, SessionName);
// Join Session Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomizeJoinSessionComplete, const uint8, ResultType, const FName&, SessionName);
// Start Session Delegate 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomizeStartSessionComplete, const bool, bIsSucceed, const FName&, SessionName);
// End Session Delegate 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomizeEndSessionComplete, const bool, bIsSucceed, const FName&, SessionName);
// Destroy Session Delegate 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomizeDestroySessionComplete, const bool, bIsSucceed, const FName&, SessionName);

UCLASS(BlueprintType)
class STEAMSESSION_API USteamSessionSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	/*
	* 获取到join 到服务器的Address
	*/
	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	void GetJoinAddress(const FName& SessionName, FString& JoinAddress);

	/*
	* 查找房间Session服务器
	* bIsLanQuery 是否查找 lan 网络
	* MaxSearchResults  最大搜索结果为 10000 条
	* bIsPresence 只查询 presence 值为 true 的
	*/
	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	bool FindRoomSession(
		const bool bIsLanQuery = false, 
		const int32 MaxSearchResults = 10000, 
		const bool bIsPresence = true
	);
	UPROPERTY(BlueprintAssignable)
	FOnCustomizeFindSessionComplete OnFindSession;

	/*
	*	创建监听一个房间Session 服务器
	* 	bIsLANMatch = false; // 设置为 外部玩家可见的非局域网匹配
	*	NumPublicConnections = 3; // 可以有 3 个玩家连接
	*	bAllowJoinInProgress = false; // 是否允许加入正在运行的游戏
	*	bAllowJoinViaPresence = true; // 允许区域玩家加入
	*	bShouldAdvertise = true; // 在在线服务上公开发布
	*	bUsesPresence = true;  // 显示用户状态信息
	*	bUseLobbiesIfAvailable = true; // 支持 Lobbies Api，不开启可能无法找到 Session
	*/
	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	bool CreateRoomSession(
		const TArray<FSessionSettingPairKeyValue>& Pairs,
		const bool bIsLANMatch = false, 
		const int32 NumPublicConnections = 3, 
		const bool bAllowJoinInProgress = true, 
		const bool bAllowJoinViaPresence = true,
		const bool bShouldAdvertise = true,
		const bool bUsesPresence = true,
		const bool bUseLobbiesIfAvailable = true
		);

	UPROPERTY(BlueprintAssignable, Category = "SteamSession")
	FOnCustomizeCreateSessionComplete OnCreateSession;

	/*
	* 启动一个房间Session 服务器
	*/
	bool StartRoomSession(const FName& SessionName);

	UPROPERTY(BlueprintAssignable, Category = "SteamSession")
	FOnCustomizeStartSessionComplete OnStartSession;

	/*
	* 结束一个房间Session 服务器
	*/
	bool EndRoomSession();

	UPROPERTY(BlueprintAssignable, Category = "SteamSession")
	FOnCustomizeEndSessionComplete OnEndSession;

	/*
	* 销毁一个房间Session 服务器
	*/
	bool DestroyRoomSession(const FName& SessionName = NAME_GameSession);

	UPROPERTY(BlueprintAssignable, Category = "SteamSession")
	FOnCustomizeDestroySessionComplete OnDestroySession;

	/*
	* 加入一个房间Session服务器
	* SessionIndex, 就是之前搜索到的 全部的session results 中的位置 Index
	*/
	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	bool JoinRoomSession(const int32 SessionIndex);

	UPROPERTY(BlueprintAssignable, Category = "SteamSession")
	FOnCustomizeJoinSessionComplete OnJoinSession;

	/*
	* 从 SessionIndex  获取到 当前的Sesion的数据
	*/
	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	bool GetSessionInfoBySessionIndex(const int32 SessionIndex, FFindSessionResult& Result);

	/*
	* 从 SessionIndex  获取到 当前的Sesion的全部数据
	*/
	bool GetSessionFullInfoBySessionIndex(const int32 SessionIndex, FOnlineSessionSearchResult& Result);

	// Set Session Setting Keys
	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	void SetSessionSettingKeys(const TArray<FName>& Keys);

	// Get Session Setting
	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	TArray<FName> GetSessionSettingKeys() const;

	bool IsExistedSession(const FName& SessionName = NAME_GameSession);

	UFUNCTION(BlueprintCallable, Category = "SteamSession")
	FString GetSteamIdAsString();



protected:

	void InitSession();

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnStartSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnEndSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnDestroySessionComplete(FName SessionName, bool Succeeded);

private:

	// 用于查找其他服务器Server Session
	TSharedPtr<FOnlineSessionSearch> SessionSearch = nullptr;	
	
	// 持有的唯一 Session 管理
	IOnlineSessionPtr SessionInterface = nullptr;		
	
	// 每次搜索session, 存储的完整的session 的搜索到的全部的结果
	TArray<FOnlineSessionSearchResult> SearchResults;

	// 每次搜索到的 可以供蓝图和C++使用的Session的数据
	TArray<FFindSessionResult> AvailableSearchResults;

	// 存储关系的 Session Setting Key
	TArray<FName> SessionSettingKeys;

	// 创建 Session 的时候相关参数配置
	TSharedPtr<FOnlineSessionSettings> CreateSessionSetting = nullptr;

};
