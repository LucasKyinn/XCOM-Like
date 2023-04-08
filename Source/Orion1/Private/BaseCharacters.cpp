// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacters.h"
#include "DataAssetForCharacters.h"
#include "XCOM_Player_Controller.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


// Sets default values
ABaseCharacters::ABaseCharacters()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


}

// Called when the game starts or when spawned
void ABaseCharacters::BeginPlay()
{
	Super::BeginPlay();
	VectDestination = GetTargetLocation();
}

// Called every frame
void ABaseCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Not smooth but will do for now
	FVector Velocity = GetVelocity();
	FRotator TargetRotation = Velocity.Rotation();
	SetActorRotation(TargetRotation);


}

// Called to bind functionality to input
void ABaseCharacters::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacters::ConfirmedExecution()
{
	if (ActionToExexcute) {
		(this->*ActionToExexcute)();
		ActionToExexcute = nullptr;
	}
}

void ABaseCharacters::CanceledExecution()
{
	ActionToExexcute = nullptr;
}

void ABaseCharacters::MoveToVectorLocation()
{

	//Set Walk mode off
	AController* PlayerController = GetController();
	AXCOM_Player_Controller* XPlayerController = Cast<AXCOM_Player_Controller>(PlayerController);
	if (XPlayerController) XPlayerController->bWalkMode = false;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller,VectDestination);

}

