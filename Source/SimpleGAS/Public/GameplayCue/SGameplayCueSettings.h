// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SGameplayCueRegistry.h"
#include "Engine/DeveloperSettings.h"
#include "SGameplayCueSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Simple Gameplay Cue"))
class SIMPLEGAS_API USGameplayCueSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Config, EditAnywhere, Category="Gameplay Cues")
	TSoftObjectPtr<USGameplayCueRegistry> CueRegistry; // avoid loading asset at started
};
