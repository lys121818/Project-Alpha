// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetData.h"
#include "RegionAreaSubsystem.h"

void FTargetData::AddFoundItem(AActor* actor, float currentTime)
{
	if (!IsValid(actor))
		return;

	// refresh current time
	for (auto& target : m_foundPickupItems)
	{
		if (target.Item == actor)
		{
			target.LastTime = currentTime;
			return;
		}
	}

	FFoundItem newTarget;
	newTarget.Item = actor;
	newTarget.LastTime = currentTime;
	m_foundPickupItems.Add(newTarget);


}

void FTargetData::RemoveFoundItem(float currentTime, float expireTime)
{
	// remove all valid containing expired actor
	m_foundPickupItems.RemoveAllSwap(
		[currentTime, expireTime](const FFoundItem& Target)
	{
		return !IsValid(Target.Item.Get()) ||
			currentTime - Target.LastTime > expireTime;
	});
}

void FTargetData::AddCausedTarget(AActor* actor, float currentTime)
{
	if (!IsValid(actor))
		return;

	// refresh current time
	for (auto& target : m_causedTargets)
	{
		if (target.Actor == actor)
		{
			target.LastTime = currentTime;
			return;
		}
	}

	FDamagedActor newTarget;
	newTarget.Actor = actor;
	newTarget.LastTime = currentTime;
	m_causedTargets.Add(newTarget);
}

void FTargetData::RemoveCausedTargets(float currentTime, float expireTime)
{
	// remove all valid containing expired actor
	m_causedTargets.RemoveAllSwap(
		[currentTime, expireTime](const FDamagedActor& Target)
		{
			return !IsValid(Target.Actor.Get()) ||
				currentTime - Target.LastTime > expireTime;
		});
}
