// Fill out your copyright notice in the Description page of Project Settings.


#include "MySpawningActor.h"


// Sets default values
AMySpawningActor::AMySpawningActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void AMySpawningActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicates(true);
	SetReplicateMovement(true);
	BaseMesh->SetSimulatePhysics(true);
}

// Called every frame
void AMySpawningActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

