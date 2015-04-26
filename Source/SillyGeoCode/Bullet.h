// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Bullet.generated.h"

UCLASS()
class SILLYGEOCODE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet(const FObjectInitializer& ObjectInitializer);

	void InitVelocity(const FVector& ShootDirection);
	void SetDamage(float Damage);

	UFUNCTION()
	virtual void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

public:
	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* BulletCollision;
	
	UPROPERTY(Category = "FX", VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* Sparks;
	
	UPROPERTY(Category = "FX", VisibleAnywhere, BlueprintReadOnly)
	class UParticleSystem* Explosion;

	UPROPERTY(Category = "FX", VisibleDefaultsOnly, BlueprintReadOnly)
	class UPointLightComponent* ProjectileLight;
	
	UPROPERTY(Category = "Movement", VisibleDefaultsOnly, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	float Damage;
};
