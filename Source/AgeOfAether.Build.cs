using UnrealBuildTool;

public class AgeOfAether : ModuleRules
{
    public AgeOfAether(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(System.IO.Path.Combine(ModuleDirectory, "AgeOfAether", "Public"));
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "UMG",
            "InputCore",
            "EnhancedInput"
        });
    }
}
