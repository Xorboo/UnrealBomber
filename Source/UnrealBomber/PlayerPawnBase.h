// Korepanov Kirill, 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnBase.generated.h"

UCLASS()
class UNREALBOMBER_API APlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawnBase();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
