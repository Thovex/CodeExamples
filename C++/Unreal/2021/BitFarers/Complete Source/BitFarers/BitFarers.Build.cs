// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BitFarers : ModuleRules
{
	public BitFarers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
	        {  "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "SmoothSyncPlugin", "UMG", "Slate", "SlateCore","NavigationSystem" });

		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

	}
}
