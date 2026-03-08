// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SAbility.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGAS_Ability, Log, All);

UCLASS()
class SIMPLEGAS_API USAbility : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	FGameplayTag AbilityTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	FGameplayTag ActivationCueTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TMap<FGameplayTag, float> AttributeCosts;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	float CooldownDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	FGameplayTag CooldownTag;

public:
	bool BCanActivate(AActor* Owner) const;
	
	virtual void CommitExecute(AActor* Owner); // can be overridden for more complex logic, but by default it just applies attribute costs
};
