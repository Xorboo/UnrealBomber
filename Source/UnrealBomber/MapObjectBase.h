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
	// Sets default values for this actor's properties
	AMapObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	bool bIsDestroyable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsDestroyable();

	
	
};
