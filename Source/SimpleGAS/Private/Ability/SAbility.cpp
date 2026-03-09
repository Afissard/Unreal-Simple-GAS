// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/SAbility.h"
#include "Attribute/SAttributeComponent.h"
#include "GameplayCue/SGameplayCueManager.h"

DEFINE_LOG_CATEGORY(LogGAS_Ability);

bool USAbility::BCanActivate(AActor* Owner) const
{
	UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate"));
	if (!Owner)
	{		
		UE_LOG(LogGAS_Ability, Error, TEXT("BCanActivate: Owner is NULL"));
		return false;
	}
	
	if (USAttributeComponent* AttributeComp = Owner->FindComponentByClass<USAttributeComponent>())
	{
		UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: Checking cooldown"));
		if (CooldownTag.IsValid())
		{
			UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: CooldownTag is valid, checking cooldown value"));
			float CooldownRemaining = AttributeComp->GetAttributeValue(CooldownTag);
			if (CooldownRemaining > 0.f)
			{
				UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: Still on cooldown (%.2f seconds remaining)"), CooldownRemaining);
				return false; // Still on cooldown
			}
			UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: Cooldown = 0, ability is ready"));
		}
		
		UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: Checking attribute costs"));
		for (const auto& Cost : AttributeCosts)
		{
			const FGameplayTag AttributeTag = Cost.Key;
			const float RequiredValue = Cost.Value;

			if (const float CurrentValue = AttributeComp->GetAttributeValue(AttributeTag); CurrentValue < RequiredValue)
			{
				UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: Not enough %s (%.2f required, %.2f available)"), *AttributeTag.ToString(), RequiredValue, CurrentValue);
				return false;
			}
			UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: %s cost met (%.2f required)"), *AttributeTag.ToString(), RequiredValue);
		}
		
		UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: All costs met and not on cooldown"));
		return true;
	}
	
	UE_LOG(LogGAS_Ability, Log, TEXT("BCanActivate: Owner has no AttributeComponent"));
	return false;
}

void USAbility::CommitExecute(AActor* Owner)
{
	UE_LOG(LogGAS_Ability, Log, TEXT("CommitExecute"));
	if (!Owner)
	{
		UE_LOG(LogGAS_Ability, Error, TEXT("CommitExecute: Owner is NULL"));
		return;
	}
	
	if (USAttributeComponent* AttributeComp = Owner->FindComponentByClass<USAttributeComponent>())
	{
		UE_LOG(LogGAS_Ability, Log, TEXT("CommitExecute: Applying attribute costs"));
		for (const auto& Cost : AttributeCosts)
		{
			FGameplayTag AttributeTag = Cost.Key;
			float CostValue = Cost.Value;
		
			AttributeComp->ApplyAttributeChange(AttributeTag, -CostValue);
		}
		
		PlayEffects(Owner);
		
		// Apply cooldown
		UE_LOG(LogGAS_Ability, Log, TEXT("CommitExecute: Applying cooldown"));
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
			} else
			{
				UE_LOG(LogGAS_Ability, Log, TEXT("CommitExecute: Applying cooldown"));
			}
		});
		Owner->GetWorldTimerManager().SetTimer(CooldownTimer, CooldownDelegate, LocalCooldownDuration, false);
		UE_LOG(LogGAS_Ability, Log, TEXT("CommitExecute: Cooldown applied for %.2f seconds"), LocalCooldownDuration);
		
		// Call GameplayCueManager to play ActivationCueTag
		UE_LOG(LogGAS_Ability, Log, TEXT("CommitExecute: Broadcasting gameplay cue for activation"));
		if (UWorld* World = Owner->GetWorld())
		{
			if (USGameplayCueManager* CueManager = World->GetSubsystem<USGameplayCueManager>())
			{
				UE_LOG(LogGAS_Ability, Log, TEXT("CommitExecute: Found GameplayCueManager, broadcasting cue"));
				CueManager->BroadcastGameplayCue(AbilityTag, Owner->GetActorLocation(), Owner);
			} else
			{
				UE_LOG(LogGAS_Ability, Error, TEXT("CommitExecute: World has no GameplayCueManager subsystem"));
			}
		} else
		{
			UE_LOG(LogGAS_Ability, Error, TEXT("CommitExecute: Owner has no valid world"));
		}
	} else
	{
		UE_LOG(LogGAS_Ability, Error, TEXT("CommitExecute: Owner has no AttributeComponent"));
	}
}

void USAbility::Initialize(const FGameplayTag InAbilityTag, const TMap<FGameplayTag, float> InAttributeCosts, const float InCoolDown)
{
	AbilityTag = InAbilityTag;
	AttributeCosts = InAttributeCosts;
	CooldownDuration = InCoolDown;
	
	UE_LOG(LogGAS_Ability, Log, TEXT("Initialized ability with Tag: %s"), *AbilityTag.ToString());
}