// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "Rocketlauncher.h"

ARocketlauncher::ARocketlauncher(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Player/Meshes/SM_Wing_Jet"));
	WeaponMesh->SetStaticMesh(Mesh.Object);

	WeaponConfig.Name = "Rocketlauncher";
	WeaponConfig.TimeBetweenShots = 2.0f;
	WeaponConfig.WeaponRange = 2000.0f;
	WeaponConfig.WeaponSpread = 5.0f;

	BulletType = EWeaponProjectile::ERocket;
}

void ARocketlauncher::ProjectileFire()
{
	Super::ProjectileFire();

	if (BulletClass != NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector BulletLoc = WeaponMesh->GetSocketLocation("BulletOrigin");
		FRotator BulletRot = WeaponMesh->GetSocketRotation("BulletOrigin");

		APlayerBullet* const Bullet = GetWorld()->SpawnActor<APlayerBullet>(BulletClass, BulletLoc, BulletRot, SpawnParams);
		if (Bullet)
		{
			Bullet->InitVelocity(BulletRot.Vector());
			Bullet->SetDamage(50.0f);
		}
	}

	bCanShoot = false;
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->GetTimerManager().SetTimer(TimeHandle_ShootCooldownExpired, this, &AMyPawnWeapon::ShootCooldownExpired, WeaponConfig.TimeBetweenShots);
	}
}