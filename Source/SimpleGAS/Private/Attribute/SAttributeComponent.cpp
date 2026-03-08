// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/SAttributeComponent.h"

DEFINE_LOG_CATEGORY(LogGAS_AttributeComponent);

USAttributeComponent::USAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

float USAttributeComponent::GetAttributeValue(const FGameplayTag AttributeTag)
{
	if (AttributeDataMap.Contains(AttributeTag))
	{
		UE_LOG(LogGAS_AttributeComponent, Verbose, TEXT("GetAttributeValue: %s = %f"), *AttributeTag.ToString(), AttributeDataMap[AttributeTag].CurrentValue);
		return AttributeDataMap[AttributeTag].CurrentValue;
	}
	UE_LOG(LogGAS_AttributeComponent, Warning, TEXT("GetAttributeValue: AttributeTag %s not found! Returning 0."), *AttributeTag.ToString());
	return 0.f;
}

void USAttributeComponent::SetAttributeValue(const FGameplayTag AttributeTag, float AttributeValue)
{
	if (AttributeDataMap.Contains(AttributeTag))
	{
		float OldValue = AttributeDataMap[AttributeTag].CurrentValue;

		if (AttributeValue > AttributeDataMap[AttributeTag].MaxValue)
		{
			UE_LOG(LogGAS_AttributeComponent, Warning, TEXT("SetAttributeValue: AttributeTag %s value %f exceeds MaxValue %f. Clamping to MaxValue."), *AttributeTag.ToString(), AttributeValue, AttributeDataMap[AttributeTag].MaxValue);
			AttributeValue = AttributeDataMap[AttributeTag].MaxValue;
		} else if (AttributeValue < AttributeDataMap[AttributeTag].MinValue)
		{
			UE_LOG(LogGAS_AttributeComponent, Warning, TEXT("SetAttributeValue: AttributeTag %s value %f below MinValue %f. Clamping to MinValue."), *AttributeTag.ToString(), AttributeValue, AttributeDataMap[AttributeTag].MinValue);
			AttributeValue = AttributeDataMap[AttributeTag].MinValue;
		}
		
		UE_LOG(LogGAS_AttributeComponent, Verbose, TEXT("SetAttributeValue: Setting %s from %f to %f"), *AttributeTag.ToString(), OldValue, AttributeValue);
		AttributeDataMap[AttributeTag].CurrentValue = AttributeValue;

		if (const float Delta = AttributeValue - OldValue; Delta != 0.f)
		{
			if (const FAttributeChangedDelegate* Found = AttributeChangedDelegates.Find(AttributeTag))
			{
				UE_LOG(LogGAS_AttributeComponent, Verbose, TEXT("SetAttributeValue: Broadcasting change for %s, new value: %f, delta: %f"), *AttributeTag.ToString(), AttributeValue, Delta);
				Found->Broadcast(AttributeValue, Delta);
			} else
			{
				UE_LOG(LogGAS_AttributeComponent, Verbose, TEXT("SetAttributeValue: No delegate found for %s, not broadcasting."), *AttributeTag.ToString());
			}
		} else
		{
			UE_LOG(LogGAS_AttributeComponent, Verbose, TEXT("SetAttributeValue: No change in value for %s, not broadcasting delegate."), *AttributeTag.ToString());
		}
	} else
	{
		UE_LOG(LogGAS_AttributeComponent, Warning, TEXT("SetAttributeValue: AttributeTag %s not found!"), *AttributeTag.ToString());
	}
}

void USAttributeComponent::ApplyAttributeChange(const FGameplayTag AttributeTag, const float DeltaValue)
{
	if (AttributeDataMap.Contains(AttributeTag))
	{
		UE_LOG(LogGAS_AttributeComponent, Verbose, TEXT("ApplyAttributeChange: Applying delta %f to %s"), DeltaValue, *AttributeTag.ToString());
		SetAttributeValue(AttributeTag, GetAttributeValue(AttributeTag) + DeltaValue);
	} else 
	{
		UE_LOG(LogGAS_AttributeComponent, Warning, TEXT("ApplyAttributeChange: AttributeTag %s not found!"), *AttributeTag.ToString());
	}
}

FAttributeChangedDelegate& USAttributeComponent::RegisterAttributeChange(FGameplayTag AttributeTag)
{
	UE_LOG(LogGAS_AttributeComponent, Verbose, TEXT("RegisterAttributeChange: Registering delegate for %s"), *AttributeTag.ToString());
	return AttributeChangedDelegates.FindOrAdd(AttributeTag);
}

void USAttributeComponent::AddAttributeChangeListener(FGameplayTag AttributeTag, UObject* ListenerObject, FName FunctionName)
{
	if (!ListenerObject)
	{
		UE_LOG(LogGAS_AttributeComponent, Warning, TEXT("AddAttributeChangeListener called with null ListenerObject"));
		return;
	}
	
	UE_LOG(LogGAS_AttributeComponent, Log, TEXT("AddAttributeChangeListener: Adding listener for %s on object %s, function %s"), *AttributeTag.ToString(), *ListenerObject->GetName(), *FunctionName.ToString());
	FAttributeChangedDelegate& Delegate = RegisterAttributeChange(AttributeTag);

	FScriptDelegate ScriptDel;
	ScriptDel.BindUFunction(ListenerObject, FunctionName);

	Delegate.Add(ScriptDel);
	UE_LOG(LogGAS_AttributeComponent, Log, TEXT("AddAttributeChangeListener: Successfully added listener for %s on object %s, function %s"), *AttributeTag.ToString(), *ListenerObject->GetName(), *FunctionName.ToString());
}
