// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "PlayerBullet.h"
#include "MyPawnWeapon.generated.h"

UENUM(BlueprintType)
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet		UMETA(DisplayName = "Bullet"),
		ESpread		UMETA(DisplayName = "Spread"),
		ERocket		UMETA(DisplayName = "Rocket")
	};
}

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = "Config", EditDefaultsOnly)
	FString Name;

	UPROPERTY(Category = "Config", EditDefaultsOnly)
	float TimeBetweenShots;

	UPROPERTY(Category = "Config", EditDefaultsOnly)
	float WeaponRange;

	UPROPERTY(Category = "Config", EditDefaultsOnly)
	float WeaponSpread;

	UPROPERTY(Category = "Config", EditDefaultsOnly)
	int32 WeaponMultiShootCount;
};


UCLASS()
class SILLYGEOCODE_API AMyPawnWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyPawnWeapon(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void Fire();

	UFUNCTION()
	virtual void ProjectileFire();

	UFUNCTION()
	void ShootCooldownExpired();

	void SetWeaponColor(FColor color);

public:
	UPROPERTY(Category = "Collision", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAcces = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly)
	UMaterialInstanceDynamic* WeaponMatInstance;

	UPROPERTY(Category = "Config", EditDefaultsOnly)
	FWeaponData WeaponConfig;

	UPROPERTY(Category = "Config", EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EWeaponProjectile::ProjectileType> BulletType;

	UPROPERTY(Category = "Bullet", EditDefaultsOnly)
	TSubclassOf<class ABullet> BulletClass;

protected:
	bool bCanShoot;
	FTimerHandle TimeHandle_ShootCooldownExpired;
};
