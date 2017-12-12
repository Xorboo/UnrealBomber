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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bomb")
	int DefaultRadius;

	UPROPERTY(BlueprintReadOnly, Category = "Bomb")
	int RealRadius;


public:
	UFUNCTION(BlueprintCallable)
	void SetBlastRadius(int Radius);
	UFUNCTION(BlueprintCallable)
	int GetExplosionRadius() { return RealRadius; }
	
};
