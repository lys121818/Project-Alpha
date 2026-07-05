// Fill out your copyright notice in the Description page of Project Settings.


#include "RegionArea.h"
#include "Components/BoxComponent.h"
#include "RegionAreaSubsystem.h"
#include "EnemyAi.h"

// Sets default values
ARegionArea::ARegionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// collision setups
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComp;
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// overlap callback function
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARegionArea::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ARegionArea::BeginPlay()
{
	Super::BeginPlay();
	
	URegionAreaSubsystem* regionSubsystem = GetWorld()->GetSubsystem<URegionAreaSubsystem>();

	if (!IsValid(regionSubsystem))
		return;

	regionSubsystem->RegisterRegion(this);
}

void ARegionArea::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (URegionAreaSubsystem* regionSubsystem =
		GetWorld()->GetSubsystem<URegionAreaSubsystem>())
	{
		regionSubsystem->UnregisterRegion(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ARegionArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyAi* AiPawn = Cast<AEnemyAi>(OtherActor);

	if (!IsValid(AiPawn))
		return;

	AiPawn->OnRegionChanged();
}

bool ARegionArea::IsInsideRegionArea(AActor* target) const
{
	return BoxComp->IsOverlappingActor(target);
}

bool ARegionArea::IsInsideRegionArea(FVector target) const
{
	return BoxComp->Bounds.GetBox().IsInsideOrOn(target);;
}

const TMap<TWeakObjectPtr<AActor>, FVector>& FAreaMemory::GetKnownEnemiesPosition()
{
	return KnownEnemiesPosition;
}

const TArray<TWeakObjectPtr<AActor>>& FAreaMemory::GetKnownItems() const
{
	return KnownItems;
}

void FAreaMemory::AddKnownEnemy(TWeakObjectPtr<AActor> enemy, float time)
{
	if (!enemy.IsValid())
		return;

	KnownEnemiesPosition.FindOrAdd(enemy) = enemy->GetActorLocation();
	KnownEnemiesSeenTime.FindOrAdd(enemy) = time;
}

void FAreaMemory::AddKnownItem(TWeakObjectPtr<AActor> item)
{
	if (!item.IsValid())
		return;

	KnownItems.AddUnique(item);
}

void FAreaMemory::RemoveEnemy(TWeakObjectPtr<AActor> enemy)
{
	KnownEnemiesPosition.Remove(enemy);
	KnownEnemiesSeenTime.Remove(enemy);
}

void FAreaMemory::CleanupInValidItems()
{
	KnownItems.RemoveAllSwap(
		[](const TWeakObjectPtr<AActor>& item)
		{
			return !IsValid(item.Get());
		});
}

void FAreaMemory::CleanupInValidEnemies()
{
	TArray<TWeakObjectPtr<AActor>> invalidEnemies;

	// remove all invalids
	for (const auto& enemy : KnownEnemiesPosition)
	{
		if (!enemy.Key.IsValid())
		{
			invalidEnemies.Add(enemy.Key);
		}
	}

	for (const auto& enemy : invalidEnemies)
	{
		RemoveEnemy(enemy);
	}
}

float FAreaMemory::GetDangerScore(float currentTime) const
{
	float totalDangerScore = 0.0f;
	int32 count = 0;

	// danger socre of area
	for (const auto& enemySeenTime : KnownEnemiesSeenTime)
	{
		float time = currentTime - enemySeenTime.Value;
		totalDangerScore += 1.0f - FMath::Clamp((time / kMaxDangerScoreTime), 0.0f, 1.0f);
		count++;
	}

	if (count <= 0)
		return 0.0f;

	return (totalDangerScore /= count);
}

float FAreaMemory::GetVisitedScore(float currentTime) const
{
	float timeSinceLastVisit = currentTime - LastVisitedTime;

	return FMath::Clamp(timeSinceLastVisit / kMaxVisitedScoreTime,0.0f,1.0f);
}
