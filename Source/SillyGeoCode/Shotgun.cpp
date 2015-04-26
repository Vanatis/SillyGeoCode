// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "Shotgun.h"

AShotgun::AShotgun(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("/Game/Player/Meshes/SM_Wing_Discus"));
	WeaponMesh->SetStaticMesh(Mesh.Object);

	WeaponConfig.Name = "Shotgun";
	WeaponConfig.TimeBetweenShots = 0.5f;
	WeaponConfig.WeaponRange = 300.0f;
	WeaponConfig.WeaponSpread = 60.0f;
	WeaponConfig.WeaponMultiShootCount = 4;

	BulletType = EWeaponProjectile::ESpread;
}

void AShotgun::ProjectileFire()
{
	Super::ProjectileFire();

	if (BulletClass != NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector BulletLoc = WeaponMesh->GetSocketLocation("BulletOrigin");
		FRotator BulletRot = WeaponMesh->GetSocketRotation("BulletOrigin");

		const int32 RandomSeed = FMath::Rand();
		FRandomStream WeaponRandomStream(RandomSeed);
		const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
		FVector ShootDir = WeaponRandomStream.VRandCone(BulletRot.Vector(), SpreadCone, SpreadCone);
		ShootDir.Z = 0;

		ABullet* const Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, BulletLoc, BulletRot, SpawnParams);
		if (Bullet)
		{
			Bullet->InitVelocity(ShootDir);
			Bullet->SetDamage(5.0f);
		}
	}

	bCanShoot = false;
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->GetTimerManager().SetTimer(TimeHandle_ShootCooldownExpired, this, &AMyPawnWeapon::ShootCooldownExpired, WeaponConfig.TimeBetweenShots);
	}
}