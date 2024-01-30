// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hypercasual : ModuleRules
{
	public Hypercasual(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GeometryCollectionEngine", "ChaosSolverEngine" });
	}
}
