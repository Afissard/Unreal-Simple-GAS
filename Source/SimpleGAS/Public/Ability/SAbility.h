// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "SAbility.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGAS_Ability, Log, All);

UCLASS(Blueprintable, EditInlineNew)
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
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool BCanActivate(AActor* Owner) const;
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void CommitExecute(AActor* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void Initialize(FGameplayTag InAbilityTag, TMap<FGameplayTag, float> InAttributeCosts, float InCoolDown);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual void PlayEffects(AActor* Owner)
	{
		// Nothing by default, but can be overridden to play particle/sound effects or other custom logic when the ability is executed
	}
};
