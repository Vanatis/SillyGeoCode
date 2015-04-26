// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyPawnWeapon.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "Enemy.h"
#include "Rocketlauncher.h"
#include "MyPawn.generated.h"

UCLASS()
class SILLYGEOCODE_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void DamageTaken(float DamageAmount);
	float GetHealthPercentage();
	const FColor& GetWeaponColor();

	const TArray<AEnemy*> GetNearbyEnemies();

protected:
	void HandleMovement(float DeltaTime);
	void EquipWeapon(uint32 WeaponIndex);

	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	UFUNCTION()
	void OnScanZoneEnter(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	UFUNCTION()
	void OnScanZoneLeave(class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SwitchWeapon();

	UFUNCTION()
	void EnableFiring();

	UFUNCTION()
	void DisableFiring();

	UFUNCTION()
	void InputDisableExpired();

public:
	UPROPERTY(Category = "Collision", VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* CollisionComponent;

	UPROPERTY(Category = "Collision", VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* ScanComponent;

	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* Puck;

	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly)
	class UParticleSystemComponent* Trail;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* CameraArm;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;

	UPROPERTY(Category = "Weapon", VisibleAnywhere)
	TSubclassOf<class AMyPawnWeapon> WeaponSpawn;

protected:
	TArray<FColor> WeaponColors;
	TArray<TSubclassOf<AMyPawnWeapon>> WeaponInventory;
	
	uint8 CurrentWeaponIndex;
	AMyPawnWeapon* EquipedWeapon;
	bool bIsFiring;

	FVector CurrentVelocity;
	float MoveSpeed;
	float InputDisabledTime;
	bool bInputDisabled;
	FTimerHandle TimeHandle_InputDisabledExpired;

	float MaxHealth;
	float CurrentHealth;
	TArray<AEnemy*> NearbyEnemyArray;
};
