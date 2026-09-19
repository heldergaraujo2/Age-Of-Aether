#include "Security/AetherSecuritySubsystem.h"

void UAetherSecuritySubsystem::Initialize(FSubsystemCollectionBase&)
{
    Super::Initialize(nullptr);
    FAetherSecurityConfig Config;
    Service.Initialize(Config);
}

void UAetherSecuritySubsystem::Deinitialize()
{
    Service.Shutdown();
    Super::Deinitialize();
}

EAetherSecurityResult UAetherSecuritySubsystem::AuthorizeRequest(uint32 ConnectionId,uint32 RequestId,EAetherSecurityAction Action,bool bAuthenticated,double NowSeconds)
{
    return Service.AuthorizeRequest(ConnectionId,RequestId,Action,bAuthenticated,NowSeconds);
}

EAetherSecurityResult UAetherSecuritySubsystem::RecordInvalidRequest(uint32 ConnectionId,EAetherSecurityAction Action,uint32 RequestId,double NowSeconds)
{
    return Service.RecordInvalidRequest(ConnectionId,Action,RequestId,NowSeconds);
}

bool UAetherSecuritySubsystem::ValidateMovement(uint32 ConnectionId,const FVector& Location,double NowSeconds)
{
    return Service.ValidateMovement(ConnectionId,Location,NowSeconds);
}

void UAetherSecuritySubsystem::RecordSuspiciousEvent(uint32 ConnectionId,EAetherSecurityAction Action,uint32 RequestId,double NowSeconds,int32 Severity)
{
    Service.RecordSuspiciousEvent(ConnectionId,Action,RequestId,NowSeconds,Severity);
}
