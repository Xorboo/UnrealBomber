// Korepanov Kirill, 2017

#include "ExplosiomBase.h"


// Sets default values
AExplosiomBase::AExplosiomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExplosiomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

