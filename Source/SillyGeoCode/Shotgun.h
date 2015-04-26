// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyPawnWeapon.h"
#include "Shotgun.generated.h"

/**
*
*/
UCLASS()
class SILLYGEOCODE_API AShotgun : public AMyPawnWeapon
{
	GENERATED_BODY()

public:
	AShotgun(const FObjectInitializer& ObjectInitializer);

	virtual void ProjectileFire() override;
};
