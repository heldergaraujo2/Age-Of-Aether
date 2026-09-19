using UnrealBuildTool;

public class AgeOfAetherServerTarget : TargetRules
{
    public AgeOfAetherServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("AgeOfAether");
    }
}
