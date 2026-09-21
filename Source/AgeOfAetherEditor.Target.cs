using UnrealBuildTool;

public class AgeOfAetherEditorTarget : TargetRules
{
    public AgeOfAetherEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("AgeOfAether");
    }
}
