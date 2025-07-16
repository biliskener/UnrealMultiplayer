// Copyright 2025 (c) Tangha Technologies, LLC. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class SteamSession : ModuleRules
{
	public SteamSession(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


		if (Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			OptimizeCode = CodeOptimization.Never;
		}

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				ModuleDirectory,
			}
			);
				
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "Sockets",

                "OnlineSubsystemUtils",
				"OnlineSubsystem",
                "Networking",
                "Sockets"
            }
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "UMG",
                "Slate",
				"SlateCore",
				"AppFramework",
				"InputCore",
				"Json"
				// ... add private dependencies that you statically link with here ...	
			}
			);

        PublicDependencyModuleNames.AddRange(new string[] { "SteamShared", "Steamworks", "OnlineSubsystemSteam" });
        AddEngineThirdPartyPrivateStaticDependencies(Target, "Steamworks");

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
