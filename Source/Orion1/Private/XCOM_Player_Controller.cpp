// Fill out your copyright notice in the Description page of Project Settings.


#include "XCOM_Player_Controller.h"

#include "BaseCharacters.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


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
		// EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Triggered, this, &AXCOM_Player_Controller::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationAction, ETriggerEvent::Canceled, this, &AXCOM_Player_Controller::OnSetDestinationReleased);

		// Actions Controls 
		EnhancedInputComponent->BindAction(Cancel, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::CancelAction);
		EnhancedInputComponent->BindAction(Confirm, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::ConfirmAction);
		EnhancedInputComponent->BindAction(Walk, ETriggerEvent::Completed, this, &AXCOM_Player_Controller::WalkMode);
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
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("bWalkMode"));

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

				// Create Ghost Walking to point ?
				if (ControledPawn->GhostClass != nullptr) {
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ControledPawn->GhostClass != nullptr"));

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
				}
			}
		}
	}


}

void AXCOM_Player_Controller::CancelAction()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("CancelAction"));
	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	ControledPawn->CanceledExecution();
}

void AXCOM_Player_Controller::ConfirmAction()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ConfirmAction"));
	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	ControledPawn->ConfirmedExecution();
}

void AXCOM_Player_Controller::WalkMode()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("WalkMode"));


	bWalkMode = true;

	//Focus Sur le perso

	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	ControledPawn->ActionToExexcute = &ABaseCharacters::MoveToVectorLocation;
}
