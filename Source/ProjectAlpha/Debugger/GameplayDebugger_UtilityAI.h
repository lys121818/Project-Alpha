// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
/**
*	Extending the Unreal Engine Gameplay Debugger
 *  source from https://www.youtube.com/watch?v=TDxA9EkxpcA
 */
class PROJECTALPHA_API GameplayDebugger_UtilityAI : public FGameplayDebuggerCategory
{
public:
	GameplayDebugger_UtilityAI();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;

	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

private:
	struct FRepData
	{
		FString Text;

		void Serialize(FArchive& Ar)
		{
			Ar << Text;
		}
	};

	FRepData Data;
};
