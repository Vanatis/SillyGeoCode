// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "Enemy.h"


// Sets default values
AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer),
	MaxHealth(100.0f),
	CurrentHealth(MaxHealth)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CollisionComponent
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionComponent->SetSphereRadius(30.0f);
	CollisionComponent->SetCollisionProfileName("Pawn");
	RootComponent = CollisionComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> EnemyMesh(TEXT("/Game/Player/Meshes/SM_Wing_Jet"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> EnemyMat(TEXT("/Game/Player/Materials/M_WingMaterial_Inst"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetStaticMesh(EnemyMesh.Object);
	Mesh->SetMaterial(0, EnemyMat.Object);
	Mesh->AttachTo(RootComponent); 
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HealthBarMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	HealthBar = CreateDefaultSubobject<UStaticMeshComponent>("Healthbar");
	HealthBar->SetStaticMesh(HealthBarMesh.Object);
	HealthBar->bAbsoluteRotation = true;
	HealthBar->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> HealthbarTexObj(TEXT("/Game/HUD/Materials/M_Healthbar_Inst"));
	HealthbarMat = UMaterialInstanceDynamic::Create(HealthbarTexObj.Object, this);
	HealthBar->SetMaterial(0, HealthbarMat);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::DamageTaken(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	
	if (HealthbarMat != NULL)
	{
		float HealthPercent = CurrentHealth / MaxHealth;
		HealthbarMat->SetScalarParameterValue("Mask_Percent", 1.0f - HealthPercent);
		HealthbarMat->SetVectorParameterValue("Color_InputA", FColor::Red);
	}
	if (CurrentHealth <= 0)
	{
		Destroy();
	}
}