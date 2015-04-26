// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bullet.h"
#include "EnemyBullet.generated.h"

/**
 * 
 */
UCLASS()
class SILLYGEOCODE_API AEnemyBullet : public ABullet
{
	GENERATED_BODY()

public:
	AEnemyBullet(const FObjectInitializer& ObjectInitializer);

	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit) override;
};
