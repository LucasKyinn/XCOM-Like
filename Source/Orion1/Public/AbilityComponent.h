// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"


UCLASS(Blueprintable)
class ORION1_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	///					Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataAsset")
	class USpellDataAsset* AbilityDataSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	int EnergyCost;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float Range;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanHitSelf;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanHitAllies;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bCanHitEnnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FString AbilityDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	FString AbilityName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables")
	UParticleSystem* AbilityEffect; //Animation du sort

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variables")
	class USoundCue* AbilitySound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	///					Other
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	class ABaseCharacters* Target;

public:	

	ABaseCharacters* Owner;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool IsTargetValid();

	UFUNCTION(BlueprintCallable)
	virtual void UseAbility();

	virtual void PlayAbilityEffect(FVector TargetSpawn );

};
