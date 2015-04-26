// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "PlayerBullet.h"
#include "Enemy.h"

APlayerBullet::APlayerBullet(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
}


void APlayerBullet::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Enemy HIT");
		Enemy->DamageTaken(Damage);

		//Disable bullet
		BulletCollision->Deactivate();
		Sparks->Deactivate();
		ProjectileLight->SetVisibility(false);
		ProjectileMovement->Deactivate();
		if (Explosion)
		{
			UGameplayStatics::SpawnEmitterAttached(Explosion, BulletCollision);
		}


		return;
	}

	ABlockingVolume* BlockingVolume = Cast<ABlockingVolume>(OtherActor);
	AStaticMeshActor* StaticMesh = Cast <AStaticMeshActor>(OtherActor);
	if (BlockingVolume || StaticMesh)
	{
		//Disable bullet
		BulletCollision->Deactivate();
		Sparks->Deactivate();
		ProjectileLight->SetVisibility(false);
		ProjectileMovement->Deactivate();
		if (Explosion)
		{
			UGameplayStatics::SpawnEmitterAttached(Explosion, BulletCollision);
		}
		return;
	}
}