// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bIsAlly = false;
	CurrentHealth = 100;
	MaxHealth = 100;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

void UHealthComponent::Heal(float HealAmount)
{
	if (CurrentHealth <= 0.0f || HealAmount <= 0.0f) return;

	CurrentHealth = FMath::Clamp(HealAmount + CurrentHealth, 0.0f, MaxHealth);
}

int UHealthComponent::GetArmorStacks()
{
	return CurrentArmorStack;
}

void UHealthComponent::ArmorUP(int ArmorAmount)
{
	if (ArmorAmount <= 0 || CurrentHealth == 0) return;
	CurrentArmorStack = FMath::Clamp(CurrentArmorStack + ArmorAmount, 0, MaxArmorStack);

}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead || Damage < 0 || IsFriendly(DamagedActor,DamageCauser) || DamagedActor == DamageCauser) return;

	float DamageTaken = Damage;
	if (GetArmorStacks() > 0) {
		DamageTaken = Damage * ArmorReducedDamage;
		ArmorUP(-1);

	}
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageTaken, 0.f, MaxHealth);
	
	bIsDead = (CurrentHealth <= 0.f);

}

bool UHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if (ActorA == nullptr || ActorB == nullptr) return true;

	UHealthComponent* HealthCompA = Cast<UHealthComponent>(ActorA->GetComponentByClass(UHealthComponent::StaticClass()));
	UHealthComponent* HealthCompB = Cast<UHealthComponent>(ActorB->GetComponentByClass(UHealthComponent::StaticClass()));

	if (HealthCompA == nullptr || HealthCompB == nullptr) return false;

	return HealthCompA->bIsAlly == HealthCompB->bIsAlly;
}

