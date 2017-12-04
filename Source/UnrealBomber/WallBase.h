// Korepanov Kirill, 2017

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "MapObjectBase.h"
#include "WallBase.generated.h"

UCLASS()
class UNREALBOMBER_API AWallBase : public AMapObjectBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWallBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
