// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAlphaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../FirstPersonBase.h"
AProjectAlphaGameMode::AProjectAlphaGameMode()
{
	// stub
}

void AProjectAlphaGameMode::StartPlay()
{
	Super::StartPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFirstPersonBase::StaticClass(), actors);

	for (const auto& actor : actors)
	{
		AddCharacterToList(actor);
	}

	
}

/*
* Start location should be a firing point of the actor while End location is center point of the actor
*/
bool AProjectAlphaGameMode::CheckIfTargetVisible(FVector start, FVector end, AActor* ignore)
{
	// set raytracing points
	FVector endLocation = end;
	FHitResult hit;

	bool isHit = CheckRaycastHit(hit, start, endLocation, ignore);

	if (!isHit)
		return true;

	endLocation = end;
	endLocation.Z += 70.0f;

	if (!CheckRaycastHit(hit, start, endLocation, ignore))
		return true;

	endLocation.Z -= 70.0f;

	endLocation = end;
	endLocation.X += 50.0f;

	isHit &= CheckRaycastHit(hit, start, endLocation, ignore);
	if (!CheckRaycastHit(hit, start, endLocation, ignore))
		return true;

	endLocation = end;
	endLocation.X -= 50.0f;

	if (!CheckRaycastHit(hit, start, endLocation, ignore))
		return true;

	endLocation = end;
	endLocation.Y += 50.0f;

	if (!CheckRaycastHit(hit, start, endLocation, ignore))
		return true;


	endLocation = end;
	endLocation.Y -= 50.0f;

	if (!CheckRaycastHit(hit, start, endLocation, ignore))
		return true;


	return false;
}

bool AProjectAlphaGameMode::CheckIfCoverFromTarget(FVector start, FVector end)
{
	// set raytracing points
	FVector endLocation = end;
	endLocation.Z = 100.0f;
	FHitResult hit;

	bool isHit = CheckRaycastHit(hit, start, endLocation, nullptr);

	endLocation = end;
	endLocation.Z += 70.0f;

	isHit &= CheckRaycastHit(hit, start, endLocation, nullptr);
	endLocation.Z -= 70.0f;

	endLocation = end;
	endLocation.X += 50.0f;

	isHit &= CheckRaycastHit(hit, start, endLocation, nullptr);

	endLocation = end;
	endLocation.X -= 50.0f;

	isHit &= CheckRaycastHit(hit, start, endLocation, nullptr);

	endLocation = end;
	endLocation.Y += 50.0f;

	isHit &= CheckRaycastHit(hit, start, endLocation, nullptr);

	endLocation = end;
	endLocation.Y -= 50.0f;

	isHit &= CheckRaycastHit(hit, start, endLocation, nullptr);

	return isHit;
}

bool AProjectAlphaGameMode::CheckRaycastHit(FHitResult& result, FVector start, FVector end, AActor* ignore)
{

	// collision check of ray cast
	FCollisionQueryParams params(SCENE_QUERY_STAT(FireTrace), true);
	ECollisionChannel wallCollision = ECC_GameTraceChannel1;
	ECollisionChannel characterCollision = ECC_GameTraceChannel2;

	if (ignore)
		params.AddIgnoredActor(ignore);

	// wall check
	bool isHit = GetWorld()->LineTraceSingleByChannel(
		result,
		start,
		end,
		ECC_Visibility,
		params
	);

	// character collision check
	if (!isHit)
	{
		isHit = GetWorld()->SweepSingleByChannel(result, start, end, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(20.0f), params);
		const FColor debugLineColor = isHit ? FColor::Red : FColor::Green;

		DrawDebugSphere(GetWorld(), end, 50.0f, 16, debugLineColor, false, 5.0f, 0, .5f);
	}

	return isHit;
}

void AProjectAlphaGameMode::AddCharacterToList(AActor* actor)
{
	m_worldActors.Add(actor);
}
