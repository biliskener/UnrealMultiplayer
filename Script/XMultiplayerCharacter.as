class XMultiplayerCharacter: AMyMultiplayerCharacter {
    UPROPERTY(DefaultComponent, Attach = CollisionCylinder)
    UWidgetComponent Widget;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "$$$$ Configuration")
    //UInputAction moveAction;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "$$$$ Configuration")
    //UInputAction escapeAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "$$$$ Configuration")
    TSubclassOf<UUserWidget> touchWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "$$$$ Configuration")
    UInputMappingContext inputMappingContext;

    bool bNameDirty = true;

    UFUNCTION(BlueprintOverride)
    void Possessed(AController NewController) // 仅在服务器时触发
    {
        if(NewController.IsPlayerController()) {
            auto PlayerController = Cast<APlayerController>(NewController);
            UEnhancedInputLocalPlayerSubsystem InputSubsystem = UEnhancedInputLocalPlayerSubsystem::Get(PlayerController);

            if(IsValid(InputSubsystem)) {
                if(true) {
                    InputSubsystem.AddMappingContext(inputMappingContext, 0, FModifyContextOptions());
                }
                else {
                    WidgetBlueprint::CreateWidget(touchWidgetClass, PlayerController).AddToPlayerScreen();
                }
            }

            bNameDirty = true;
        }
    }

    UFUNCTION(BlueprintOverride)
    void Tick(float DeltaSeconds)
    {
        if(bNameDirty) {
            if(PlayerState != nullptr) {
                auto HeadUI = Cast<XMultiplayerHeadUI>(Widget.GetWidget());
                HeadUI.SetPlayerName(PlayerState.GetPlayerName());
                bNameDirty = false;
                
            }
        }
    }
}
