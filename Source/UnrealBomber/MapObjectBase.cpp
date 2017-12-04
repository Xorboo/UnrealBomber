// Korepanov Kirill, 2017

#include "MapObjectBase.h"


AMapObjectBase::AMapObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AMapObjectBase::IsDestroyable()
{
	return bIsDestroyable;
}

bool AMapObjectBase::CanConsumeBlast()
{
	return bCanConsumesBlast;
}