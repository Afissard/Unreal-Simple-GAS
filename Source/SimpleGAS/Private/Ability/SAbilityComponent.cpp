// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/SAbilityComponent.h"
#include "Ability/SAbility.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogGAS_AbilityComponent);

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
		UE_LOG(LogGAS_AbilityComponent, Warning, TEXT("Attempted to add null ability"));
		return;
	}
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Adding Ability %s"), *Ability->GetName());

	// If the ability tag already exists, replace it
	FGameplayTag Tag = Ability->AbilityTag;

	// Optionally: check OwnedTags for compatibility. If the ability has tags that conflict, don't add.
	// For now, we'll assume AbilityTag should not be blocked by OwnedTags - user can control this externally.

	TObjectPtr<USAbility>* Found = Abilities.Find(Tag);
	if (Found && *Found)
	{
		UE_LOG(LogGAS_AbilityComponent, Warning, TEXT("Ability with tag %s already exists. Replacing."), *Tag.ToString());
		// Replace
		Abilities[Tag] = Ability;
		OnAbilityAdded.Broadcast(Tag, Ability);
		UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Added Ability %s"), *Ability->GetName());
		return;
	}

	Abilities.Add(Tag, Ability);
	OnAbilityAdded.Broadcast(Tag, Ability);
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Added Ability %s"), *Ability->GetName());
}

void USAbilityComponent::RemoveAbility(FGameplayTag AbilityTag)
{
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Removing Ability with tag %s"), *AbilityTag.ToString());
	
	TObjectPtr<USAbility>* Found = Abilities.Find(AbilityTag);
	if (!Found || !*Found)
	{
		UE_LOG(LogGAS_AbilityComponent, Warning, TEXT("Attempted to remove non-existent ability with tag %s"), *AbilityTag.ToString());
		return;
	}

	USAbility* Ability = *Found;
	Abilities.Remove(AbilityTag);
	OnAbilityRemoved.Broadcast(AbilityTag, Ability);
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Removed Ability %s"), *Ability->GetName());
}

bool USAbilityComponent::TryTriggerAbility(FGameplayTag AbilityTag)
{
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Trying to trigger Ability with tag %s"), *AbilityTag.ToString());
	
	TObjectPtr<USAbility>* Found = Abilities.Find(AbilityTag);
	if (!Found || !*Found)
	{
		UE_LOG(LogGAS_AbilityComponent, Warning, TEXT("Attempted to trigger non-existent ability with tag %s"), *AbilityTag.ToString());
		return false;
	}
	USAbility* Ability = *Found;
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogGAS_AbilityComponent, Warning, TEXT("Ability Component has no owner. Cannot trigger ability with tag %s"), *AbilityTag.ToString());
		return false;
	}

	if (!Ability->BCanActivate(Owner))
	{
		UE_LOG(LogGAS_AbilityComponent, Warning, TEXT("Ability %s cannot be activated. Check BCanActivate conditions."), *Ability->GetName());
		return false;
	}
	
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Triggering Ability %s"), *Ability->GetName());
	OnAbilityExecuted.Broadcast(AbilityTag, Ability);
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Executed Ability %s"), *Ability->GetName());
	Ability->CommitExecute(Owner);
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Committed Ability %s"), *Ability->GetName());
	OnAbilityCompleted.Broadcast(AbilityTag, Ability);
	UE_LOG(LogGAS_AbilityComponent, Log, TEXT("Completed Ability %s"), *Ability->GetName());
	return true;
}
