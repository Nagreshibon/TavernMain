// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TAV : ModuleRules
{
	public TAV(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "GameplayTasks", "UMG", "Slate", "SlateCore", "GameplayTags" });
    }
}
