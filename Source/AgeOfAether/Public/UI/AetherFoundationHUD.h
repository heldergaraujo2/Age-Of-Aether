#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "AetherFoundationHUD.generated.h"

UCLASS()
class AGEOFAETHER_API AAetherFoundationHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
};
