// Korepanov Kirill, 2017

#include "BombBase.h"


// Sets default values
ABombBase::ABombBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultRadius = 3;
	RealRadius = DefaultRadius;
}

// Called when the game starts or when spawned
void ABombBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABombBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABombBase::IncreaseBlastRadius(int Radius)
{
	RealRadius += Radius;
}
