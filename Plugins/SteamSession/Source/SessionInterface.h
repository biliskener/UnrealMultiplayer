// Copyright 2025 (c) Tangha Technologies, LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SteamSessionSubSystem.h"
#include "UObject/Interface.h"
#include "SessionInterface.generated.h"



/*
* Session Setting
*/
#define SESSION_ROOM_NAME FName(TEXT("SessionRoomName"))


UINTERFACE(Blueprintable)
class STEAMSESSION_API USessionInterface : public UInterface
{
	GENERATED_BODY()
};


class STEAMSESSION_API ISessionInterface
{
	GENERATED_BODY()

public: 

	/*
	* 绑定session 相关的一些回调
	*/
	void InitSession(const UObject* WorldContextObject);

	bool GetJoinAddressFromSession(const UObject* WorldContextObject, const FName& SessionName, const uint8 ResultType, FString& Address);

protected:

	/*
	* 查找session
	*/
	void FindSession(const UObject* WorldContextObject);
	/*
	* 查找session 成功回调
	*/
	UFUNCTION()
	virtual void FindSessionCallback(const bool bIsSucceed, const TArray<FFindSessionResult>& AllResults) {}

	/*
	* 加入房间Session
	*/
	void JoinSession(const UObject* WorldContextObject, const int32 SessionID);
	/*
	* 加入房间 Session 成功回调
	*/
	UFUNCTION()
	virtual void JoinSessionCallback(const uint8 ResultType, const FName& SessionName) {}


	void DestroySession(const UObject* WorldContextObject);

	/*
	* 销毁房间 Session 成功回调
	*/
	UFUNCTION()
	virtual void DestroySessionCallback(const bool bIsSucceed, const FName& SessionName) {}

	/*
	* 创建房间Session
	*/
	void CreateSession(const UObject* WorldContextObject, FString RoomName, const int32 RoomLimits = 2);
	/*
	* 创建房间 Session 成功回调
	*/
	UFUNCTION()
	virtual void CreateSessionCallback(const bool bIsSucceed, const FName& SessionName) {}

	/*
	* 开始房间Session
	*/
	void StartSession(const FName& SessionName, const UObject* WorldContextObject);
	/*
	* 开始房间 Session 成功回调
	*/
	UFUNCTION()
	virtual void StartSessionCallback(const bool bIsSucceed, const FName& SessionName) {}

};
