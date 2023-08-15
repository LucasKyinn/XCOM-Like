// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseCharacters.h"
#include "XCGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitTurnStart, ABaseCharacters*, CharPlaying);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurnEnd);

UENUM()
enum class EGamePhase : uint8
{
	InProgress,
	GameWon,
	GameLost
};

UCLASS()
class ORION1_API AXCGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

private:
	int NbTurn = 0 ;
	TArray<class ABaseCharacters*> CharArray;
	class ABaseCharacters* PlayingUnit;
	int IndexPlayingUnit=0;

public:
	bool bAnyAllyAlive = true ;
	TArray<class ABaseCharacters*> FillCharArray();

	bool AnyAllyAlive();

	bool AnyEnnemmiesAlive();

	void SetupTurn();

	void GameOverFunctionLose();

	void GameOverFunctionWin();


	void UnitEndTurn();

	bool HandleUnitPossess(class ABaseCharacters* C);

	virtual void HandleBeginPlay();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTurnEnd OnTurnEnd;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUnitTurnStart OnUnitTurnStart;

	UFUNCTION(BlueprintCallable)
	void NextChar();

	enum class EGamePhase : uint8
	{
		InProgress,
		GameWon,
		GameLost
	};

	UPROPERTY(BlueprintReadWrite, Category = "Turn Based")
	EGamePhase CurrentGamePhase; //TODO Use it 

	int TurnNumber=0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
