// Korepanov Kirill, 2017

#include "UnrealBomberGameModeBase.h"
#include "Engine/World.h"
#include "WallBase.h"


AUnrealBomberGameModeBase::AUnrealBomberGameModeBase()
{
	MapSize = 15;
	WallSpawnChance = 40.0;
}

AUnrealBomberGameModeBase::~AUnrealBomberGameModeBase()
{
}

void AUnrealBomberGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GenerateMap();
}

FVector AUnrealBomberGameModeBase::RoundPositionToGrid(FVector position)
{
	FVector r = position / 100.0;
	r = FVector(FMath::Round(r.X), FMath::Round(r.Y), FMath::Round(r.Z));
	return r * 100.0;
}


void AUnrealBomberGameModeBase::GenerateMap()
{
	UE_LOG(LogTemp, Display, TEXT("Generating level..."));
	for (int x = 0; x < MapSize; x++)
	{
		for (int y = 0; y < MapSize; y++)
		{
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


void AUnrealBomberGameModeBase::SpawnWall(TSubclassOf<AWallBase> wall, int x, int y)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FVector Position = FVector(x * 100, y * 100, 0);
		World->SpawnActor<AWallBase>(wall, Position, FRotator::ZeroRotator);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("No World found"));
}
