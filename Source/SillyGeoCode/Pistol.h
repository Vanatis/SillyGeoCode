// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyPawnWeapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class SILLYGEOCODE_API APistol : public AMyPawnWeapon
{
	GENERATED_BODY()
	
public:
	APistol(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileFire() override;
};