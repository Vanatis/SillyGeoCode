// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "SillyGeoHUD.h"
#include "MyPawn.h"
#include "Enemy.h"

ASillyGeoHUD::ASillyGeoHUD(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	HealthbarRadius(100)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/HUD/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> HealthbarTexObj(TEXT("MaterialInstanceConstant'/Game/HUD/Materials/M_Healthbar_Inst.M_Healthbar_Inst'"));
	HealthbarMat = UMaterialInstanceDynamic::Create(HealthbarTexObj.Object, this);
}

void ASillyGeoHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D CrosshairPos; 
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	PController->GetMousePosition(CrosshairPos.X, CrosshairPos.Y);
	CrosshairPos.X -= CrosshairTex->GetSurfaceWidth() / 2;
	CrosshairPos.Y -= CrosshairTex->GetSurfaceHeight() / 2;

	FCanvasTileItem TileItem(CrosshairPos, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
	
	AMyPawn* PlayerPawn = (AMyPawn*)GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn != NULL)
	{
		float playerHealthPercentage = PlayerPawn->GetHealthPercentage();
		FColor playerColor = PlayerPawn->GetWeaponColor();
		DrawHealthbar(PlayerPawn, playerHealthPercentage, playerColor);
	}
}

void ASillyGeoHUD::DrawHealthbar(AActor* Actor, float HealthPercentage, FColor Color)
{
	FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	if (Actor)
	{
		HealthbarMat->SetScalarParameterValue("Mask_Percent", 1.0f - HealthPercentage);
		HealthbarMat->SetVectorParameterValue("Color_InputA", Color);
		GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(Actor->GetActorLocation(), Center);
	}

	DrawMaterialSimple(HealthbarMat, Center.X - HealthbarRadius, Center.Y - HealthbarRadius, HealthbarRadius * 2, HealthbarRadius * 2);
}