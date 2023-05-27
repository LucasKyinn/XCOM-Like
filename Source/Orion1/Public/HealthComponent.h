// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Delegates/DelegateCombinations.h>
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORION1_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BluePrintReadWrite, VisibleAnywhere, Category = "HealthComponent")
	bool bIsAlly;

	bool bIsDead;


protected :

	///					HEALTH					///

	//Max Health for the character, also starting health , Health cannot go beyond this amount
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = "Stats", meta = (ClampMin = "0", ClampMax = "200"))
	int MaxHealth;

	//Character Current Health
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "HealthComponent", meta = (ClampMin = "0", ClampMax = "200"))
	int CurrentHealth;


	///					ARMOR					///

	//Maximum of Armor Stack you can have
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stats", meta = (ClampMin = "1", ClampMax = "5"))
	int MaxArmorStack;

	//Current Stack of armor, cannot exede MaxArmorStack
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "HealthComponent", meta = (ClampMin = "0", ClampMax = "5"))
	int CurrentArmorStack;

	// Percentage of damage that armor can reduce
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "0.9"))
	float ArmorReducedDamage;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
	static bool IsFriendly(AActor* ActorA, AActor* ActorB);

	//HealthManagement//
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Heal(float HealAmount);

	//ArmorManagement
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "HealthComponent")
	int GetArmorStacks();

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void ArmorUP(int ArmorAmount);

	//DamageManagement
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
