// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GEP_Platform : ModuleRules
{
	public GEP_Platform(ReadOnlyTargetRules Target) : base(Target)
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
			//Da qui aggiunti io
			"UMG",					//UserWidget / HUD
			"Slate",				//UserWidget / HUD
			"SlateCore",			//UserWidget / HUD
			"AIModule",				//Enemy
			"NavigationSystem"		//Enemy
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GEP_Platform",
			"GEP_Platform/Variant_Platforming",
			"GEP_Platform/Variant_Platforming/Animation",
			"GEP_Platform/Variant_Combat",
			"GEP_Platform/Variant_Combat/AI",
			"GEP_Platform/Variant_Combat/Animation",
			"GEP_Platform/Variant_Combat/Gameplay",
			"GEP_Platform/Variant_Combat/Interfaces",
			"GEP_Platform/Variant_Combat/UI",
			"GEP_Platform/Variant_SideScrolling",
			"GEP_Platform/Variant_SideScrolling/AI",
			"GEP_Platform/Variant_SideScrolling/Gameplay",
			"GEP_Platform/Variant_SideScrolling/Interfaces",
			"GEP_Platform/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
