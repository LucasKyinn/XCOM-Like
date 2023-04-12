// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacters.h"
#include "DataAssetForCharacters.h"
#include "XCOM_Player_Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
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

int ABaseCharacters::ChanceToHit()
{
	int Chance = 0;
	FVector Start = GetActorLocation();
	FVector NoNoiseEnd, Direction, Noise;
	FHitResult OutHit;
	Direction = Target->GetActorLocation() - Start;
	NoNoiseEnd = Direction.GetSafeNormal() * Direction.Length() * 2 + Start;

	for (int i = 0; i < 100; i++) {

		Noise = UKismetMathLibrary::RandomUnitVector() * FMath::Clamp((Direction.Length() - Precision), 50, Precision); //Vraiment Claqué au sol :Smiley Face :
		ActorLineTraceSingle(OutHit, Start, NoNoiseEnd + Noise, ECollisionChannel::ECC_GameTraceChannel1, FCollisionQueryParams()) ? Chance++ : Chance;
		DrawDebugLine(GetWorld(), Start, NoNoiseEnd + Noise, FColor::Red, false, 5.0f);
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Chance %d "),Chance));

	return Chance;
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

void ABaseCharacters::PewPewExecution()
{
	//TODO SHOOT ON THE ENEMIES PEWPEWPEW RATATATATTAATATTAATATAT
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("RATATATATTAATATTAATATAT"));


	ChanceToHit();


	//Set Shoot mode off
	AController* PlayerController = GetController();
	AXCOM_Player_Controller* XPlayerController = Cast<AXCOM_Player_Controller>(PlayerController);
	if (XPlayerController) XPlayerController->bShootMode = false;
}

void ABaseCharacters::NextTarget()
{
	if (ActorInRange.Num() == 0) return;
	int place = ActorInRange.Find(Target);
	if (place == ActorInRange.Num()-1) Target = ActorInRange[0]; //Fin de l'array -1 ????? 
	else Target = ActorInRange[(place+1)];
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, Target->GetActorLabel());

	//Focus sur Target ? probablment plus facile en BP
}

void ABaseCharacters::PreviousTarget()
{
	if (ActorInRange.Num() == 0) return;
	int place = ActorInRange.Find(Target);
	if (place == 0) Target = ActorInRange[(ActorInRange.Num()-1)]; 
	else Target = ActorInRange[(place-1)];
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, Target->GetActorLabel());

	//Focus sur Target ? probablment plus facile en BP

}

