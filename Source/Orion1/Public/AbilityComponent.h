// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	class USpellDataAsset* AbilityDataSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int EnergyCost;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float Range;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bCanHitSelf;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bCanHitAllies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bCanHitEnnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FString AbilityDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FString AbilityName;

	UPROPERTY(EditDefaultsOnly, Category = "Variables")
	UParticleSystem* AbilityEffect; //Animation du sort

	UPROPERTY(EditDefaultsOnly, Category = "Variables")
	class USoundCue* AbilitySound;

	///					Other
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	class ABaseCharacters* Target;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool IsTargetValid();

	UFUNCTION(BlueprintCallable)
	virtual void UseAbility();

};
