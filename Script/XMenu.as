class XMenu: UUserWidget {
    UPROPERTY(BindWidget)
    UButton HostButton;

    UPROPERTY(BindWidget)
    UButton JoinButton;

    int NumPublicConnections = 4;
    FString MatchType = "FreeForAll";

    private UMultiplayerSessionsSubsystem MultiplayerSessionsSubsystem;

    UFUNCTION(BlueprintOverride)
    void Construct()
    {
        HostButton.OnClicked.AddUFunction(this, n"HostButtonClicked");
        JoinButton.OnClicked.AddUFunction(this, n"JoinButtonClicked");
        //MenuSetup();
    }

    UFUNCTION(BlueprintOverride)
    void Destruct()
    {
        MenuTearDown();
    }

    UFUNCTION()
    void MenuSetup(int NumPublicConnections_ = 4, FString MatchType_ = "FreeForAll") {
        NumPublicConnections = NumPublicConnections_;
        MatchType = MatchType_;

        AddToViewport();
        SetVisibility(ESlateVisibility::Visible);
        bIsFocusable = true;
        auto PlayerController = OwningPlayer; //Gameplay::GetPlayerController(0);
        if(PlayerController != nullptr) {
            UMultiplayerSessionsSubsystem::SetInputModeUIOnly(PlayerController, this);
        }

        MultiplayerSessionsSubsystem = UMultiplayerSessionsSubsystem::Get();

        if(MultiplayerSessionsSubsystem != nullptr) {
            Print(f"~~~ OnlineSubsystemName: {MultiplayerSessionsSubsystem.GetOnlineSubsystemName()}");
            MultiplayerSessionsSubsystem.MultiplayerOnCreateSessionComplete.AddUFunction(this, n"OnCreateSession");
            MultiplayerSessionsSubsystem.MultiplayerOnFindSessionsComplete.AddUFunction(this, n"OnFindSessions");
            MultiplayerSessionsSubsystem.MultiplayerOnJoinSessionComplete.AddUFunction(this, n"OnJoinSession");
            MultiplayerSessionsSubsystem.MultiplayerOnDestroySessionComplete.AddUFunction(this, n"OnDestroySession");
            MultiplayerSessionsSubsystem.MultiplayerOnStartSessionComplete.AddUFunction(this, n"OnStartSession");
        }
    }

    UFUNCTION()
    void MenuTearDown() {
        //RemoveFromParent();
        auto PlayerController = OwningPlayer;
        if(PlayerController != nullptr) {
            UMultiplayerSessionsSubsystem::SetInputModeGameOnly(PlayerController, this);
        }
    }

    UFUNCTION()
    void OnCreateSession(bool bWasSuccessful) {
        Print(f"~~~ OnCreateSession {bWasSuccessful}", 5.0f, bWasSuccessful ? FLinearColor::Blue : FLinearColor::Red);
        if(bWasSuccessful) {
            //GetWorld().ServerTravel("/Game/_Multiplayer2/LobbyLevel?Listen", true, false);
            UMultiplayerSessionsSubsystem::ServerTravel(OwningPlayer, "/Game/_Multiplayer2/LobbyLevel?Listen", true);
        }
    }

    UFUNCTION()
    void OnFindSessions(const TArray<FMultiplayerSessionsSearchResult>&in SearchResults, bool bWasSuccessful) {
        Print(f"~~~ OnFindSessions: {bWasSuccessful} count: {SearchResults.Num()}");
        Print(f"~~~ NeedMatchType: {MatchType}");
        for(auto& SearchResult: SearchResults) {
            auto MatchType_ = SearchResult.GetSettingsStrValue("MatchType");
            Print(f"~~~ FoundSession: {SearchResult.OwningUserName} : {MatchType_}");
            if(SearchResult.GetSettingsStrValue("MatchType") == MatchType) {
                Print(f"~~~ JoinSession: {SearchResult.GetOwningUserName()} count: {SearchResults.Num()}");
                MultiplayerSessionsSubsystem.JoinSession(SearchResult);
                return;
            }
        }
    }

    UFUNCTION()
    void OnJoinSession(FString Result) {
        Print(f"~~~ OnJoinSession Result: {Result}");
        if(Result == "Success") {
            FString Address = MultiplayerSessionsSubsystem.GetResolvedConnectString();
            Print(f"~~~ Address: {Address}");
            UMultiplayerSessionsSubsystem::ClientTravel(OwningPlayer, Address, ETravelType::TRAVEL_Absolute);
            //Gameplay::OpenLevel(FName(Address), true); //? 暂无ClientTravel接口 由PlayerController提供 PlayerController.ClientTravel(xxx, true);
        }
    }

    UFUNCTION()
    void OnDestroySession(bool bWasSuccessful) {

    }

    UFUNCTION()
    void OnStartSession(bool bWasSuccessful) {
    }

    UFUNCTION()
    void HostButtonClicked() {
        Print("~~~ Host Button Clicked", 5.0f, FLinearColor::Blue);
        if(MultiplayerSessionsSubsystem != nullptr) {
            if(MultiplayerSessionsSubsystem.CreateSession(NumPublicConnections, MatchType)) {
            }
        }
    }

    UFUNCTION()
    void JoinButtonClicked() {
        Print("~~~ Join Button Clicked", 5.0f, FLinearColor::Blue);
        if(MultiplayerSessionsSubsystem != nullptr) {
            MultiplayerSessionsSubsystem.FindSessions(10000);
        }
    }
}
