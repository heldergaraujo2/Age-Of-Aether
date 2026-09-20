#pragma once
#include "CoreMinimal.h"
#include "Content/AetherContentPackageTypes.h"
class FAetherContentPackageRegistry
{
public:
 bool Register(const UAetherContentPackage* Package);
 const UAetherContentPackage* Find(const FString& PackageID) const;
 bool Validate(TArray<FString>& OutErrors) const;
 void Reset();
 int32 Num() const { return Packages.Num(); }
private:
 static FString Normalize(const FString& Value);
 TMap<FString,TObjectPtr<const UAetherContentPackage>> Packages;
};