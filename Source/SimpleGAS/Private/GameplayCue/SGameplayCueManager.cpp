// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/SGameplayCueManager.h"
#include "GameplayTagContainer.h"

DEFINE_LOG_CATEGORY(LogGAS_GameplayCueManager);

void USGameplayCueManager::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogGAS_GameplayCueManager, Log, TEXT("Initialize: Starting initialization of GameplayCueManager"));
	Super::Initialize(Collection);

	if (const USGameplayCueSettings* Settings = GetDefault<USGameplayCueSettings>(); Settings->CueRegistry.IsNull() == false)
	{
		UE_LOG(LogGAS_GameplayCueManager, Log, TEXT("Initialize: Loading CueRegistry from settings"));
		Registry = Settings->CueRegistry.LoadSynchronous();
	} else 
	{
		UE_LOG(LogGAS_GameplayCueManager, Warning, TEXT("Initialize: No CueRegistry set in settings!"));
	}
}

void USGameplayCueManager::Deinitialize()
{
	UE_LOG(LogGAS_GameplayCueManager, Log, TEXT("Deinitialize: Cleaning up GameplayCueManager"));
	Super::Deinitialize();
	Registry = nullptr;
}

void USGameplayCueManager::BroadcastGameplayCue(FGameplayTag CueTag, const FVector& Location, AActor* Instigator)
{
	UE_LOG(LogGAS_GameplayCueManager, Log, TEXT("BroadcastGameplayCue: Broadcasting cue %s at location %s with instigator %s"), *CueTag.ToString(), *Location.ToString(), Instigator ? *Instigator->GetName() : TEXT("None"));
	if (!Registry)
	{
		UE_LOG(LogGAS_GameplayCueManager, Warning, TEXT("GameplayCueManager: Registry not loaded"));
		return;
	} else
	{
		UE_LOG(LogGAS_GameplayCueManager, Verbose, TEXT("BroadcastGameplayCue: Registry loaded successfully"));
	}

	const TSubclassOf<AActor>* CueClass = Registry->CueActorMap.Find(CueTag);
	if (!CueClass || !(*CueClass))
	{
		UE_LOG(LogGAS_GameplayCueManager, Warning, TEXT("GameplayCueManager: No cue registered for tag %s"),*CueTag.ToString());
		return;
	} else
	{
		UE_LOG(LogGAS_GameplayCueManager, Verbose, TEXT("BroadcastGameplayCue: Found cue class %s for tag %s"), *(*CueClass)->GetName(), *CueTag.ToString());
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogGAS_GameplayCueManager, Warning, TEXT("GameplayCueManager: Unable to get world context"));
		return;
	}
	
	UE_LOG(LogGAS_GameplayCueManager, Log, TEXT("BroadcastGameplayCue: Spawning cue actor %s at location %s"), *(*CueClass)->GetName(), *Location.ToString());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Instigator ? Instigator->GetInstigator() : nullptr;
	SpawnParams.Owner = Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* ActorSpawned = World->SpawnActor<AActor>(
		*CueClass,
		Location,
		FRotator::ZeroRotator,
		SpawnParams
	);
	if (ActorSpawned)
	{
		UE_LOG(LogGAS_GameplayCueManager, Log, TEXT("BroadcastGameplayCue: Successfully spawned cue actor %s"), *ActorSpawned->GetName());
	} else
	{
		UE_LOG(LogGAS_GameplayCueManager, Warning, TEXT("BroadcastGameplayCue: Failed to spawn cue actor for tag %s"), *CueTag.ToString());
	}
}