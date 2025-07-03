// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBox.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMyBox::AMyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(Root);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>("TextRender");
	TextRender->SetupAttachment(Root);

	ValueTextRender = CreateDefaultSubobject<UTextRenderComponent>("ValueTextRender");
	ValueTextRender->SetupAttachment(Root);

	RepValue = 50;
}

// Called when the game starts or when spawned
void AMyBox::BeginPlay()
{
	Super::BeginPlay();

	CheckAuthority();
	SetReplicates(true);
	SetReplicateMovement(true);

	GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AMyBox::DecreaseReplicated, 2.0f, false);
}

void AMyBox::CheckAuthority()
{
	auto localRole = GetLocalRole();
	if (localRole == ENetRole::ROLE_Authority) {
		TextRender->SetText(FText::FromString("Authority"));
	}
	else {
		TextRender->SetText(FText::FromString("No Authority"));
	}
}

void AMyBox::UpdateRepValue(int32 newValue)
{
	RepValue = newValue;
}

void AMyBox::OnRep_RepValue()
{
	FString valueStr = FString::Printf(TEXT("RepValue: %d"), RepValue);
	ValueTextRender->SetText(FText::FromString(valueStr));
}

void AMyBox::DecreaseReplicated()
{
	if (HasAuthority()) {
		RepValue--;
		OnRep_RepValue();
		if (RepValue > 0) {
			GetWorld()->GetTimerManager().SetTimer(TestTimer, this, &AMyBox::DecreaseReplicated, 2.0f, false);
		}
	}
}

// Called every frame
void AMyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyBox, RepValue);
}

