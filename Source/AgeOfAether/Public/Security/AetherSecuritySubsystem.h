#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Security/AetherSecurityService.h"
#include "Security/AetherSecurityTypes.h"
#include "AetherSecuritySubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherSecuritySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    EAetherSecurityResult AuthorizeRequest(uint32 ConnectionId,uint32 RequestId,EAetherSecurityAction Action,bool bAuthenticated,double NowSeconds);
    EAetherSecurityResult RecordInvalidRequest(uint32 ConnectionId,EAetherSecurityAction Action,uint32 RequestId,double NowSeconds);
    bool ValidateMovement(uint32 ConnectionId,const FVector& Location,double NowSeconds);
    void RecordSuspiciousEvent(uint32 ConnectionId,EAetherSecurityAction Action,uint32 RequestId,double NowSeconds,int32 Severity=1);

    FAetherSecurityService& GetService() { return Service; }
    const FAetherSecurityConfig& GetConfig() const { return Service.GetConfig(); }

private:
    FAetherSecurityService Service;
};
