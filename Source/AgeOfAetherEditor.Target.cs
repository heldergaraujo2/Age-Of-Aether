using UnrealBuildTool;

public class AgeOfAetherEditorTarget : TargetRules
{
    public AgeOfAetherEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("AgeOfAether");
    }
}
