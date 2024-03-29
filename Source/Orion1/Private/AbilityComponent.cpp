// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "BaseCharacters.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "SpellDataAsset.h"
#include "Sound/SoundCue.h"


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Owner = Cast<ABaseCharacters>(GetOwner());

	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAbilityComponent::IsTargetValid()
{
	UHealthComponent* TargetHealthComp = Cast<UHealthComponent>(Target->GetComponentByClass(UHealthComponent::StaticClass()));
	if (TargetHealthComp == nullptr) return false;
	if ((TargetHealthComp->bIsAlly && !bCanHitAllies) || (!TargetHealthComp->bIsAlly && !bCanHitEnnemies) || (bCanHitSelf && Target==Cast<ABaseCharacters>(GetOwner())))
	return false;
	return true;
}

void UAbilityComponent::UseAbility()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Ability"));
}

void UAbilityComponent::PlayAbilityEffect(FVector TargetSpawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("PlayAbilityEffect"));
}

