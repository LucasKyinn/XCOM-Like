// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "ShootComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORION1_API UShootComponent : public UAbilityComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		UParticleSystem* TracerEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		FName TracerTargetName;

public:
	virtual void UseAbility() override;

	virtual void PlayAbilityEffect(FVector TargetSpawn) override;

};
