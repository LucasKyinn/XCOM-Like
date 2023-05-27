// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityShoot.h"
#include "AbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacters.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


void UAbilityShoot::UseAbility()
{
	// jouer le son du tir 
	if (AbilitySound) {
		UGameplayStatics::SpawnSoundAttached(Cast<USoundBase>(AbilitySound), Owner->GetRootComponent());
	}

	FVector Start = Owner->GetActorLocation();
	FVector NoNoiseEnd, Direction, Noise;
	FHitResult OutHit;
	Direction = Cast<ABaseCharacters>(Owner)->Target->GetActorLocation() - Start;
	NoNoiseEnd = Direction.GetSafeNormal() * Direction.Length() * 2 + Start;
	TArray<AActor*> Ignore;
	Ignore.Add(Owner);
	Noise = UKismetMathLibrary::RandomUnitVector() * FMath::Clamp((Direction.Length() - Owner->Precision), 50, Owner->Precision); 


	// Tirer
	bool Hit = UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), Start, NoNoiseEnd + Noise, FName("None"), false, Ignore, EDrawDebugTrace::ForDuration, OutHit, true);
	AActor* HitActor = OutHit.GetActor();

	FVector EyesLocation;
	FRotator EyesRotation;

	Owner->GetActorEyesViewPoint(EyesLocation, EyesRotation);
	FVector ShotDirection = EyesRotation.Vector();


	//Retirer des PV 
	if (Hit) {
		UGameplayStatics::ApplyPointDamage(HitActor, Owner->BaseDamage, ShotDirection, OutHit, Owner->GetInstigatorController(), Owner, DamageType);
	}

	// Jouer effet du tir 
	PlayAbilityEffect(OutHit.ImpactPoint);
	

}


void UAbilityShoot::PlayAbilityEffect(FVector TargetSpawn)
{
	USkeletalMeshComponent* MeshComp= Owner->GetMesh();
	if (MuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect) {
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);

		if (TracerComp) {
			TracerComp->SetVectorParameter(TracerTargetName, TargetSpawn);
		}
	}


}
