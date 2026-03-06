// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeData.generated.h"

/**
 * Simple POD-style attribute data that can be stored in containers.
 */
USTRUCT(BlueprintType)
struct SIMPLEGAS_API FSAttributeData
{
	GENERATED_BODY()

	FSAttributeData(): BaseValue(0.f), CurrentValue(0.f), MaxValue(0.f), MinValue(0.f)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue;
};
