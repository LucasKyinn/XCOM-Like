// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpellDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ORION1_API USpellDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats", meta = (ClampMin = "1.0", ClampMax = "100.0"))
		float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats", meta = (ClampMin = "1", ClampMax = "10"))
		int EnergyCost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		bool bCanHitSelf;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		bool bCanHitAllies;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		bool bCanHitEnnemies;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		FString AbilityDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		FString AbilityName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variables")
	UParticleSystem* AbilityEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variables")
	class USoundCue* AbilitySound;
};
