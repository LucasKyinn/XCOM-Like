// Fill out your copyright notice in the Description page of Project Settings.


#include "XCOM_Player_Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacters.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "XCGameStateBase.h"


AXCOM_Player_Controller::AXCOM_Player_Controller()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

}

void AXCOM_Player_Controller::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	if (AllyClass) {
		UWorld* World = GetWorld();
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(World, AllyClass, OutActors);
		for (AActor* Actor : OutActors) {
			AllyArray.Add(Cast<ACharacter>(Actor));
		}
		Possess(AllyArray[0]);
	}
}

void AXCOM_Player_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXCOM_Player_Controller::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Started, this, &AXCOM_Player_Controller::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this, &AXCOM_Player_Controller::OnSetDestinationReleased);

		// Actions Controls 
		EnhancedInputComponent->BindAction(Cancel, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::CancelAction);
		EnhancedInputComponent->BindAction(Confirm, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::ConfirmAction);
		EnhancedInputComponent->BindAction(Walk, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::WalkMode);
		EnhancedInputComponent->BindAction(Shoot, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::ShootMode);
		EnhancedInputComponent->BindAction(Next, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::GoNext);
		EnhancedInputComponent->BindAction(Previous, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::GoPrevious);
		EnhancedInputComponent->BindAction(EndTurn, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::EndCharTurn);


	}

}

void AXCOM_Player_Controller::OnInputStarted()
{
	//
}

void AXCOM_Player_Controller::OnSetDestinationTriggered()
{
	//
}

void AXCOM_Player_Controller::OnSetDestinationReleased()
{
	if (bWalkMode) {
		UWorld* World = GetWorld();
		if (World != nullptr) {

			FHitResult Hit;
			bool bHitSuccessful = false;
			// If we hit a surface, cache the location
			bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

			//Todo : Test Dist to Pawn

			FTransform SpawnTransform = FTransform();
			SpawnTransform.SetLocation(Hit.Location);

			ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
			if (ControledPawn != nullptr) {

				ControledPawn->VectDestination = Hit.Location;

				if (ControledPawn->GhostClass != nullptr) {
					UWorld* Worl = GetWorld();
					FTransform GhostSpawnTransform;
					FVector LocationWithZ = Hit.Location;
					ACharacter* Ghost = World->SpawnActorDeferred<ACharacter>(ControledPawn->GhostClass, GhostSpawnTransform);

					//SetCapsule Woks
					UCapsuleComponent* GhostCapsule = Ghost->GetCapsuleComponent();
					UCapsuleComponent* PawnCapsule = ControledPawn->GetCapsuleComponent();
					GhostCapsule->SetCapsuleSize(PawnCapsule->GetScaledCapsuleRadius(), PawnCapsule->GetScaledCapsuleHalfHeight());

					//SetMesh
					USkeletalMeshComponent* GhostMesh = Ghost->GetMesh();
					GhostMesh->SetSkeletalMeshAsset(ControledPawn->GetMesh()->GetSkeletalMeshAsset());
					GhostMesh->SetRelativeTransform(ControledPawn->GetMesh()->GetRelativeTransform());

					//Set Anim
					GhostMesh->SetAnimClass(ControledPawn->GetMesh()->GetAnimClass());

					LocationWithZ.Z += PawnCapsule->GetScaledCapsuleHalfHeight();
					GhostSpawnTransform.SetLocation(LocationWithZ);
					Ghost->FinishSpawning(GhostSpawnTransform);

					if (ControledPawn->MovementGhost != nullptr) {
						ControledPawn->MovementGhost->Destroy();
						ControledPawn->MovementGhost = nullptr;
					}

					ControledPawn->MovementGhost = Ghost;
					return;
				}
			}
		}
	}

}

void AXCOM_Player_Controller::CancelAction()
{
	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	ControledPawn->CanceledExecution();
	bWalkMode = false;
	bShootMode = false;
}

void AXCOM_Player_Controller::ConfirmAction()
{
	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	ControledPawn->ConfirmedExecution();
}

void AXCOM_Player_Controller::WalkMode()
{
	if (bShootMode)return;

	bWalkMode = true;

	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	ControledPawn->ActionToExexcute = &ABaseCharacters::MoveToVectorLocation;
}

void AXCOM_Player_Controller::ShootMode()
{
	if (bWalkMode)return;
	UWorld* World = GetWorld();
	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	if (World != nullptr || ControledPawn == nullptr ) {
		bShootMode = true;
		//Selection de target
		TArray<AActor*> ActorInRange; 
		TArray< AActor* >ActorToIgnore = TArray< AActor* >();
		ActorToIgnore.Add(Cast<AActor>(ControledPawn));
		UKismetSystemLibrary::SphereOverlapActors(World, ControledPawn->GetActorLocation(), ControledPawn->Range, TArray< TEnumAsByte< EObjectTypeQuery > >(), ABaseCharacters::StaticClass(), ActorToIgnore, ActorInRange);
		if (EnnClass && ActorInRange.Num() > 0) {
			for (AActor* Actor : ActorInRange) {
				if (!Actor->IsA(EnnClass)) {
					ActorInRange.Remove(Actor);
				}
			}
			if (ActorInRange.Num() > 0) { //Gross
				ControledPawn->ActorInRange = ActorInRange;
				ControledPawn->SetTarget(ActorInRange[0]);
				ControledPawn->ActionToExexcute = &ABaseCharacters::PewPewExecution;
			}
		}
		else {
			bShootMode = false;
		}
		
	}

}

void AXCOM_Player_Controller::GoNext()
{
	if (bShootMode) {
		ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
		ControledPawn->NextTarget();
	}
}

void AXCOM_Player_Controller::GoPrevious()
{
	if (bShootMode) {
		ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
		ControledPawn->PreviousTarget();
	}
}

void AXCOM_Player_Controller::EndCharTurn()
{	
	if (bShootMode || bWalkMode) return;

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>();

	AXCGameStateBase* GameState = Cast<AXCGameStateBase>(GameMode->GameState);

	if (GameState) {
		GameState->UnitEndTurn();
	}

}

