// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyPawnWeapon.h"
#include "Rocketlauncher.generated.h"

/**
 * 
 */
UCLASS()
class SILLYGEOCODE_API ARocketlauncher : public AMyPawnWeapon
{
	GENERATED_BODY()
		
public:
	ARocketlauncher(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileFire() override;
};
