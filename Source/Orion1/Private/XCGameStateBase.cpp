// Fill out your copyright notice in the Description page of Project Settings.


#include "XCGameStateBase.h"
#include "BaseCharacters.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AXCGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentGamePhase = EGamePhase::InProgress;
	CharArray = FillCharArray();
}

TArray<ABaseCharacters*> AXCGameStateBase::FillCharArray()
{
	//TurnNumber++;
	NbTurn++;

	TArray<ABaseCharacters*> ResultArray = TArray< ABaseCharacters*>();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacters::StaticClass(), FoundActors);
	for (AActor* A : FoundActors) {
		ABaseCharacters* Char = Cast<ABaseCharacters>(A);
		if (Char && Char->IsAlive()) ResultArray.Push(Char); // TODO also check if aggro ;
	}
	ResultArray.Sort([](const ABaseCharacters& A, const ABaseCharacters& B) { return A.Speed > B.Speed; });

	if (ResultArray.Num() <= 0)  GameOverFunctionLose();
	return ResultArray;
}

bool AXCGameStateBase::AnyAllyAlive()
{
	bAnyAllyAlive = false;
	for (ABaseCharacters* C : CharArray) {
		if (C) {
			if (C->IsAlive()) C->WalkedDistance = C->MaxWalkableDistance; //TODO Reset Ablity to shoot
			if (bAnyAllyAlive == false && C->IsAlive() && C->bIsAlly) bAnyAllyAlive = true;
		}
	}
	return bAnyAllyAlive;
}

bool AXCGameStateBase::AnyEnnemmiesAlive()
{
	for (ABaseCharacters* C : CharArray) {
		if (C && C->IsAlive() && !(C->bIsAlly)) return true;
	}
	return false;
}

void AXCGameStateBase::SetupTurn()
{
	IndexPlayingUnit = -1;
	for (ABaseCharacters* C : CharArray) {
		IndexPlayingUnit++;
		if( HandleUnitPossess(C)) return;
	}
}

void AXCGameStateBase::NextChar()
{
	IndexPlayingUnit++;
	if (IndexPlayingUnit >= CharArray.Num()) { //Enveryone played
		if (AnyAllyAlive()) {
			CharArray = FillCharArray();
			SetupTurn();
		}
		else {
			GameOverFunctionLose();
			return;
		}
	}

	HandleUnitPossess(CharArray[IndexPlayingUnit]);
}




void AXCGameStateBase::GameOverFunctionLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Game Is Done"));
	CurrentGamePhase = EGamePhase::GameLost;
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit , false);
}

void AXCGameStateBase::GameOverFunctionWin()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Game Is Done"));
	CurrentGamePhase = EGamePhase::GameWon;
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void AXCGameStateBase::UnitEndTurn()
{
	OnTurnEnd.Broadcast();

	if (!AnyEnnemmiesAlive()) {
		GameOverFunctionWin(); //&& plus de spawner TODO
		return;
	}

	//Depossess ; TODO

	NextChar();
}

bool AXCGameStateBase::HandleUnitPossess(ABaseCharacters* C)
{
	if (!C->IsAlive()) return false ;

	if (C->bIsAlly) {
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController(); //single player only one ;

		if (PlayerController)
		{
			PlayerController->Possess(C);
			OnUnitTurnStart.Broadcast(C);
			return true;
		}
		else return false;
	}
	else {
		//TEMP
		UBlackboardComponent* BBComp = Cast<AAIController>(C->GetController())->GetBlackboardComponent();
		if (BBComp) {
			BBComp->SetValueAsBool(FName("AIsTurn"), true);
			OnUnitTurnStart.Broadcast(C);
		}		
		return true;
	}
}

void AXCGameStateBase::HandleBeginPlay()
{
	Super::HandleBeginPlay();
	if (!CharArray.IsEmpty())
		SetupTurn();
	else GameOverFunctionLose();

}
