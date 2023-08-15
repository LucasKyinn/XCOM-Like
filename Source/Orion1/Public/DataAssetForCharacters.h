
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SpellDataAsset.h"
#include "Engine/Texture2D.h"

#include "DataAssetForCharacters.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ORION1_API UDataAssetForCharacters : public UDataAsset
{
	GENERATED_BODY()

public:

	///					HEALTH					///

	UPROPERTY(BlueprintReadWrite , EditAnywhere, meta = (ClampMin = "0", ClampMax = "200"))
		int MaxHealth = 100;

	///					ARMOR					///

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "1", ClampMax = "5"))
	int MaxArmorStack = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "0.9"))
	float ArmorReducedDamage = 0.5f;


	///					ENERGY					///

	// Les sorts vont couter genre 5/6
	UPROPERTY(BlueprintReadWrite , EditAnywhere  , meta = (ClampMin = "1", ClampMax = "10"))
		int MaxEnergy = 0;

	//Regen Par Tour plus pour le perso supp 
	UPROPERTY(BlueprintReadWrite , EditAnywhere, meta = (ClampMin = "1", ClampMax = "10"))
		int RegenEnergy = 2;

	///				COMBAT STATS				///


	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "10"))
		int Speed=5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stats")
		float Precision=250.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1500"))
		float MaxWalkableDistance=1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0", ClampMax = "200"))
		int BaseDamage=25;

	///				MISCELLAENEOUS				///

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool Team=0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skills")
	USpellDataAsset* ShootDataAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText UnitName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Image")
	UTexture2D* UnitIcone;

};
