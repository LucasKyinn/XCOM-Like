// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "XCGameStateBase.generated.h"

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

	bool EndOfTurn();

	void SetupTurn();

	void GameOverFunction();

	bool HandleUnitPossess(class ABaseCharacters* C);

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
