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
	CharArray = FillCharArray();
	SetupTurn();

}

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

	if (ResultArray.Num() <= 0)  GameOverFunction();
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
		if (EndOfTurn()) {
			CharArray = FillCharArray();
			SetupTurn();
		}
		else {
			GameOverFunction();
		}
	}

	HandleUnitPossess(CharArray[IndexPlayingUnit]);
}




void AXCGameStateBase::GameOverFunction()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Game Is Done"));
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit , false);
}

bool AXCGameStateBase::HandleUnitPossess(ABaseCharacters* C)
{
	if (!C->IsAlive()) return false ;

	if (C->bIsAlly) {
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController(); //single player only one ;

		if (PlayerController != nullptr)
		{
			PlayerController->Possess(C);
			return true;
		}
		else return false;
	}
	else {

		UBlackboardComponent* BBComp = Cast<AAIController>(C->GetController())->GetBlackboardComponent();
		if (BBComp) {
			BBComp->SetValueAsBool(FName("AIsTurn"), true);
		}
		else {

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("AI Turn")); //Temporary while AI doesn't exist
			NextChar();
		}
		
		return true;
	}
}
