// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/SAbilityComponent.h"
#include "Ability/SAbility.h"
#include "GameFramework/Actor.h"

USAbilityComponent::USAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USAbilityComponent::AddAbility(USAbility* Ability)
{
	if (!Ability)
	{
		return;
	}

	// If the ability tag already exists, replace it
	FGameplayTag Tag = Ability->AbilityTag;

	// Optionally: check OwnedTags for compatibility. If the ability has tags that conflict, don't add.
	// For now, we'll assume AbilityTag should not be blocked by OwnedTags - user can control this externally.

	TObjectPtr<USAbility>* Found = Abilities.Find(Tag);
	if (Found && *Found)
	{
		// Replace
		Abilities[Tag] = Ability;
		OnAbilityAdded.Broadcast(Tag, Ability);
		return;
	}

	Abilities.Add(Tag, Ability);
	OnAbilityAdded.Broadcast(Tag, Ability);
}

void USAbilityComponent::RemoveAbility(FGameplayTag AbilityTag)
{
	TObjectPtr<USAbility>* Found = Abilities.Find(AbilityTag);
	if (!Found || !*Found)
	{
		return;
	}

	USAbility* Ability = *Found;
	Abilities.Remove(AbilityTag);
	OnAbilityRemoved.Broadcast(AbilityTag, Ability);
}

bool USAbilityComponent::TryTriggerAbility(FGameplayTag AbilityTag)
{
	TObjectPtr<USAbility>* Found = Abilities.Find(AbilityTag);
	if (!Found || !*Found)
	{
		return false;
	}

	USAbility* Ability = *Found;
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}

	if (!Ability->BCanActivate(Owner))
	{
		return false;
	}

	OnAbilityExecuted.Broadcast(AbilityTag, Ability);

	Ability->CommitExecute(Owner);

	OnAbilityCompleted.Broadcast(AbilityTag, Ability);

	return true;
}
