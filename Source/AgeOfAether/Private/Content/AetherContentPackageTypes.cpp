#include "Content/AetherContentPackageTypes.h"
bool FAetherContentPackageEntry::IsValid() const
{
    return !ContentID.TrimStartAndEnd().IsEmpty() && !DisplayName.TrimStartAndEnd().IsEmpty() &&
        !Version.TrimStartAndEnd().IsEmpty();
}
bool UAetherContentPackage::Validate(TArray<FString>& OutErrors) const
{
    OutErrors.Reset();
    if (PackageID.TrimStartAndEnd().IsEmpty() || DisplayName.TrimStartAndEnd().IsEmpty() || Version.TrimStartAndEnd().IsEmpty())
        OutErrors.Add(TEXT("Package identity is required."));
    TSet<FString> IDs;
    for (const auto& Entry : Entries)
    {
        if (!Entry.IsValid()) OutErrors.Add(FString::Printf(TEXT("Invalid content entry: %s"), *Entry.ContentID));
        const FString Key = Entry.ContentID.TrimStartAndEnd().ToLower();
        if (IDs.Contains(Key)) OutErrors.Add(FString::Printf(TEXT("Duplicate content entry: %s"), *Key));
        IDs.Add(Key);
    }
    TSet<FString> Assets;
    for (const FString& AssetID : RequiredAssetIDs)
    {
        const FString Key = AssetID.TrimStartAndEnd().ToLower();
        if (Key.IsEmpty() || Assets.Contains(Key)) OutErrors.Add(TEXT("Invalid or duplicate required asset ID."));
        Assets.Add(Key);
    }
    return OutErrors.IsEmpty();
}