#include "Data/AetherItemRegistry.h"
#include "Data/AetherAssetRegistry.h"
#include "Data/AetherContentRegistry.h"

bool FAetherItemRegistry::RegisterItem(const FAetherItemDefinition& Definition, FString& OutError)
{
    OutError.Reset();
    if (!Definition.IsStructurallyValid(OutError)) return false;
    const FString ID = Definition.DefinitionID.TrimStartAndEnd();
    if (Definitions.Contains(ID)) { OutError = FString::Printf(TEXT("Duplicate Item DefinitionID '%s'."), *ID); return false; }
    FAetherItemDefinition Copy = Definition;
    Copy.DefinitionID = ID;
    Definitions.Add(ID, MoveTemp(Copy));
    return true;
}

bool FAetherItemRegistry::RegisterItems(const TArray<FAetherItemDefinition>& InDefinitions, TArray<FAetherItemValidationIssue>& OutIssues)
{
    OutIssues.Reset();
    bool bSuccess = true;
    TArray<FAetherItemValidationIssue> RegistrationIssues;
    for (const FAetherItemDefinition& Definition : InDefinitions)
    {
        FString Error;
        if (!RegisterItem(Definition, Error))
        {
            bSuccess = false;
            AddIssue(RegistrationIssues, Definition.DefinitionID, TEXT("RegisterFailed"), Error);
        }
    }
    TArray<FAetherItemValidationIssue> ValidationIssues;
    if (!Validate(ValidationIssues)) bSuccess = false;
    OutIssues.Append(RegistrationIssues);
    OutIssues.Append(ValidationIssues);
    return bSuccess;
}

bool FAetherItemRegistry::Resolve(const FString& DefinitionID, FAetherItemDefinition& OutDefinition) const
{
    const FAetherItemDefinition* Found = Definitions.Find(DefinitionID.TrimStartAndEnd());
    if (!Found) return false;
    OutDefinition = *Found;
    return true;
}

bool FAetherItemRegistry::Contains(const FString& DefinitionID) const
{
    return Definitions.Contains(DefinitionID.TrimStartAndEnd());
}

bool FAetherItemRegistry::Remove(const FString& DefinitionID)
{
    return Definitions.Remove(DefinitionID.TrimStartAndEnd()) > 0;
}

void FAetherItemRegistry::Reset()
{
    Definitions.Reset();
}

bool FAetherItemRegistry::Validate(TArray<FAetherItemValidationIssue>& OutIssues,
    const FAetherContentRegistry* ContentRegistry,
    const FAetherAssetRegistry* AssetRegistry) const
{
    OutIssues.Reset();
    bool bValid = true;
    TArray<FString> IDs;
    Definitions.GetKeys(IDs);
    IDs.Sort();

    for (const FString& ID : IDs)
    {
        const FAetherItemDefinition& Definition = Definitions.FindChecked(ID);
        FString Error;
        if (!Definition.IsStructurallyValid(Error))
        {
            AddIssue(OutIssues, ID, TEXT("InvalidDefinition"), Error);
            bValid = false;
        }

        if (ContentRegistry && !ContentRegistry->Contains(ID))
        {
            AddIssue(OutIssues, ID, TEXT("MissingContentDefinition"),
                FString::Printf(TEXT("Content registry entry '%s' does not exist."), *ID));
            bValid = false;
        }

        const FString VisualIDs[] = { Definition.Visuals.IconAssetID, Definition.Visuals.WorldMeshAssetID, Definition.Visuals.EquippedVisualAssetID };
        for (const FString& AssetID : VisualIDs)
        {
            if (!AssetID.TrimStartAndEnd().IsEmpty() && AssetRegistry && !AssetRegistry->Contains(AssetID))
            {
                AddIssue(OutIssues, ID, TEXT("MissingAssetReference"),
                    FString::Printf(TEXT("Asset '%s' does not exist."), *AssetID));
                bValid = false;
            }
        }

        for (const FAetherEnhancementLevel& Enhancement : Definition.Enhancements)
        {
            if (!Enhancement.VisualAssetID.TrimStartAndEnd().IsEmpty() && AssetRegistry && !AssetRegistry->Contains(Enhancement.VisualAssetID))
            {
                AddIssue(OutIssues, ID, TEXT("MissingEnhancementVisual"),
                    FString::Printf(TEXT("Enhancement visual asset '%s' does not exist."), *Enhancement.VisualAssetID));
                bValid = false;
            }
            for (const FAetherEnhancementMaterial& Material : Enhancement.Materials)
            {
                if (!Definitions.Contains(Material.ItemID.TrimStartAndEnd()))
                {
                    AddIssue(OutIssues, ID, TEXT("MissingEnhancementMaterial"),
                        FString::Printf(TEXT("Enhancement material '%s' does not exist."), *Material.ItemID));
                    bValid = false;
                }
            }
        }
    }
    return bValid && OutIssues.Num() == 0;
}

void FAetherItemRegistry::GetDefinitionIDs(TArray<FString>& OutIDs) const
{
    Definitions.GetKeys(OutIDs);
    OutIDs.Sort();
}

void FAetherItemRegistry::AddIssue(TArray<FAetherItemValidationIssue>& OutIssues,
    const FString& DefinitionID, const FString& Code, const FString& Message) const
{
    FAetherItemValidationIssue& Issue = OutIssues.AddDefaulted_GetRef();
    Issue.DefinitionID = DefinitionID;
    Issue.Code = Code;
    Issue.Message = Message;
}
