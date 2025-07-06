class XMultiplayerGameMode: AUnrealMultiplayerGameMode {
    UFUNCTION(BlueprintOverride)
    void OnPostLogin(APlayerController NewPlayer)
    {
        Log(f"~~~~ OnPostLogin {NewPlayer.PlayerState.PlayerName}");
    }

    UFUNCTION(BlueprintOverride)
    void OnLogout(AController ExitingController)
    {
        Log(f"~~~~ OnLogout {ExitingController.PlayerState.PlayerName}");
    }
}
