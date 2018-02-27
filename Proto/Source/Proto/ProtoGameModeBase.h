// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProtoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API AProtoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
		virtual void StartPlay() override;
	
	
};
