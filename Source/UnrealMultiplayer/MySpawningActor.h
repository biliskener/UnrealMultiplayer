// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySpawningActor.generated.h"


class UStaticMeshComponent;

UCLASS()
class UNREALMULTIPLAYER_API AMySpawningActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MyComponent")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

public:	
	// Sets default values for this actor's properties
	AMySpawningActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
