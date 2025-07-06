#pragma once
#include "CoreMinimal.h"
#include "Tickable.h"
#include "BaseClasses/ScriptGameInstanceSubsystem.h"
#include "ScriptGameInstanceSubsystemEx.generated.h"

UCLASS(Blueprintable, Abstract)
class UNREALMULTIPLAYER_API UScriptGameInstanceSubsystemEx : public UScriptGameInstanceSubsystem
{
	GENERATED_BODY()

	FSubsystemCollectionBase* _Collection;
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		_Collection = &Collection;
		Super::Initialize(Collection);
	}

	UFUNCTION(ScriptCallable)
	USubsystem* InitializeDependency(TSubclassOf<USubsystem> SubsystemClass) {
		return _Collection->InitializeDependency(SubsystemClass);
	}
};
