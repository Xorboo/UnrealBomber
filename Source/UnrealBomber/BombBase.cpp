// Korepanov Kirill, 2017

#include "BombBase.h"


ABombBase::ABombBase()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultRadius = 3;
	RealRadius = DefaultRadius;
}


void ABombBase::IncreaseBlastRadius(int Radius)
{
	RealRadius += Radius;
}
