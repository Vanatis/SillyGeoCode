// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "SillyGeoHUD.generated.h"

/**
 * 
 */
UCLASS()
class SILLYGEOCODE_API ASillyGeoHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASillyGeoHUD(const FObjectInitializer& ObjectInitializer);
	
	virtual void DrawHUD() override;

private:
	void DrawHealthbar(AActor* Actor, float HealthPercentage, FColor Color);

private:
	UTexture2D* CrosshairTex;
	UMaterialInstanceDynamic* HealthbarMat;

	float HealthbarRadius;
};
