// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "Bullet.h"
#include "MyPawnWeapon.h"

#define TRACE_PROJECTILE ECC_GameTraceChannel2


// Sets default values
ABullet::ABullet(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	Damage(10.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	BulletCollision->InitSphereRadius(5.0f);
	BulletCollision->BodyInstance.SetCollisionProfileName("Projectile");
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnCollision);
	RootComponent = BulletCollision;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SparksFX(TEXT("/Game/Weapons/Particles/PFX_SimpleProjectile"));
	static ConstructorHelpers::FObjectFinder<UMaterial> SparksMat(TEXT("/Game/Player/Materials/M_WorldSparks"));
	Sparks = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Sparks"));
	Sparks->AttachTo(RootComponent);
	Sparks->SetTemplate(SparksFX.Object);
	Sparks->SetMaterial(0, SparksMat.Object);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionFX(TEXT("/Game/Weapons/Particles/PFX_SimpleProjectile_Explode"));
	Explosion = ExplosionFX.Object;

	ProjectileLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	ProjectileLight->AttachTo(RootComponent);
	ProjectileLight->Intensity = 500;
	ProjectileLight->AttenuationRadius = 2500.0f;
	ProjectileLight->SourceRadius = 5.0f;
	ProjectileLight->CastShadows = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovement->UpdatedComponent = BulletCollision;
	ProjectileMovement->bInitialVelocityInLocalSpace = false;
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->MaxSpeed = 500.0f;

	InitialLifeSpan = 5.0f;
}

void ABullet::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void ABullet::SetDamage(float Damage)
{
	this->Damage = Damage;
}

void ABullet::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
}