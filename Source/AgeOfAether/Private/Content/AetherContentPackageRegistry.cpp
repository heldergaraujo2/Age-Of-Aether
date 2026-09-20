#include "Content/AetherContentPackageRegistry.h"
FString FAetherContentPackageRegistry::Normalize(const FString& Value){return Value.TrimStartAndEnd().ToLower();}
bool FAetherContentPackageRegistry::Register(const UAetherContentPackage* Package)
{
 if(!Package) return false; TArray<FString> Errors; if(!Package->Validate(Errors)) return false;
 const FString Key=Normalize(Package->PackageID); if(Key.IsEmpty()||Packages.Contains(Key)) return false;
 Packages.Add(Key,Package); return true;
}
const UAetherContentPackage* FAetherContentPackageRegistry::Find(const FString& PackageID) const{return Packages.FindRef(Normalize(PackageID));}
bool FAetherContentPackageRegistry::Validate(TArray<FString>& OutErrors) const
{
 OutErrors.Reset();
 for(const auto& Pair:Packages){if(!IsValid(Pair.Value)) OutErrors.Add(FString::Printf(TEXT("Invalid package: %s"),*Pair.Key));}
 return OutErrors.IsEmpty();
}
void FAetherContentPackageRegistry::Reset(){Packages.Reset();}