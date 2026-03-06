// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SGameplayCueRegistry.generated.h"


UCLASS()
class SIMPLEGAS_API USGameplayCueRegistry : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Cues")
	TMap<FGameplayTag, TSubclassOf<AActor>> CueActorMap;
};
