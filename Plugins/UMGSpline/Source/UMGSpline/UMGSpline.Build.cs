// Copyright Qibo Pang 2022. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UMGSpline : ModuleRules
{
	public UMGSpline(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"ApplicationCore",
				"CoreUObject",
				"Engine",
				"UMG",
				"Slate",
				"SlateCore"
			}
		);
	}
}
