using UnrealBuildTool;

public class AgeOfAetherTarget : TargetRules
{
    public AgeOfAetherTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("AgeOfAether");
    }
}
