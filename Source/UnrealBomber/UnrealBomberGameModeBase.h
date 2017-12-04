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
	FVector RoundPositionToGrid(FVector Position);
	UFUNCTION(BlueprintCallable)
	void BombAdded(class ABombBase* Bomb);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void ExplodeBomb(UObject* WorldContextObject);
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	int MapSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TSubclassOf<class AWallBase> SolidWall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TSubclassOf<class AWallBase> DestroyableWall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float WallSpawnChance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AExplosiomBase> ExplosionVisuals;

private:
	void GenerateMap();
	void DestroyMap();
	void SpawnWall(TSubclassOf<class AWallBase> wall, int x, int y);

	void ChainExplosions(struct BombExplosion Explosion);
	bool CheckExplosion(int x, int y, TArray<class AMapObjectBase*>& destroyed, TArray<struct BombExplosion>& explosions, TArray<ABombBase*>& explodedBombs, TArray<FVector>& explodedPositions);

	class AMapObjectBase*** Map;
};
