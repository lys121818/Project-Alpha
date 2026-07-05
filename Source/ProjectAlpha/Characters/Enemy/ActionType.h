// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EActionType : uint8
{
    kNone       UMETA(DisplayName = "None"),

    // Combat Action
    kCover      UMETA(DisplayName = "CoverAction"),
    kAttack     UMETA(DisplayName = "AttackAction"),
    kReload     UMETA(DisplayName = "ReloadAction"),

    // Non Combat Action
    kLoot       UMETA(DisplayName = "LootAction"),
    kEscape     UMETA(DisplayName = "EscapeAction"),

    // Both
    kHeal       UMETA(DisplayName = "HealAction"),
};
