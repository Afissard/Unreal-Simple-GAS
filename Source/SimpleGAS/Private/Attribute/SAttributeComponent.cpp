// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/SAttributeComponent.h"

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
		return AttributeDataMap[AttributeTag].CurrentValue;
	}
	return 0.f;
}

void USAttributeComponent::SetAttributeValue(const FGameplayTag AttributeTag, float AttributeValue)
{
	if (AttributeDataMap.Contains(AttributeTag))
	{
		float OldValue = AttributeDataMap[AttributeTag].CurrentValue;

		if (AttributeValue > AttributeDataMap[AttributeTag].MaxValue)
		{
			AttributeValue = AttributeDataMap[AttributeTag].MaxValue;
		} else if (AttributeValue < AttributeDataMap[AttributeTag].MinValue)
		{
			AttributeValue = AttributeDataMap[AttributeTag].MinValue;
		}
		
		AttributeDataMap[AttributeTag].CurrentValue = AttributeValue;

		float Delta = AttributeValue - OldValue;
		if (Delta != 0.f)
		{
			FAttributeChangedDelegate* Found = AttributeChangedDelegates.Find(AttributeTag);
			if (Found)
			{
				Found->Broadcast(AttributeValue, Delta);
			}
		}
	}
}

void USAttributeComponent::ApplyAttributeChange(const FGameplayTag AttributeTag, const float DeltaValue)
{
	if (AttributeDataMap.Contains(AttributeTag))
	{
		SetAttributeValue(AttributeTag, GetAttributeValue(AttributeTag) + DeltaValue);
	}
}

FAttributeChangedDelegate& USAttributeComponent::RegisterAttributeChange(FGameplayTag AttributeTag)
{
	return AttributeChangedDelegates.FindOrAdd(AttributeTag);
}

void USAttributeComponent::AddAttributeChangeListener(FGameplayTag AttributeTag, UObject* ListenerObject, FName FunctionName)
{
	if (!ListenerObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddAttributeChangeListener called with null ListenerObject"));
		return;
	}

	FAttributeChangedDelegate& Delegate = RegisterAttributeChange(AttributeTag);

	FScriptDelegate ScriptDel;
	ScriptDel.BindUFunction(ListenerObject, FunctionName);

	Delegate.Add(ScriptDel);
}
