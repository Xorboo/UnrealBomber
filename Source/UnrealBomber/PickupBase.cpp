// Korepanov Kirill, 2017

#include "PickupBase.h"
#include "Components/BoxComponent.h"


// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*bIsDestroyable = true;
	bCanConsumesBlast = false;

	UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
	BoxComponent->bGenerateOverlapEvents = true;
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlapBegin);*/
	//BoxComponent->SetSimulatePhysics(false);
}


void APickupBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Collision!"));
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		AffectPlayer(nullptr);
	}
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupBase::AffectPlayer_Implementation(APlayerPawnBase* player)
{
}

