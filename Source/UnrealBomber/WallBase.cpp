// Korepanov Kirill, 2017

#include "WallBase.h"


// Sets default values
AWallBase::AWallBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWallBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
