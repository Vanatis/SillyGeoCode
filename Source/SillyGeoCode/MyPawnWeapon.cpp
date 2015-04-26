// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "MyPawnWeapon.h"
#include "Enemy.h"
#include "Engine.h"
#include "Engine/Blueprint.h"

AMyPawnWeapon::AMyPawnWeapon(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer),
	bCanShoot(true)
{
 	PrimaryActorTick.bCanEverTick = true;
	
	//CollisionComponent
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionComponent->SetSphereRadius(18.0f);
	RootComponent = CollisionComponent;

	//MeshComponent
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachTo(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> WeaponMat(TEXT("/Game/Player/Materials/M_WingMaterial_Inst"));
	WeaponMatInstance = UMaterialInstanceDynamic::Create(WeaponMat.Object, this);
	WeaponMesh->SetMaterial(0, WeaponMatInstance);

	WeaponConfig.Name = "Weapon";
	WeaponConfig.TimeBetweenShots = 1.0f;
	WeaponConfig.WeaponRange = 500.0f;
	WeaponConfig.WeaponSpread = 5.0f;
	WeaponConfig.WeaponMultiShootCount = 1;

	BulletType = EWeaponProjectile::EBullet;

	static ConstructorHelpers::FObjectFinder<UBlueprint> BulletBlueprint(TEXT("Blueprint'/Game/Weapons/BP_PlayerBullet.BP_PlayerBullet'"));
	BulletClass = NULL;
	if (BulletBlueprint.Succeeded())
	{
		BulletClass = (UClass*)BulletBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AMyPawnWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPawnWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/* Handle Rotate To Mouse */
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	FVector MouseWorldPos = FVector(0, 0, 0);
	FVector MouseDir = FVector(0, 0, 0);
	PController->DeprojectMousePositionToWorld(MouseWorldPos, MouseDir);
	
	FVector MouseAimVector = MouseDir * 10;
	MouseAimVector = FVector(MouseAimVector.X, -MouseAimVector.Y, 0);
	FRotator ReticleRotator = FRotationMatrix::MakeFromX(MouseAimVector).Rotator();
	FVector ReticleDistanceVec = FVector(200, 0, 0);
	FVector Location = ReticleRotator.UnrotateVector(ReticleDistanceVec);

	FVector WingsDir = Location - RootComponent->RelativeLocation;
	FRotator WingsRotator = FRotationMatrix::MakeFromX(WingsDir).Rotator() + FRotator(0.0f, 90.0f, 0.0f);
	FRotator WingsRot = RootComponent->RelativeRotation;

	FRotator InterpWingsRot = FMath::RInterpTo(WingsRot, WingsRotator, DeltaTime, 10);
	RootComponent->SetRelativeRotation(InterpWingsRot);
}

void AMyPawnWeapon::Fire()
{
	if (bCanShoot)
	{
		if (BulletType == EWeaponProjectile::EBullet || BulletType == EWeaponProjectile::ERocket)
		{
			ProjectileFire();
		}
		else if (BulletType == EWeaponProjectile::ESpread)
		{
			for (int32 i = 0; i < WeaponConfig.WeaponMultiShootCount; ++i)
			{
				ProjectileFire();
			}
		}
	}
}

void AMyPawnWeapon::ProjectileFire()
{
}

void AMyPawnWeapon::ShootCooldownExpired()
{
	bCanShoot = true;
}


void AMyPawnWeapon::SetWeaponColor(FColor color)
{
	if (WeaponMatInstance)
	{
		WeaponMatInstance->SetVectorParameterValue("WingColor", color);
	}
}