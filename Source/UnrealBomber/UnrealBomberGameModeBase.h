// Korepanov Kirill, 2017

#pragma once

#include "Core.h"
#include "GameFramework/GameMode.h"
#include "UnrealBomberGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALBOMBER_API AUnrealBomberGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
	
	
public:
	AUnrealBomberGameModeBase();
	~AUnrealBomberGameModeBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FVector RoundPositionToGrid(FVector position);

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	int MapSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TSubclassOf<class AWallBase> SolidWall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TSubclassOf<class AWallBase> DestroyableWall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float WallSpawnChance;

private:
	void GenerateMap();
	void SpawnWall(TSubclassOf<class AWallBase> wall, int x, int y);


};
