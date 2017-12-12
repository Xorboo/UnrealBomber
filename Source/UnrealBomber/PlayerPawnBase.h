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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(BlueprintReadOnly, Category = "Controls")
	float MoveForwardAmount;
	UPROPERTY(BlueprintReadOnly, Category = "Controls")
	float MoveRightAmount;

	UFUNCTION(BlueprintImplementableEvent)
	void BombPlantPressed();

protected:


private:
	static const FString MoveForwardBinding;
	static const FString MoveRightBinding;
	static const FString FireBinding;
	static const FString BindAppendBase;


	void MoveForward(float Amount);
	void MoveRight(float Amount);
};
