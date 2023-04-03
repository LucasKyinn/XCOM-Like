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
	UPROPERTY( EditAnywhere , Category = "Stats" , meta= (ClampMin = "0" , ClampMax = "200"))
	int MaxHealth;

	//Character Current Health
	UPROPERTY( VisibleAnywhere, Category = "Stats", meta = (ClampMin = "0", ClampMax = "200"))
	int CurrentHealth;

	///					ARMOR					///


	//Maximum of Armor Stack you can have
	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "1", ClampMax = "5"))
	int MaxArmorStack;

	//Current Stack of armor, cannot exede MaxArmorStack
	UPROPERTY(VisibleAnywhere, Category = "Stats", meta = (ClampMin = "0", ClampMax = "5"))
	int CurrentArmorStack;

	// Percentage of damage that armor can reduce
	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "0.9"))
	float ArmorReducedDamage;


	///					ENERGY					///

	//Max Energy for the character, also starting Energy , Energy cannot go beyond this amount
	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "1", ClampMax = "10"))
	int MaxEnergy;

	//Character Current Energy
	UPROPERTY(VisibleAnywhere, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "10"))
	int CurrentEnergy;

	//Amount of Energy recoverde every turn
	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "1", ClampMax = "10"))
	int RegendEnergy;

	///				COMBAT STATS				///

	// The faster a character is the sooner he gets to play in the turn, if tie random
	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0", ClampMax = "10"))
	int Speed;

	//Unit ability to hit at range NEED TEST TO FIGURE HOW IT WILL WORK
	UPROPERTY(EditAnywhere, Category = "Stats")
	float Precision;

	// Precision Buff in case we missed last shot (dopamine hack) +65% buff
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	bool BuffPrecision; 

	// Maximum distance walkable everyturn (need test)
	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "1500"))
	float MaxWalkableDistance;

	//Walked Distance This Turn 
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float WalkedDistance = 0.f;

	//Weapon Base Damage
	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "30", ClampMax = "200"))
	int BaseDamage;

	///				MISCELLAENEOUS				///
	// Ally team or Ennemy (bool cause only two teams)
	UPROPERTY(EditAnywhere, Category = "Stats")
	bool Team; 



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
