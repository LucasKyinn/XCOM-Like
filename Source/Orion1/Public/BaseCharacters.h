// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacters.generated.h"

class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetChanged, AActor*, Target);

//Base class for all characters in the game, allies and ennemies
UCLASS()
class ORION1_API ABaseCharacters : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacters();

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

	//Weapon Range TODO ADD IT IN THE THING
	UPROPERTY(BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "30", ClampMax = "3000"))
	float Range = 2000.f;

	///				MISCELLAENEOUS				///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsAlly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost")
	TSubclassOf <ACharacter> GhostClass;

	ACharacter* MovementGhost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ghost")
	class UMaterialInstance * GhostMaterialClass;

	UPROPERTY(BlueprintReadOnly , Category = "Stuff")
	FVector VectDestination; 

	TArray<AActor*> ActorInRange;



	FVector WhereToShoot;

	void(ABaseCharacters::*ActionToExexcute)();

	static FName ShootComponentClassName;


	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilityShoot> ShootComponenttest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAbilityShoot> ShootBPClass ;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTargetChanged TargetChanged;



	void SetTarget(AActor* NewTarget);

	AActor* GetTarget() { return Target; };
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category = "Stats")
	class UDataAssetForCharacters* UnitDataAsset;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
	AActor* Target;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION ?
	int ChanceToHit();


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//Actions Functions 
	void ConfirmedExecution();
	void CanceledExecution();
	void MoveToVectorLocation();
	void PewPewExecution();
	void NextTarget();
	void PreviousTarget();
	bool IsAlive();

};
