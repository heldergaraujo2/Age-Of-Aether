#include "Core/Runtime/AetherServiceRegistry.h"

bool FAetherServiceRegistry::RegisterService(const TSharedRef<IAetherService>& Service)
{
    const FName ServiceName = Service->GetServiceName();
    if (ServiceName.IsNone() || Services.Contains(ServiceName))
    {
        return false;
    }

    if (!Service->Initialize())
    {
        return false;
    }

    Services.Add(ServiceName, Service);
    return true;
}

bool FAetherServiceRegistry::UnregisterService(FName ServiceName)
{
    if (TSharedRef<IAetherService>* Service = Services.Find(ServiceName))
    {
        (*Service)->Shutdown();
        Services.Remove(ServiceName);
        return true;
    }

    return false;
}

TSharedPtr<IAetherService> FAetherServiceRegistry::FindService(FName ServiceName) const
{
    const TSharedRef<IAetherService>* Service = Services.Find(ServiceName);
    return Service ? TSharedPtr<IAetherService>(*Service) : nullptr;
}

void FAetherServiceRegistry::ShutdownAll()
{
    for (TPair<FName, TSharedRef<IAetherService>>& Pair : Services)
    {
        Pair.Value->Shutdown();
    }

    Services.Reset();
}

int32 FAetherServiceRegistry::NumServices() const
{
    return Services.Num();
}
