// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacters.h"
#include "DataAssetForCharacters.h"
#include "XCOM_Player_Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "AbilityShoot.h"
#include "ShootComponent.h"
#include "HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

//FName ABaseCharacters::ShootComponentClassName(TEXT("BP_AbilityShoot"));

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
	HealthComponent = FindComponentByClass<UHealthComponent>();

}

// Called every frame
void ABaseCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Direction;
	FRotator TargetRotation;
	FRotator CurrentRotation;
	FRotator NewRotation;

	if (!GetVelocity().IsNearlyZero(0.1)) {

		Direction = (GetActorLocation() + GetVelocity()) - GetActorLocation();
		Direction.Z = 0; // To prevent unwanted pitch rotation
		Direction.Normalize();
		TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		CurrentRotation = GetActorRotation();
		NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 200.f);
		SetActorRotation(NewRotation);
	}
	else if (Target != nullptr) {

		Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Z = 0; // To prevent unwanted pitch rotation
		Direction.Normalize();
		TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		CurrentRotation = GetActorRotation();
		NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 200.f);
		SetActorRotation(NewRotation);
	}
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
	TArray<AActor*> Ignore;
	Ignore.Add(this);

	for (int i = 0; i < 100; i++) {

		//FMAth::RandCon a la place
		Noise = UKismetMathLibrary::RandomUnitVector() * FMath::Clamp((Direction.Length() - Precision), 50, Precision); //Vraiment Claqué au sol :Smiley Face :

		bool Hit =UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), Start, NoNoiseEnd + Noise, FName("None"), false,Ignore, EDrawDebugTrace::None, OutHit, true);
		if (Hit && OutHit.GetActor()==Target) {
			Chance++;
		}
	}
	//GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, FString::Printf(TEXT("Chance %d "),Chance));

	return Chance;
}

float ABaseCharacters::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent != nullptr) {
		HealthComponent->HandleTakeAnyDamage(this, DamageAmount, nullptr, DamageCauser->GetInstigatorController(), DamageCauser);
		if (HealthComponent->bIsDead) {
			GetMesh()->SetSimulatePhysics(true);

			UPawnMovementComponent* MovementComp = GetMovementComponent();
			if (MovementComp) {
				MovementComp->Deactivate();
				// Define a timer handle
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this](){this->Destroy();}, 3.0f, false);
			}
		}
	}

	return 0.0f;
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
	ChanceToHit();
	if(Cast<ABaseCharacters>(Target)->bIsAlly != bIsAlly )
	ShootComponenttest->UseAbility();

	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cant Shoot All"));
	}
	//Retirer point d action or we 

	//Set Target To None
	Target = nullptr;

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

}

bool ABaseCharacters::IsAlive()
{
	if(HealthComponent != nullptr) return !HealthComponent->bIsDead;
	return false;
}

