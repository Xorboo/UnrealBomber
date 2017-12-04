// Korepanov Kirill, 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapObjectBase.h"
#include "BombBase.generated.h"

UCLASS()
class UNREALBOMBER_API ABombBase : public AMapObjectBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABombBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bomb")
	int DefaultRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Bomb")
	int RealRadius;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void IncreaseBlastRadius(int Radius);
	UFUNCTION(BlueprintCallable)
	int GetExplosionRadius() { return RealRadius; }
	
};
