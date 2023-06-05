// Fill out your copyright notice in the Description page of Project Settings.


#include "XCGameStateBase.h"
#include "BaseCharacters.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


TArray<ABaseCharacters*> AXCGameStateBase::FillCharArray()
{
	TArray<ABaseCharacters*> ResultArray = TArray< ABaseCharacters*>();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacters::StaticClass(), FoundActors);

	for (AActor* A : FoundActors) {
		ABaseCharacters* Char = Cast<ABaseCharacters>(A);
		if (Char && Char->IsAlive()) ResultArray.Push(Char); // TODO also check if aggro ;
	}

	ResultArray.Sort([](const ABaseCharacters& A, const ABaseCharacters& B) { return A.Speed < B.Speed; });
	return ResultArray;
}

bool AXCGameStateBase::EndOfTurn()
{
	bAnyAllyAlive = false;
	for (ABaseCharacters* C : CharArray) {
		if (C->IsAlive()) C->WalkedDistance = C->MaxWalkableDistance; //TODO Reset Ablity to shoot
		if (bAnyAllyAlive == false && C->IsAlive() && C->bIsAlly) bAnyAllyAlive = true;
	}
	return bAnyAllyAlive;
}

void AXCGameStateBase::HandleTurn()
{
	for (ABaseCharacters* C : CharArray) {
		PlayingUnit = C;
		
		if (!PlayingUnit->IsAlive()) continue; 

		if (C->bIsAlly) { //Doner le controle du perso au joueur 
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Player Turn"));

			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController(); //single player only one ;

			if(PlayerController != nullptr ) PlayerController->Possess(C);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("AI Turn"));

		}

	}
}

void AXCGameStateBase::NextChar()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SwapCharPls"));

}
