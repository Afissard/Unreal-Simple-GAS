// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Ability/SAbility.h"
#include "SAbilityComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGAS_AbilityComponent, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityAdded, FGameplayTag, AbilityTag, USAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityRemoved, FGameplayTag, AbilityTag, USAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityExecuted, FGameplayTag, AbilityTag, USAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCompleted, FGameplayTag, AbilityTag, USAbility*, Ability);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLEGAS_API USAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	USAbilityComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FGameplayTagContainer OwnedTags;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USAbility>> Abilities;
	
	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityAdded OnAbilityAdded;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityRemoved OnAbilityRemoved;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityExecuted OnAbilityExecuted;

	UPROPERTY(BlueprintAssignable, Category = "Ability")
	FOnAbilityCompleted OnAbilityCompleted;

protected:
	
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void AddAbility(USAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveAbility(FGameplayTag AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryTriggerAbility(FGameplayTag AbilityTag);
	
};
