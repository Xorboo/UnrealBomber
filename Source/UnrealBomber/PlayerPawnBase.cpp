// Korepanov Kirill, 2017

#include "PlayerPawnBase.h"
#include "Engine.h"
#include "Components/InputComponent.h"
#include "BomberPlayerController.h"


const FString APlayerPawnBase::MoveForwardBinding("MoveForward");
const FString APlayerPawnBase::MoveRightBinding("MoveRight");
const FString APlayerPawnBase::FireBinding("PlantBomb");
const FString APlayerPawnBase::BindAppendBase("_P");


APlayerPawnBase::APlayerPawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
}


void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto controller = Cast<ABomberPlayerController>(Controller);
	int32 id = controller->GetLocalPlayer()->GetControllerId();
	UE_LOG(LogTemp, Display, TEXT("Pawn controller id: %d"), id);

	auto append = BindAppendBase + FString::FromInt(id);
	auto forwardBind = FName(*(MoveForwardBinding + append));

	PlayerInputComponent->BindAxis(forwardBind, this, &APlayerPawnBase::MoveForward);
	PlayerInputComponent->BindAxis(FName(*(MoveRightBinding + append)), this, &APlayerPawnBase::MoveRight);
	PlayerInputComponent->BindAction(FName(*(FireBinding + append)), IE_Pressed, this, &APlayerPawnBase::BombPlantPressed);
}


void APlayerPawnBase::MoveForward(float Amount)
{
	MoveForwardAmount = Amount;
}


void APlayerPawnBase::MoveRight(float Amount)
{
	MoveRightAmount = Amount;
}