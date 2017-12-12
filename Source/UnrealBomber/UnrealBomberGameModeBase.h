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


	/** Events called when new object is spawned */
	UFUNCTION(BlueprintCallable)
	void BombAdded(class ABombBase* Bomb);
	UFUNCTION(BlueprintCallable)
	void PickupAdded(class APickupBase* Pickup);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void PickupCollected(UObject* WorldContextObject);

	/** Coordinate helpers */
	UFUNCTION(BlueprintCallable)
	FVector RoundPositionToGrid(FVector Position);
	UFUNCTION(BlueprintCallable)
	bool IsFreeTile(FVector Position);
	UFUNCTION(BlueprintCallable)
	bool IsFreeTileCoords(int X, int Y);

	/** Called by the first exploded bomb, which starts the chain reaction */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void ExplodeBomb(UObject* WorldContextObject);
	
protected:
	/** Parameters for generation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	int MapSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TSubclassOf<class AWallBase> SolidWall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	TSubclassOf<class AWallBase> DestroyableWall;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float WallSpawnChance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Generation")
	float RestartPause;

	/** Explosion */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class AExplosiomBase> ExplosionVisuals;

private:
	/** Generation functions */
	void LaunchGame();
	void GenerateMap();
	void DestroyMap();
	void SpawnWall(TSubclassOf<class AWallBase> wall, int x, int y);
	void RestartGame();

	/** Destroying functions */
	void ChainExplosions(struct BombExplosion Explosion);
	bool CheckExplosion(int x, int y, TArray<class AMapObjectBase*>& destroyed, TArray<struct BombExplosion>& explosions, TArray<ABombBase*>& explodedBombs, TArray<FVector>& explodedPositions);

	/** Coordinate helpers */
	void PositionToCoordinate(const FVector& Position, int& X, int &Y);

	/** Current map data */
	class AMapObjectBase*** Map;
	/** All players */
	TArray<AActor*> Players;

	void SpawnPlayersIfNeeded();
};
