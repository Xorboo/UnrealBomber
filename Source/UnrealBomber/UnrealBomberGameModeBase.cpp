// Korepanov Kirill, 2017

#include "UnrealBomberGameModeBase.h"
#include "Engine/World.h"
#include "WallBase.h"
#include "MapObjectBase.h"
#include "BombBase.h"
#include "ExplosiomBase.h"


struct BombExplosion 
{
	ABombBase* Bomb;
	int X;
	int Y;
};

AUnrealBomberGameModeBase::AUnrealBomberGameModeBase()
{
	MapSize = 15;
	WallSpawnChance = 40.0;
	Map = nullptr;
}

AUnrealBomberGameModeBase::~AUnrealBomberGameModeBase()
{
	DestroyMap();
}

void AUnrealBomberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateMap();
}

FVector AUnrealBomberGameModeBase::RoundPositionToGrid(FVector Position)
{
	FVector r = Position / 100.0;
	r = FVector(FMath::RoundToInt(r.X), FMath::RoundToInt(r.Y), 0.0);
	return r * 100.0;
}

void AUnrealBomberGameModeBase::BombAdded(ABombBase* Bomb)
{
	auto pos = Bomb->GetActorLocation();
	int x = FMath::RoundToInt(pos.X / 100.0);
	int y = FMath::RoundToInt(pos.Y / 100.0);

	UE_LOG(LogTemp, Display, TEXT("Bomb added on %d, %d."), x, y);

	if (!Map[y][x])
	{
		Map[y][x] = Bomb;
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Bomb spawned on already taken coordinates"));
}

void AUnrealBomberGameModeBase::ExplodeBomb(UObject* WorldContextObject)
{
	auto bomb = Cast<ABombBase>(WorldContextObject);
	if (bomb)
	{
		auto pos = bomb->GetActorLocation();
		int bombX = FMath::RoundToInt(pos.X / 100.0);
		int bombY = FMath::RoundToInt(pos.Y / 100.0);
		UE_LOG(LogTemp, Display, TEXT("Bomb on %d, %d exploded"), bombX, bombY);
		BombExplosion explosion{ bomb, bombX, bombY };
		ChainExplosions(explosion);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Cant cast bomb from world object"));
}

void AUnrealBomberGameModeBase::ChainExplosions(BombExplosion Explosion)
{
	TArray<AMapObjectBase*> destroyed;
	TArray<BombExplosion> explosions;
	TArray<ABombBase*> explodedBombs;
	TArray<FVector> explodedPositions;

	explosions.Add(Explosion);
	destroyed.Add(Explosion.Bomb);
	explodedPositions.Add(FVector(Explosion.X, Explosion.Y, 0));

	while (explosions.Num() > 0)
	{
		auto explosion = explosions.Pop();
		auto bomb = explosion.Bomb;
		explodedBombs.Add(bomb);

		int x = explosion.X;
		int y = explosion.Y;
		Map[y][x] = nullptr;

		for (int sX = x + 1; sX < FMath::Min(x + bomb->GetExplosionRadius(), MapSize); sX++)
		{
			bool shouldStop = CheckExplosion(sX, y, destroyed, explosions, explodedBombs, explodedPositions);
			if (shouldStop)
				break;
		}
		for (int sX = x - 1; sX >= FMath::Max(x - bomb->GetExplosionRadius(), 0); sX--)
		{
			bool shouldStop = CheckExplosion(sX, y, destroyed, explosions, explodedBombs, explodedPositions);
			if (shouldStop)
				break;
		}
		for (int sY = y + 1; sY < FMath::Min(y + bomb->GetExplosionRadius(), MapSize); sY++)
		{
			bool shouldStop = CheckExplosion(x, sY, destroyed, explosions, explodedBombs, explodedPositions);
			if (shouldStop)
				break;
		}
		for (int sY = y - 1; sY >= FMath::Max(y - bomb->GetExplosionRadius(), 0); sY--)
		{
			bool shouldStop = CheckExplosion(x, sY, destroyed, explosions, explodedBombs, explodedPositions);
			if (shouldStop)
				break;
		}
	}

	for (auto iter(destroyed.CreateIterator()); iter; iter++)
	{
		auto obj = *iter;
		//UE_LOG(LogTemp, Display, TEXT("Destroyed object: %s"), *GetNameSafe(obj));
		obj->Destroy();
	}

	auto world = GetWorld();
	for (auto iter(explodedPositions.CreateIterator()); iter; iter++)
	{
		auto position = (*iter) * 100.0;
		auto explosionBase = ExplosionVisuals;

		world->SpawnActor<AExplosiomBase>(ExplosionVisuals, position, FRotator::ZeroRotator);
	}
}

bool AUnrealBomberGameModeBase::CheckExplosion(int x, int y, TArray<AMapObjectBase*>& destroyed, TArray<BombExplosion>& explosions, TArray<ABombBase*>& explodedBombs, TArray<FVector>& explodedPositions)
{
	FVector explosionPosition = FVector(x, y, 0);

	auto obj = Map[y][x];
	if (!obj)
	{
		UE_LOG(LogTemp, Display, TEXT("[%d, %d] - no object"), x, y);
		if (!explodedPositions.Contains(explosionPosition))
			explodedPositions.Add(explosionPosition);

		return false;
	}

	if (!obj->IsDestroyable())
	{
		UE_LOG(LogTemp, Display, TEXT("[%d, %d] - static, %s"), x, y, *GetNameSafe(obj));
		return true;
	}

	if (!explodedPositions.Contains(explosionPosition))
		explodedPositions.Add(explosionPosition);

	auto bomb = Cast<ABombBase>(obj);
	if (bomb && !explodedBombs.Contains(bomb))
	{
		UE_LOG(LogTemp, Display, TEXT("[%d, %d] - bomb - adding it to queue"), x, y);
		explosions.Add(BombExplosion{ bomb, x, y });
	}

	destroyed.Add(obj);
	Map[y][x] = nullptr;

	bool stopBlast = obj->CanConsumeBlast();
	UE_LOG(LogTemp, Display, TEXT("[%d, %d] - destroyable, %s - consume blast: %s"), x, y, *GetNameSafe(obj), stopBlast ? TEXT("TRUE") : TEXT("FALSE"));
	return stopBlast;
}

void AUnrealBomberGameModeBase::GenerateMap()
{
	DestroyMap();

	UE_LOG(LogTemp, Display, TEXT("Generating level..."));

	Map = new AMapObjectBase**[MapSize];
	for (int y = 0; y < MapSize; y++)
	{
		Map[y] = new AMapObjectBase*[MapSize];
		for (int x = 0; x < MapSize; x++)
		{
			Map[y][x] = nullptr;

			// Spawn bound walls
			if (x == 0 || x == MapSize - 1 || y == 0 || y == MapSize - 1)
			{
				SpawnWall(SolidWall, x, y);
				continue;
			}

			// Spawn grid walls
			if (x % 2 == 0 && y % 2 == 0)
			{
				SpawnWall(SolidWall, x, y);
				continue;
			}

			// Dont spawn in corners
			if (FMath::Abs(x - 1) + FMath::Abs(y - 1) <= 1)
				continue;
			if (FMath::Abs(x - 1) + FMath::Abs(y - MapSize + 2) <= 1)
				continue;
			if (FMath::Abs(x - MapSize + 2) + FMath::Abs(y - 1) <= 1)
				continue;
			if (FMath::Abs(x - MapSize + 2) + FMath::Abs(y - MapSize + 2) <= 1)
				continue;

			// Spawn random wall
			if (FMath::FRandRange(0, 100) < WallSpawnChance)
			{
				SpawnWall(DestroyableWall, x, y);
			}
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Done!"));
}


void AUnrealBomberGameModeBase::DestroyMap()
{
	if (!Map)
		return;

	for (int y = 0; y < MapSize; y++) 
	{
		delete[] Map[y];
	}
	delete Map;

	Map = nullptr;
}


void AUnrealBomberGameModeBase::SpawnWall(TSubclassOf<AWallBase> wall, int x, int y)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FVector Position = FVector(x * 100, y * 100, 0);
		auto spawnedWall = World->SpawnActor<AWallBase>(wall, Position, FRotator::ZeroRotator);
		Map[y][x] = spawnedWall;
	}
	else
		UE_LOG(LogTemp, Error, TEXT("No World found"));
}
