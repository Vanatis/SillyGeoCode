// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class SILLYGEOCODE_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void DamageTaken(float DamageAmount);

public:
	UPROPERTY(Category = "Collision", VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* HealthBar;

private:
	UMaterialInstanceDynamic* HealthbarMat;
	
	float MaxHealth;
	float CurrentHealth;
};
