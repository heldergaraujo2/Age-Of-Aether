#pragma once
#include "CoreMinimal.h"
#include "Production/AetherProductionTypes.h"
class AGEOFAETHER_API FAetherProductionService{
public:bool Initialize(const FAetherProductionConfig&,double);void Shutdown(double);void BeginDrain(double);bool SetCheck(EAetherProductionCheck,bool);void RecordMetric(const FString&,double,double);void RecordEvent(const FString&,const FString&,bool,double);FAetherProductionHealth GetHealth(double)const;TArray<FAetherProductionMetric> GetMetrics()const{return Metrics;}TArray<FAetherProductionAuditEvent> GetEvents()const{return Events;}const FAetherProductionConfig&GetConfig()const{return Config;}EAetherProductionState GetState()const{return State;}
private:FAetherProductionConfig Config;EAetherProductionState State=EAetherProductionState::Stopped;TMap<EAetherProductionCheck,bool> Checks;TArray<FAetherProductionMetric> Metrics;TArray<FAetherProductionAuditEvent> Events;double StartedAt=0.;double DrainStartedAt=0.;};
