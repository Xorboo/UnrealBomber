// Korepanov Kirill, 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapObjectBase.generated.h"

UCLASS()
class UNREALBOMBER_API AMapObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapObjectBase();

protected:
	/** If an object will be destroyed by bomb explosion */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MapObject")
	bool bIsDestroyable;

	/** If an object will stop the explosion from spreading */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MapObject")
	bool bCanConsumesBlast;

public:	
	UFUNCTION(BlueprintCallable)
	bool IsDestroyable();
	UFUNCTION(BlueprintCallable)
	bool CanConsumeBlast();

};
