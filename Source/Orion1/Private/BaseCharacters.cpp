// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacters.h"

// Sets default values
ABaseCharacters::ABaseCharacters()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacters::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacters::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacters::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

