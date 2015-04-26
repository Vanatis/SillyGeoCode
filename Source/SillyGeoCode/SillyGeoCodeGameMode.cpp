// Fill out your copyright notice in the Description page of Project Settings.

#include "SillyGeoCode.h"
#include "SillyGeoCodeGameMode.h"

#include "MyPawn.h"
#include "SillyGeoHUD.h"

ASillyGeoCodeGameMode::ASillyGeoCodeGameMode(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	DefaultPawnClass = AMyPawn::StaticClass();
	HUDClass = ASillyGeoHUD::StaticClass();
}