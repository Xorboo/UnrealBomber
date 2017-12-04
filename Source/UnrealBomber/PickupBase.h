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
};
