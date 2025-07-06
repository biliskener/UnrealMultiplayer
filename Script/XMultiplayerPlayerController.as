class XMultiplayerPlayerController: AUnrealMultiplayerPlayerController {
    UFUNCTION(BlueprintOverride)
    void BeginPlay()
    {
        this.OnPossessedPawnChanged.AddUFunction(this, n"OnControlledPawnChanged");
    }

    UFUNCTION()
    void OnControlledPawnChanged(APawn OldPawn, APawn NewPawn) {
    }
}
