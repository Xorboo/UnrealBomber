// Korepanov Kirill, 2017

#include "MapObjectBase.h"


AMapObjectBase::AMapObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMapObjectBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMapObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


bool AMapObjectBase::IsDestroyable()
{
	return bIsDestroyable;
}

bool AMapObjectBase::CanConsumeBlast()
{
	return bCanConsumesBlast;
}