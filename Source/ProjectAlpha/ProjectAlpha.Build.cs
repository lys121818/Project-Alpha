// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectAlpha : ModuleRules
{
	public ProjectAlpha(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayDebugger"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ProjectAlpha",
            "ProjectAlpha/Characters/Enemy",
            "ProjectAlpha/Characters/Enemy/EnvQuery",
            "ProjectAlpha/Characters/Enemy/Services",
            "ProjectAlpha/Characters/Enemy/Tasks",
            "ProjectAlpha/Characters/Player",
            "ProjectAlpha/Characters",
            "ProjectAlpha/Components",
            "ProjectAlpha/DataAsset",
            "ProjectAlpha/DataAsset/Considerations",
            "ProjectAlpha/DataAsset/Base",
            "ProjectAlpha/GameActors",
            "ProjectAlpha/GameMode",
            "ProjectAlpha/Managers",
            "ProjectAlpha/UserInterface",
            "ProjectAlpha/UtilityAI",
            "ProjectAlpha/World",
            "ProjectAlpha/Debugger",
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
