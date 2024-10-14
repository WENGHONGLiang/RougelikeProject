// Copyright Qibo Pang 2022. All Rights Reserved.

using UnrealBuildTool;

public class UMGSplineEditor : ModuleRules
{
	public UMGSplineEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Json",
				"JsonUtilities",
				"UMGSpline",
				"EditorStyle",
				"InputCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
        //  4.23+
        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
          new string[] {  "BlueprintGraph",
                "KismetCompiler",
                "GraphEditor",
                "UnrealEd",
                "Kismet",
				"ToolMenus",
				 }
			);
        }

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
