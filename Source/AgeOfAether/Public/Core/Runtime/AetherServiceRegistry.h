#pragma once

#include "CoreMinimal.h"

class AGEOFAETHER_API IAetherService
{
public:
    virtual ~IAetherService() = default;

    virtual FName GetServiceName() const = 0;
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
};

class AGEOFAETHER_API FAetherServiceRegistry
{
public:
    bool RegisterService(const TSharedRef<IAetherService>& Service);
    bool UnregisterService(FName ServiceName);
    TSharedPtr<IAetherService> FindService(FName ServiceName) const;
    void ShutdownAll();
    int32 NumServices() const;

private:
    TMap<FName, TSharedRef<IAetherService>> Services;
};
