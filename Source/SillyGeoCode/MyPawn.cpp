// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "MyPawn.h"
#include "Engine.h"
#include "Engine/Blueprint.h"

AMyPawn::AMyPawn(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	CurrentWeaponIndex(0),
	EquipedWeapon(nullptr),
	bIsFiring(false),
	CurrentVelocity(0.0f, 0.0f, 0.0f),
	MoveSpeed(1500.0f),
	InputDisabledTime(0.1f),
	bInputDisabled(false),
	MaxHealth(100.0f),
	CurrentHealth(MaxHealth)
{
	PrimaryActorTick.bCanEverTick = true;

	//Weapon Colors
	WeaponColors.Add(FColor(255, 0, 0, 255));
	WeaponColors.Add(FColor(0, 0, 255, 255));
	WeaponColors.Add(FColor(0, 255, 0, 255));
	WeaponColors.Add(FColor(255, 204, 0, 255));

	//Inventory reserve space
	WeaponInventory.SetNum(WeaponColors.Num(), false);

	//CollisionComponent
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	CollisionComponent->SetSphereRadius(30.0f);
	CollisionComponent->SetCollisionProfileName("Pawn");
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyPawn::OnCollision);
	RootComponent = CollisionComponent;

	//ScanComponent
	ScanComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ScanComponent"));
	ScanComponent->SetSphereRadius(300.0f);
	ScanComponent->SetCollisionProfileName("OverlapAll");
	ScanComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyPawn::OnScanZoneEnter);
	ScanComponent->OnComponentEndOverlap.AddDynamic(this, &AMyPawn::OnScanZoneLeave);
	ScanComponent->AttachTo(RootComponent);

	//MeshComponent
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PuckMesh(TEXT("/Game/Player/Meshes/SM_PlayerCenter"));
	static ConstructorHelpers::FObjectFinder<UMaterial> PuckRimMat(TEXT("/Game/Player/Materials/M_PlayerRim"));
	static ConstructorHelpers::FObjectFinder<UMaterial> PuckSkinMat(TEXT("/Game/Player/Materials/M_PlayerSkin"));
	Puck = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Puck"));
	Puck->SetRelativeRotation(FRotator(0, 90, 0));
	Puck->SetStaticMesh(PuckMesh.Object);
	Puck->SetMaterial(0, PuckRimMat.Object);
	Puck->SetMaterial(1, PuckSkinMat.Object);
	Puck->AttachTo(RootComponent);

	// Create Trail Component
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TrailFX(TEXT("/Game/Player/Particles/PFX_PlayerTrail"));
	static ConstructorHelpers::FObjectFinder<UMaterial> TrailMat(TEXT("/Game/Player/Materials/M_PlayerTrail"));
	static ConstructorHelpers::FObjectFinder<UMaterial> DistortionMat(TEXT("/Game/Player/Materials/M_PlayerDistortion"));
	Trail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	Trail->SetRelativeLocation(FVector(0.0f, 0.0f, -15.0f));
	Trail->SetTemplate(TrailFX.Object);
	Trail->SetMaterial(0, TrailMat.Object);
	Trail->SetMaterial(1, DistortionMat.Object);
	Trail->SetColorParameter("TrailColor", WeaponColors[CurrentWeaponIndex]);
	Trail->AttachTo(Puck);

	// Create Camera
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->bAbsoluteRotation = true;
	CameraArm->RelativeRotation = FRotator(-90.0f, 0.0f, 0.0f);
	CameraArm->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
	CameraArm->TargetArmLength = 600.0f;
	CameraArm->AttachTo(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCam"));
	Camera->AttachTo(CameraArm, USpringArmComponent::SocketName);

	//Load Pistol
	WeaponSpawn = APistol::StaticClass();
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	//Spawn Pistol at start and put it in the weapon inventory
	FVector WeaponLoc = Puck->GetSocketLocation("Weapon");
	FRotator WeaponRot = Puck->GetSocketRotation("Weapon");
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	//Equip pistol from inventory
	if (WeaponSpawn)
	{
		WeaponInventory[0] = WeaponSpawn;
		EquipWeapon(0);
	}
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement(DeltaTime);

	//Firing
	if (bIsFiring)
	{
		if (EquipedWeapon != nullptr)
		{
			EquipedWeapon->Fire();
		}
	}
}

void AMyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent);

	InputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AMyPawn::SwitchWeapon);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMyPawn::EnableFiring);
	InputComponent->BindAction("Fire", IE_Released, this, &AMyPawn::DisableFiring);

	InputComponent->BindAxis("MoveForward");
	InputComponent->BindAxis("MoveRight"); 
}


void AMyPawn::DamageTaken(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0)
	{
		//You died
		Destroy();
	}
}

float AMyPawn::GetHealthPercentage()
{
	return (CurrentHealth / MaxHealth);
}

const FColor& AMyPawn::GetWeaponColor()
{
	return WeaponColors[CurrentWeaponIndex];
}


const TArray<AEnemy*> AMyPawn::GetNearbyEnemies()
{
	return NearbyEnemyArray;
}

void AMyPawn::HandleMovement(float DeltaTime)
{
	FVector InputMovementForce(0.0f, 0.0f, 0.0f);
	
	//Calculate movement vector
	if (!bInputDisabled)
	{
		float ForwardValue = GetInputAxisValue("MoveForward");
		float RightValue = GetInputAxisValue("MoveRight");
		InputMovementForce = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
	}
	
	const FVector FrictionForce = CurrentVelocity;
	const FVector Acceleration = InputMovementForce - FrictionForce;	
	CurrentVelocity += Acceleration * DeltaTime;
	const FVector Movement = CurrentVelocity * MoveSpeed * DeltaTime;

	//Move Pawn
	if (Movement.SizeSquared() > 0.0f)
	{
		//Check and handle hits
		FHitResult Hit(CollisionComponent->GetScaledSphereRadius());
		RootComponent->MoveComponent(Movement, FRotator::ZeroRotator, true, &Hit);
		if (Hit.IsValidBlockingHit())
		{
			const float ProjectedLength = FVector::DotProduct(CurrentVelocity, Hit.Normal);
			const FVector Projected = Hit.Normal * ProjectedLength;
			const FVector Reflected = (CurrentVelocity - 2 * Projected).GetSafeNormal2D();
			CurrentVelocity = Reflected * (1 - Hit.Time) * CurrentVelocity.Size();

			//Disable Input for a short while
			bInputDisabled = true;
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				World->GetTimerManager().SetTimer(TimeHandle_InputDisabledExpired, this, &AMyPawn::InputDisableExpired, InputDisabledTime);
			}
		}
	}
}

void AMyPawn::EquipWeapon(uint32 WeaponIndex)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	AMyPawnWeapon* WeaponSpawn = GetWorld()->SpawnActor<AMyPawnWeapon>(WeaponInventory[WeaponIndex], SpawnParams);
	if (WeaponSpawn)
	{
		if (EquipedWeapon != NULL)
		{
			FString EquipedWeaponName = EquipedWeapon->WeaponConfig.Name;
			for (int32 i = 0; i < WeaponInventory.Num(); ++i)
			{
				if (WeaponInventory[i] != NULL)
				{
					FString InventoryWeaponName = WeaponInventory[i]->GetDefaultObject<AMyPawnWeapon>()->WeaponConfig.Name;
					if (InventoryWeaponName == EquipedWeaponName)
					{
						WeaponInventory[i] = EquipedWeapon->GetClass();
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "I Put " + EquipedWeaponName + " away in slot " + FString::FromInt(i));
					}
				}
			}
			EquipedWeapon->Destroy();
		}

		WeaponSpawn->SetActorEnableCollision(false);
		WeaponSpawn->AttachRootComponentTo(Puck, "Weapon", EAttachLocation::SnapToTarget);
		EquipedWeapon = WeaponSpawn;
		
		Trail->SetColorParameter("TrailColor", WeaponColors[WeaponIndex]);
		EquipedWeapon->SetWeaponColor(WeaponColors[WeaponIndex]);

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "My current weapon is " + EquipedWeapon->WeaponConfig.Name);
	}
}

void AMyPawn::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//WeaponInventory[0] reserved for Pistol. Always set in Constructor

	AShotgun* Shotgun = Cast<AShotgun>(OtherActor);
	if (Shotgun)
	{
		WeaponInventory[1] = Shotgun->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "Pistol Picked Up: " + Shotgun->WeaponConfig.Name);
		Shotgun->Destroy();
	}

	ARocketlauncher* RocketLauncher = Cast<ARocketlauncher>(OtherActor);
	if (RocketLauncher)
	{
		WeaponInventory[2] = RocketLauncher->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "Pistol Picked Up: " + RocketLauncher->WeaponConfig.Name);
		RocketLauncher->Destroy();
	}
}

void AMyPawn::OnScanZoneEnter(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		NearbyEnemyArray.Add(Enemy);
	}
}

void AMyPawn::OnScanZoneLeave(class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		NearbyEnemyArray.Remove(Enemy);
	}
}

void AMyPawn::SwitchWeapon()
{
	//Is there a weapon in the inventory
	bool ValidWeaponFound = false;
	for (auto weapon : WeaponInventory)
	{
		if (weapon != NULL)
		{
			ValidWeaponFound = true;
		}
	}

	if (!ValidWeaponFound)
	{
		return;
	}

	//if there is, then look for the next one
	uint32 OldWeaponIndex = CurrentWeaponIndex;
	++CurrentWeaponIndex;
	while (WeaponInventory[CurrentWeaponIndex] == NULL)
	{
		++CurrentWeaponIndex;
		CurrentWeaponIndex = CurrentWeaponIndex % WeaponColors.Num();
	}

	if (OldWeaponIndex == CurrentWeaponIndex)
	{
		return;
	}
	else
	{
		EquipWeapon(CurrentWeaponIndex);
	}
}

void AMyPawn::EnableFiring()
{
	bIsFiring = true;
}

void AMyPawn::DisableFiring()
{
	bIsFiring = false;
}

void AMyPawn::InputDisableExpired()
{
	bInputDisabled = false;
}