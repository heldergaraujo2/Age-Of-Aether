#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/Runtime/AetherRuntimeTypes.h"
#include "Core/Runtime/AetherServerClock.h"
#include "Core/Runtime/AetherScheduler.h"
#include "Core/Runtime/AetherServiceRegistry.h"
#include "Core/Runtime/AetherRuntimeConfig.h"

#include "AetherRuntimeSubsystem.generated.h"

UCLASS()
class AGEOFAETHER_API UAetherRuntimeSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Runtime")
    EAetherRuntimeState GetRuntimeState() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Runtime")
    EAetherHealthState GetHealthState() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Runtime")
    double GetServerUptimeSeconds() const;

    UFUNCTION(BlueprintCallable, Category = "Age of Aether|Runtime")
    FAetherRuntimeResult RequestShutdown();

    FAetherServerClock& GetServerClock();
    FAetherScheduler& GetScheduler();

private:
    bool TickRuntime(float DeltaTime);
    FAetherRuntimeResult TransitionTo(EAetherRuntimeState NewState);
    void SetHealth(EAetherHealthState NewHealth);

    EAetherRuntimeState RuntimeState = EAetherRuntimeState::Uninitialized;
    EAetherHealthState HealthState = EAetherHealthState::Unknown;
    FAetherServerClock ServerClock;
    FAetherScheduler Scheduler;
    FAetherServiceRegistry ServiceRegistry;
    FTSTicker::FDelegateHandle TickerHandle;
    FAetherRuntimeConfig RuntimeConfig;
};
