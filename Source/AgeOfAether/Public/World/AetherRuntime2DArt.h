#pragma once

#include "CoreMinimal.h"

class UPaperSprite;
class UObject;

class AGEOFAETHER_API FAetherRuntime2DArt
{
public:
    static UPaperSprite* CreateCharacterSprite(UObject* Outer);
    static UPaperSprite* CreateTreeSprite(UObject* Outer, int32 Variant = 0);
    static UPaperSprite* CreateRockSprite(UObject* Outer, int32 Variant = 0);
    static UPaperSprite* CreateHouseSprite(UObject* Outer, int32 Variant = 0);
};
