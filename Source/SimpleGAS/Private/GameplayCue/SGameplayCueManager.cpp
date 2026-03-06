// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayCue/SGameplayCueManager.h"
#include "GameplayTagContainer.h"


void USGameplayCueManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const USGameplayCueSettings* Settings = GetDefault<USGameplayCueSettings>(); Settings->CueRegistry.IsNull() == false)
	{
		Registry = Settings->CueRegistry.LoadSynchronous();
	}
}

void USGameplayCueManager::Deinitialize()
{
	Super::Deinitialize();

	Registry = nullptr;
}

void USGameplayCueManager::BroadcastGameplayCue(FGameplayTag CueTag, const FVector& Location, AActor* Instigator)
{
	if (!Registry)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayCueManager: Registry not loaded"));
		return;
	}

	const TSubclassOf<AActor>* CueClass = Registry->CueActorMap.Find(CueTag);

	if (!CueClass || !(*CueClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayCueManager: No cue registered for tag %s"),*CueTag.ToString());
		return;
	}

	UWorld* World = GetWorld();
	if (!World)return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Instigator ? Instigator->GetInstigator() : nullptr;
	SpawnParams.Owner = Instigator;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	World->SpawnActor<AActor>(
		*CueClass,
		Location,
		FRotator::ZeroRotator,
		SpawnParams
	);
}