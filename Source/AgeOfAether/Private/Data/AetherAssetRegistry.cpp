#include "Data/AetherAssetRegistry.h"

namespace
{
void AddReferenceIssue(
    TArray<FAetherAssetValidationIssue>& Issues,
    const FString& AssetID,
    const TCHAR* Code,
    const FString& Message)
{
    FAetherAssetValidationIssue& Issue = Issues.AddDefaulted_GetRef();
    Issue.AssetID = AssetID;
    Issue.Code = Code;
    Issue.Message = Message;
}
}

bool FAetherAssetRegistry::RegisterAsset(const FAetherAssetDefinition& Definition, FString& OutError)
{
    OutError.Reset();

    if (!Definition.IsStructurallyValid(OutError))
    {
        return false;
    }

    const FString ID = Definition.Asset.AssetID.TrimStartAndEnd();
    if (Assets.Contains(ID))
    {
        OutError = FString::Printf(TEXT("Duplicate AssetID '%s'."), *ID);
        return false;
    }

    FAetherAssetDefinition Copy = Definition;
    Copy.Asset.AssetID = ID;
    Assets.Add(ID, MoveTemp(Copy));
    return true;
}

bool FAetherAssetRegistry::RegisterAssets(
    const TArray<FAetherAssetDefinition>& InDefinitions,
    TArray<FAetherAssetValidationIssue>& OutIssues)
{
    OutIssues.Reset();
    bool bSuccess = true;
    TArray<FAetherAssetValidationIssue> RegistrationIssues;

    for (const FAetherAssetDefinition& Definition : InDefinitions)
    {
        FString Error;
        if (!RegisterAsset(Definition, Error))
        {
            bSuccess = false;
            AddReferenceIssue(RegistrationIssues, Definition.Asset.AssetID, TEXT("RegisterFailed"), Error);
        }
    }

    TArray<FAetherAssetValidationIssue> ValidationIssues;
    if (!Validate(ValidationIssues))
    {
        bSuccess = false;
    }

    OutIssues.Append(RegistrationIssues);
    OutIssues.Append(ValidationIssues);
    return bSuccess;
}

bool FAetherAssetRegistry::Resolve(const FString& AssetID, FAetherAssetDefinition& OutDefinition) const
{
    const FString NormalizedID = AssetID.TrimStartAndEnd();
    const FAetherAssetDefinition* Found = Assets.Find(NormalizedID);
    if (!Found)
    {
        return false;
    }

    OutDefinition = *Found;
    return true;
}

bool FAetherAssetRegistry::Contains(const FString& AssetID) const
{
    return Assets.Contains(AssetID.TrimStartAndEnd());
}

bool FAetherAssetRegistry::Remove(const FString& AssetID)
{
    return Assets.Remove(AssetID.TrimStartAndEnd()) > 0;
}

void FAetherAssetRegistry::Reset()
{
    Assets.Reset();
}

bool FAetherAssetRegistry::Validate(TArray<FAetherAssetValidationIssue>& OutIssues) const
{
    OutIssues.Reset();
    bool bValid = true;

    TArray<FString> IDs;
    Assets.GetKeys(IDs);
    IDs.Sort();

    for (const FString& ID : IDs)
    {
        const FAetherAssetDefinition& Definition = Assets.FindChecked(ID);
        FString Error;

        if (!Definition.IsStructurallyValid(Error))
        {
            AddReferenceIssue(OutIssues, ID, TEXT("InvalidDefinition"), Error);
            bValid = false;
        }

        const FAetherAssetReference& Asset = Definition.Asset;

        if (!Asset.FallbackAssetID.IsEmpty() && !Assets.Contains(Asset.FallbackAssetID.TrimStartAndEnd()))
        {
            AddReferenceIssue(OutIssues, ID, TEXT("MissingFallback"),
                FString::Printf(TEXT("Fallback asset '%s' does not exist."), *Asset.FallbackAssetID));
            bValid = false;
        }

        if (!Asset.SkeletonAssetID.IsEmpty() && !Assets.Contains(Asset.SkeletonAssetID.TrimStartAndEnd()))
        {
            AddReferenceIssue(OutIssues, ID, TEXT("MissingSkeleton"),
                FString::Printf(TEXT("Skeleton asset '%s' does not exist."), *Asset.SkeletonAssetID));
            bValid = false;
        }

        for (const FString& Reference : Asset.AnimationSetIDs)
        {
            if (Reference.TrimStartAndEnd().IsEmpty() || !Assets.Contains(Reference.TrimStartAndEnd()))
            {
                AddReferenceIssue(OutIssues, ID, TEXT("MissingAnimation"),
                    FString::Printf(TEXT("Animation asset '%s' does not exist."), *Reference));
                bValid = false;
            }
        }

        for (const FString& Reference : Asset.MaterialAssetIDs)
        {
            if (Reference.TrimStartAndEnd().IsEmpty() || !Assets.Contains(Reference.TrimStartAndEnd()))
            {
                AddReferenceIssue(OutIssues, ID, TEXT("MissingMaterial"),
                    FString::Printf(TEXT("Material asset '%s' does not exist."), *Reference));
                bValid = false;
            }
        }

        for (const FString& Reference : Asset.VFXAssetIDs)
        {
            if (Reference.TrimStartAndEnd().IsEmpty() || !Assets.Contains(Reference.TrimStartAndEnd()))
            {
                AddReferenceIssue(OutIssues, ID, TEXT("MissingVFX"),
                    FString::Printf(TEXT("VFX asset '%s' does not exist."), *Reference));
                bValid = false;
            }
        }

        for (const FString& Reference : Asset.SFXAssetIDs)
        {
            if (Reference.TrimStartAndEnd().IsEmpty() || !Assets.Contains(Reference.TrimStartAndEnd()))
            {
                AddReferenceIssue(OutIssues, ID, TEXT("MissingSFX"),
                    FString::Printf(TEXT("SFX asset '%s' does not exist."), *Reference));
                bValid = false;
            }
        }
    }

    return bValid && OutIssues.Num() == 0;
}

void FAetherAssetRegistry::GetAssetIDs(TArray<FString>& OutIDs) const
{
    Assets.GetKeys(OutIDs);
    OutIDs.Sort();
}

void FAetherAssetRegistry::AddIssue(
    TArray<FAetherAssetValidationIssue>& OutIssues,
    const FString& AssetID,
    const FString& Code,
    const FString& Message) const
{
    AddReferenceIssue(OutIssues, AssetID, *Code, Message);
}
