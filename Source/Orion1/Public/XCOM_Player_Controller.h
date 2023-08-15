// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "XCOM_Player_Controller.generated.h"


/**
 * 
 */
UCLASS()
class ORION1_API AXCOM_Player_Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	AXCOM_Player_Controller();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationAction;

	//Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Cancel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Confirm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Walk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Shoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Next;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Previous;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EndTurn;
	
	// Other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters")
	TSubclassOf <ACharacter> AllyClass;

	// Other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters")
	TSubclassOf <ACharacter> EnnClass;

	UPROPERTY(BlueprintReadOnly, Category = "Characters", meta = (AllowPrivateAccess = "true"))
	TArray<ACharacter*> AllyArray; 

	bool bWalkMode = false;
	bool bShootMode = false;

protected: 

	UPROPERTY(BlueprintReadOnly)
	ACharacter* CurrControlled;


	//Basic Functions
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual void SetupInputComponent() override;

	//Input Functions
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	UFUNCTION(BlueprintCallable)
	void CancelAction();

	UFUNCTION(BlueprintCallable)
	void ConfirmAction();

	UFUNCTION(BlueprintCallable)
	void WalkMode();

	UFUNCTION(BlueprintCallable)
	void ShootMode();

	UFUNCTION(BlueprintCallable)
	void GoNext();

	UFUNCTION(BlueprintCallable)
	void GoPrevious();

	UFUNCTION(BlueprintCallable)
	void EndCharTurn();
};
