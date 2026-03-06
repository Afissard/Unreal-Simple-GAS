// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAttributeData.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangedDelegate, float, NewValue, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEGAS_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public: 	
	// Sets default values for this component's properties
	USAttributeComponent();

	// Use the lightweight struct for attribute storage
	UPROPERTY()
	TMap<FGameplayTag, FSAttributeData> AttributeDataMap;

	// Map of delegates per attribute tag. Created on demand via FindOrAdd.
	UPROPERTY()
	TMap<FGameplayTag, FAttributeChangedDelegate> AttributeChangedDelegates;

public:
	
	float GetAttributeValue(FGameplayTag AttributeTag);
	
	void SetAttributeValue(FGameplayTag AttributeTag, float AttributeValue);
	
	void ApplyAttributeChange(FGameplayTag AttributeTag, float DeltaValue);

	FAttributeChangedDelegate& RegisterAttributeChange(FGameplayTag AttributeTag);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void AddAttributeChangeListener(FGameplayTag AttributeTag, UObject* ListenerObject, FName FunctionName);
	
protected:
	virtual void BeginPlay() override;

};
