// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegionArea.generated.h"

class UBoxComponent;

USTRUCT()
struct FAreaMemory
{
	GENERATED_BODY()

	static constexpr float kMaxDangerScoreTime = 60.0f;	// max value of time effecting danger score from last seen time
	static constexpr float kMaxVisitedScoreTime = 300.0f;	// max value of time effecting visited score from last visited time
public:
	const TMap<TWeakObjectPtr<AActor>, FVector>& GetKnownEnemiesPosition();
	const TArray<TWeakObjectPtr<AActor>>& GetKnownItems() const;
	float GetLastVisitedTime() { return LastVisitedTime; }

	void AddKnownEnemy(TWeakObjectPtr<AActor> enemy, float time);

	void AddKnownItem(TWeakObjectPtr<AActor> item);

	void RemoveEnemy(TWeakObjectPtr<AActor> enemy);


	void SetLastVisitedTime(float time) { LastVisitedTime = time; }

	float GetDangerScore(float currentTime) const;
	float GetVisitedScore(float currentTime) const;

	void CleanupInValidItems();

	void CleanupInValidEnemies();

private:

	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, FVector> KnownEnemiesPosition;

	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, float> KnownEnemiesSeenTime;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> KnownItems;

	UPROPERTY()
	float LastVisitedTime;
};

UCLASS()
class PROJECTALPHA_API ARegionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARegionArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Region")
	TArray<TObjectPtr<ARegionArea>> ConnectedAreas;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Region")
	FName RegionName;

public:	

	FName GetAreaName() const { return RegionName; }

	bool IsInsideRegionArea(AActor* target) const;
	bool IsInsideRegionArea(FVector target) const;

	const TArray<TObjectPtr<ARegionArea>>& GetConnectedAreas() const { return ConnectedAreas; }
};
