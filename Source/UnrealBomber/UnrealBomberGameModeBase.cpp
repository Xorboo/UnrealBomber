// Korepanov Kirill, 2017

#include "UnrealBomberGameModeBase.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "WallBase.h"
#include "MapObjectBase.h"
#include "BombBase.h"
#include "PickupBase.h"
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
	RestartPause = 3.0;

	Map = nullptr;
}

AUnrealBomberGameModeBase::~AUnrealBomberGameModeBase()
{
	//DestroyMap();
}

void AUnrealBomberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnPlayersIfNeeded();

	// Generate level
	LaunchGame();
}


void AUnrealBomberGameModeBase::SpawnPlayersIfNeeded()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerPawnBase::StaticClass(), Players);

	for(int i = Players.Num(); i < 2; i++)
	{
		auto actor = UGameplayStatics::CreatePlayer(GetWorld());
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerPawnBase::StaticClass(), Players);
}


FVector AUnrealBomberGameModeBase::RoundPositionToGrid(FVector Position)
{
	FVector r = Position / 100.0;
	r = FVector(FMath::RoundToInt(r.X), FMath::RoundToInt(r.Y), 0.0);
	return r * 100.0;
}

void AUnrealBomberGameModeBase::BombAdded(ABombBase* Bomb)
{
	int x = 0, y = 0;
	PositionToCoordinate(Bomb->GetActorLocation(), x, y);

	if (IsFreeTileCoords(x, y))
	{
		UE_LOG(LogTemp, Display, TEXT("Bomb added on %d, %d."), x, y);
		Map[y][x] = Bomb;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Bomb spawned on already taken coordinates [%d, %d], killing it"), x, y);
		Bomb->Destroy();
	}
}


void AUnrealBomberGameModeBase::PickupAdded(APickupBase* Pickup)
{
	if (Pickup)
	{
		int x = 0, y = 0;
		PositionToCoordinate(Pickup->GetActorLocation(), x, y);

		if (IsFreeTileCoords(x, y))
		{
			UE_LOG(LogTemp, Display, TEXT("Pickup added on %d, %d."), x, y);
			Map[y][x] = Pickup;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Pickup spawned on already taken coordinates [%d, %d], killing it"), x, y);
			Pickup->Destroy();
		}
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Empty pickup spawned"));
}


void AUnrealBomberGameModeBase::PickupCollected(UObject* WorldContextObject)
{
	auto pickup = Cast<APickupBase>(WorldContextObject);
	if (pickup)
	{
		int x, y;
		PositionToCoordinate(pickup->GetActorLocation(), x, y);
		UE_LOG(LogTemp, Display, TEXT("Pickup on [%d, %d] collected"), x, y);

		if (Map[y][x] == pickup)
		{
			Map[y][x] = nullptr;
			pickup->SetLifeSpan(3);
		}
		else
			UE_LOG(LogTemp, Error, TEXT("There is another object on pickup position"));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Cant cast pickup from world object"));
}

bool AUnrealBomberGameModeBase::IsFreeTile(FVector Position)
{
	int x = 0, y = 0;
	PositionToCoordinate(Position, x, y);
	return IsFreeTileCoords(x, y);
}

bool AUnrealBomberGameModeBase::IsFreeTileCoords(const int X, const int Y)
{
	return Map[Y][X] == nullptr;
}

void AUnrealBomberGameModeBase::ExplodeBomb(UObject* WorldContextObject)
{
	auto bomb = Cast<ABombBase>(WorldContextObject);
	if (bomb)
	{
		int x, y;
		PositionToCoordinate(bomb->GetActorLocation(), x, y);
		UE_LOG(LogTemp, Display, TEXT("Bomb on [%d, %d] exploded"), x, y);
		BombExplosion explosion{ bomb, x, y };
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

	// Explode chain of bombs
	while (explosions.Num() > 0)
	{
		auto explosion = explosions.Pop();
		auto bomb = explosion.Bomb;
		explodedBombs.Add(bomb);

		int x = explosion.X;
		int y = explosion.Y;
		Map[y][x] = nullptr;

		// Check adjacent tiles of that bomb
		// We need separate loops so we can stop each of them when colliding with static tile
		for (int sX = x + 1; sX <= FMath::Min(x + bomb->GetExplosionRadius(), MapSize); sX++)
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
		for (int sY = y + 1; sY <= FMath::Min(y + bomb->GetExplosionRadius(), MapSize); sY++)
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

	// Destroy objects
	for (auto iter(destroyed.CreateIterator()); iter; iter++)
	{
		auto obj = *iter;
		//UE_LOG(LogTemp, Display, TEXT("Destroyed object: %s"), *GetNameSafe(obj));
		if (obj->IsA(AWallBase::StaticClass()))
		{
			FOutputDeviceNull ar;
			obj->CallFunctionByNameWithArguments(TEXT("OnPreDestroy"), ar, nullptr, true);
		}
		obj->Destroy();
	}

	auto world = GetWorld();

	// Get all players with centered positions
	TMap<FVector, AActor*> PlayerPositions;
	for (auto iter(Players.CreateIterator()); iter; iter++)
	{
		auto player = *iter;
		int x, y;
		PositionToCoordinate(player->GetTargetLocation(), x, y);
		FVector centeredPosition = FVector(x, y, 0);
		PlayerPositions.Add(centeredPosition, player);
	}

	// Check if any player is killed
	bool bPlayerWasKilled = false;
	for (auto iter(explodedPositions.CreateIterator()); iter; iter++)
	{
		auto position = (*iter);
		auto playerRef = PlayerPositions.Find(position);
		if (playerRef)
		{
			auto player = *playerRef;
			bPlayerWasKilled = true;
			UE_LOG(LogTemp, Warning, TEXT("Player '%s' is dead - not implemented, sorry"), *GetNameSafe(player));

			FOutputDeviceNull ar;
			player->CallFunctionByNameWithArguments(TEXT("OnPlayerDead"), ar, nullptr, true);
		}
	}
	
	// Restart the game if so
	if (bPlayerWasKilled)
	{
		FTimerHandle unusedHandle;
		GetWorldTimerManager().SetTimer(unusedHandle, this, &AUnrealBomberGameModeBase::RestartGame, RestartPause, false);
	}

	// Spawn explosion particles
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
		//UE_LOG(LogTemp, Display, TEXT("[%d, %d] - no object"), x, y);
		if (!explodedPositions.Contains(explosionPosition))
			explodedPositions.Add(explosionPosition);

		return false;
	}

	if (!obj->IsDestroyable())
	{
		//UE_LOG(LogTemp, Display, TEXT("[%d, %d] - static, %s"), x, y, *GetNameSafe(obj));
		return true;
	}

	if (!explodedPositions.Contains(explosionPosition))
		explodedPositions.Add(explosionPosition);

	auto bomb = Cast<ABombBase>(obj);
	if (bomb && !explodedBombs.Contains(bomb))
	{
		//UE_LOG(LogTemp, Display, TEXT("[%d, %d] - bomb - adding it to queue"), x, y);
		explosions.Add(BombExplosion{ bomb, x, y });
	}

	destroyed.Add(obj);
	Map[y][x] = nullptr;

	bool stopBlast = obj->CanConsumeBlast();
	//UE_LOG(LogTemp, Display, TEXT("[%d, %d] - destroyable, %s - consume blast: %s"), x, y, *GetNameSafe(obj), stopBlast ? TEXT("TRUE") : TEXT("FALSE"));
	return stopBlast;
}


void AUnrealBomberGameModeBase::LaunchGame()
{
	GenerateMap();

	int index = 0;
	for (auto iter(Players.CreateIterator()); iter; iter++)
	{
		auto player = *iter;

		/** Update player position
		  * This will work only for 2 players
		  * For more players we should probably store/calculate positions for each index
		 */
		float size = (MapSize - 3) * 100.0;
		FVector position(size * index, size * index, 0.0);
		FVector shift(100.0, 100.0, 50.0);
		player->SetActorLocation(position + shift);
		UE_LOG(LogTemp, Display, TEXT("Actor %d on position %s"), index, *(position + shift).ToString());

		FOutputDeviceNull ar;
		player->CallFunctionByNameWithArguments(TEXT("OnNewMatchStarted"), ar, nullptr, true);

		index++;
	}
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
		for (int x = 0; x < MapSize; x++)
		{
			auto obj = Map[y][x];
			if (obj)
				obj->Destroy();
		}
		delete[] Map[y];
	}
	delete Map;

	Map = nullptr;
}

void AUnrealBomberGameModeBase::RestartGame()
{
	UE_LOG(LogTemp, Display, TEXT("Game restart"));
	LaunchGame();
}

void AUnrealBomberGameModeBase::SpawnWall(TSubclassOf<AWallBase> wall, int x, int y)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FVector Position = FVector(x * 100, y * 100, 0);
		auto spawnedWall = World->SpawnActor<AWallBase>(wall, Position, FRotator::ZeroRotator);
		Map[y][x] = spawnedWall;
#if WITH_EDITOR
		spawnedWall->SetFolderPath("Level");
#endif
	}
	else
		UE_LOG(LogTemp, Error, TEXT("No World found"));
}

void AUnrealBomberGameModeBase::PositionToCoordinate(const FVector& Position, int& X, int &Y)
{
	X = FMath::RoundToInt(Position.X / 100.0);
	Y = FMath::RoundToInt(Position.Y / 100.0);
}