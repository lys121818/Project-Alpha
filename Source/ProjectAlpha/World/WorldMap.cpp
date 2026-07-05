// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap.h"
#include "ProjectAlphaGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWorldMap::AWorldMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<FVector> AWorldMap::GetSurroundedWalkables(FVector location, float distance)
{
	TArray<FVector> walkables;

	// tile index
	int x = FMath::FloorToInt((location.X + kDefaultOffset.X) / kTileSize);
	int y = FMath::FloorToInt((location.Y + kDefaultOffset.Y) / kTileSize);

	int tileRadius = (int)(distance / kTileSize);

	const int minX = FMath::Max(0, x - tileRadius);
	const int maxX = FMath::Min(kMaxTileWidthSize - 1, x + tileRadius);
	const int minY = FMath::Max(0, y - tileRadius);
	const int maxY = FMath::Min(kMaxTileHeightSize - 1, y + tileRadius);

	// world tile length
	for (int wy = 0; wy < kMaxTileHeightSize; wy++)
	{
		//world tile width
		for (int wx = 0; wx < kMaxTileHeightSize; wx++)
		{
			const FVector tileLocation = FVector(wx * kTileSize + kDefaultOffset.X, wy * kTileSize + kDefaultOffset.Y,0.0f);
			const FVector actorLocation = FVector(location.X, location.Y, 0.0f);
			
			// out bound
			if (FVector::DistSquared(tileLocation, actorLocation) > FMath::Square(distance))
				continue;

			// static wall
			if ((TileType)kWorldMap[wx][wy] == TileType::kStatic)
				continue;

			walkables.Add(tileLocation);
		}
	}

	return walkables;
}

void AWorldMap::InitWorldMap()
{
	CreateTiles();
	SetItemSpawnPoints();
}

// Called when the game starts or when spawned
void AWorldMap::BeginPlay()
{
	Super::BeginPlay();
	InitWorldMap();

	// set world map to game mode
	AProjectAlphaGameMode* gameMode = Cast<AProjectAlphaGameMode>(UGameplayStatics::GetGameMode(this));
	if (gameMode)
		gameMode->SetWorldMap(this);

}

// Called every frame
void AWorldMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldMap::CreateTiles()
{
	for (int y = 0; y < kMaxTileHeightSize; ++y)
	{
		for (int x = 0; x < kMaxTileWidthSize; ++x)
		{
			
			FVector location = FVector(x * kTileSize + kDefaultOffset.X, y * kTileSize + kDefaultOffset.Y, 0.0f + kDefaultOffset.Z);
			FTileData newTile(location);
			newTile.SetLocation(location);
			m_tiles.Add(newTile);
		}
	}
}

void AWorldMap::SetItemSpawnPoints()
{

}

void AWorldMap::DebugDrawWorldMap() const
{
	if (!GetWorld())
		return;

	for (int y = 0; y < kMaxTileHeightSize; ++y)
	{
		for (int x = 0; x < kMaxTileWidthSize; ++x)
		{
			DebugDrawSingleTile(x, y);
		}
	}
}

void AWorldMap::DebugDrawSingleTile(int x, int y) const
{
	if (!GetWorld())
		return;

	const FVector center = FVector(x * kTileSize + kDefaultOffset.X, y * kTileSize + kDefaultOffset.Y, 10.0f);

	const FVector tileSize = FVector(kTileSize * 0.5f, kTileSize * 0.5f, 10.0f);

	const TileType tileType = (TileType)kWorldMap[x][y];

	FColor color = FColor::White;
	switch (tileType)
	{
	case TileType::kNone:
		color = FColor::Green;   // walkable
		break;
	case TileType::kStatic:
		color = FColor::Red;     // wall
		break;
	case TileType::kPickup:
		color = FColor::Yellow;  // pickup
		break;
	default:
		color = FColor::White;
		break;
	}

	DrawDebugBox(
		GetWorld(),
		center,
		tileSize,
		color,
		true,     // persistent
		-1.0f,    // lifetime
		0,
		2.0f
	);

	DrawDebugPoint(GetWorld(), center, 8.0f, color, true, -1.0f);
}

