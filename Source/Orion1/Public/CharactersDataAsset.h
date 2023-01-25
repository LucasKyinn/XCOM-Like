// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"


#include "CharactersDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ORION1_API UCharactersDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public: 

	UPROPERTY(BlueprintReadWrite)
		FGameplayTag MyGmplTag;

	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", ClampMax = "100"))
		int HealthPoints = 100;

	// Les sorts vont couter genre 5/6
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", ClampMax = "10"))
		int Energy = 0;

	//Regen Par Tour plus pour le perso supp 
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", ClampMax = "10"))
		int RegenEnergy = 2;

	//Armure = % de reduce damage si actif, ce casse après avoir pris un coup 
	//Le tank recup l'armure tt les tours 
	UPROPERTY(BlueprintReadWrite, Category = "Stats")
		bool Armor = false;

	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", ClampMax = "1"))
		float ArmorReducedDamage = 0.75f;

	
};
