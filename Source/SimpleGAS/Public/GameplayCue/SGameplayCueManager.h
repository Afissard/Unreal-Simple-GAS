// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SGameplayCueSettings.h"
#include "Subsystems/WorldSubsystem.h"
#include "SGameplayCueManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGAS_GameplayCueManager, Log, All);

UCLASS()
class SIMPLEGAS_API USGameplayCueManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	USGameplayCueRegistry* Registry;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void BroadcastGameplayCue(FGameplayTag CueTag, const FVector& Location, AActor* Instigator);
};
