// Korepanov Kirill, 2017

#include "BombBase.h"


ABombBase::ABombBase()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultRadius = 1;
	RealRadius = DefaultRadius;
}


void ABombBase::SetBlastRadius(int Radius)
{
	RealRadius = DefaultRadius + Radius;
}
