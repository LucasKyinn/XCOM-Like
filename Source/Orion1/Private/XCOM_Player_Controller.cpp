// Fill out your copyright notice in the Description page of Project Settings.


#include "XCOM_Player_Controller.h"

#include "BaseCharacters.h"
#include "BaseCharacters.h"
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
	// If Walk Mode On
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnSetDestinationReleased"));

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

	//Focus Sur le perso

	//Set Walk Mode On

	ABaseCharacters* ControledPawn = Cast<ABaseCharacters>(GetPawn());
	ControledPawn->ActionToExexcute = &ABaseCharacters::MoveToVectorLocation;

	//Set Walk Mode Off 
}
