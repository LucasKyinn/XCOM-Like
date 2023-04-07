// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacters.generated.h"

//Base class for all characters in the game, allies and ennemies
UCLASS()
class ORION1_API ABaseCharacters : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacters();

	///					HEALTH					///

	//Max Health for the character, also starting health , Health cannot go beyond this amount
	UPROPERTY(BlueprintReadWrite, Category = "Stats" , meta= (ClampMin = "0" , ClampMax = "200"))
	int MaxHealth;

	//Character Current Health
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", ClampMax = "200"))
	int CurrentHealth;

	///					ARMOR					///


	//Maximum of Armor Stack you can have
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "1", ClampMax = "5"))
	int MaxArmorStack;

	//Current Stack of armor, cannot exede MaxArmorStack
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", ClampMax = "5"))
	int CurrentArmorStack;

	// Percentage of damage that armor can reduce
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "0.9"))
	float ArmorReducedDamage;


	///					ENERGY					///

	//Max Energy for the character, also starting Energy , Energy cannot go beyond this amount
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "1", ClampMax = "10"))
	int MaxEnergy;

	//Character Current Energy
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "10"))
	int CurrentEnergy;

	//Amount of Energy recoverde every turn
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "1", ClampMax = "10"))
	int RegendEnergy;

	///				COMBAT STATS				///

	// The faster a character is the sooner he gets to play in the turn, if tie random
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0", ClampMax = "10"))
	int Speed;

	//Unit ability to hit at range NEED TEST TO FIGURE HOW IT WILL WORK
	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float Precision;

	// Precision Buff in case we missed last shot (dopamine hack) +65% buff
	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	bool BuffPrecision; 

	// Maximum distance walkable everyturn (need test)
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "1500"))
	float MaxWalkableDistance;

	//Walked Distance This Turn 
	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float WalkedDistance = 0.f;

	//Weapon Base Damage
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "30", ClampMax = "200"))
	int BaseDamage;

	///				MISCELLAENEOUS				///
	// Ally team or Ennemy (bool cause only two teams)
	UPROPERTY(BlueprintReadWrite,  Category = "Stats")
	bool Team; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost")
	TSubclassOf < AActor> GhostClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost")
	class UMaterialInstance * GhostMaterialClass;

	UPROPERTY(BlueprintReadOnly , Category = "Stuff")
	FVector VectDestination; 

	void(ABaseCharacters::*ActionToExexcute)();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Stats")
	class UDataAssetForCharacters* UnitDataAsset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Actions Functions 
	void ConfirmedExecution();
	void CanceledExecution();
	void MoveToVectorLocation();
};
