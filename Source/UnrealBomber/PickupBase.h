// Korepanov Kirill, 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapObjectBase.h"
#include "PlayerPawnBase.h"
#include "PickupBase.generated.h"

UCLASS()
class UNREALBOMBER_API APickupBase : public AMapObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void AffectPlayer(APlayerPawnBase* player);
	void AffectPlayer_Implementation(APlayerPawnBase* player);
	
private:
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
