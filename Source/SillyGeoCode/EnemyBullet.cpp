// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "EnemyBullet.h"
#include "MyPawn.h"

AEnemyBullet::AEnemyBullet(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}


void AEnemyBullet::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AMyPawn* MyPawn = Cast<AMyPawn>(OtherActor);
	if (MyPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Player HIT");
		MyPawn->DamageTaken(Damage);
		Destroy();
	}
}