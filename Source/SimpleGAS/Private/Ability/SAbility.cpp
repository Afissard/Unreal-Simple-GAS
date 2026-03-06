// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/SAbility.h"
#include "Attribute/SAttributeComponent.h"
#include "GameplayCue/SGameplayCueManager.h"

bool USAbility::BCanActivate(AActor* Owner) const
{
	if (!Owner)
	{		
		return false;
	}
	
	if (USAttributeComponent* AttributeComp = Owner->FindComponentByClass<USAttributeComponent>())
	{
		if (CooldownTag.IsValid())
		{
			float CooldownRemaining = AttributeComp->GetAttributeValue(CooldownTag);
			if (CooldownRemaining > 0.f)
			{
				return false; // Still on cooldown
			}
		}
		
		for (const auto& Cost : AttributeCosts)
		{
			FGameplayTag AttributeTag = Cost.Key;
			float RequiredValue = Cost.Value;
		
			float CurrentValue = AttributeComp->GetAttributeValue(AttributeTag);

			if (CurrentValue < RequiredValue)
			{
				return false;
			}
		}
		return true;
	}
	
	return false;
}

void USAbility::CommitExecute(AActor* Owner)
{
	if (!Owner)
	{
		return;
	}
	
	if (USAttributeComponent* AttributeComp = Owner->FindComponentByClass<USAttributeComponent>())
	{
		for (const auto& Cost : AttributeCosts)
		{
			FGameplayTag AttributeTag = Cost.Key;
			float CostValue = Cost.Value;
		
			AttributeComp->ApplyAttributeChange(AttributeTag, -CostValue);
		}
		
		// Apply cooldown
		AttributeComp->ApplyAttributeChange(CooldownTag, CooldownDuration);
		FTimerHandle CooldownTimer;
		FGameplayTag LocalCooldownTag = CooldownTag;
		float LocalCooldownDuration = CooldownDuration;
		TWeakObjectPtr<USAttributeComponent> WeakAttributeComp = AttributeComp;
		FTimerDelegate CooldownDelegate = FTimerDelegate::CreateLambda([WeakAttributeComp, LocalCooldownTag, LocalCooldownDuration]()
		{
			if (USAttributeComponent* AttrComp = WeakAttributeComp.Get())
			{
				AttrComp->ApplyAttributeChange(LocalCooldownTag, -LocalCooldownDuration);
			}
		});
		Owner->GetWorldTimerManager().SetTimer(CooldownTimer, CooldownDelegate, LocalCooldownDuration, false);
		
		// Call GameplayCueManager to play ActivationCueTag
		if (UWorld* World = Owner->GetWorld())
		{
			if (USGameplayCueManager* CueManager = World->GetSubsystem<USGameplayCueManager>())
			{
				CueManager->BroadcastGameplayCue(AbilityTag, Owner->GetActorLocation(), Owner);
			}
		}
	}
}
