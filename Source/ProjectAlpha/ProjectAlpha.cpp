// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAlpha.h"
#include "Modules/ModuleManager.h"

#include "GameplayDebugger.h"
#include "GameplayDebugger_UtilityAI.h"

class FProjectAlphaModule : public FDefaultGameModuleImpl
{
public:

    virtual void StartupModule() override
    {
#if WITH_GAMEPLAY_DEBUGGER

        IGameplayDebugger::Get().RegisterCategory(
            TEXT("UtilityAI"),
            IGameplayDebugger::FOnGetCategory::CreateStatic(
                &GameplayDebugger_UtilityAI::MakeInstance),
            EGameplayDebuggerCategoryState::EnabledInGameAndSimulate,
            5
        );

        IGameplayDebugger::Get().NotifyCategoriesChanged();

#endif
    }

    virtual void ShutdownModule() override
    {
#if WITH_GAMEPLAY_DEBUGGER

        if (IGameplayDebugger::IsAvailable())
        {
            IGameplayDebugger::Get().UnregisterCategory(TEXT("UtilityAI"));
        }

#endif
    }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FProjectAlphaModule, ProjectAlpha, "ProjectAlpha" );

DEFINE_LOG_CATEGORY(LogProjectAlpha)