class XMultiplayerHeadUI: UUserWidget {
    UPROPERTY(BindWidget)
    UTextBlock PlayerNameText;

    void SetPlayerName(const FString& PlayerName) {
        this.PlayerNameText.SetText(FText::FromString(PlayerName));
    }
}
