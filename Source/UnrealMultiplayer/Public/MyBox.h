// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBox.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UTextRenderComponent;

UCLASS()
class UNREALMULTIPLAYER_API AMyBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyComponent")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MyComponent")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyComponent")
	TObjectPtr<UTextRenderComponent> TextRender;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MyComponent")
	TObjectPtr<UTextRenderComponent> ValueTextRender;

	//UPROPERTY(Replicated, BlueprintReadWrite, Category = "MyComponent")
	UPROPERTY(ReplicatedUsing=OnRep_RepValue, BlueprintReadWrite, Category = "MyComponent")
	int32 RepValue;

	UFUNCTION()
	void CheckAuthority();

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void UpdateRepValue(int32 newValue);

	UFUNCTION(BlueprintCallable, Category = "MyFunction")
	void OnRep_RepValue();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
