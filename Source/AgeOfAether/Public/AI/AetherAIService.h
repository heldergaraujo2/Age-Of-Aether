#pragma once
#include "CoreMinimal.h"
#include "AI/AetherAITypes.h"
#include "AI/AetherAIProvider.h"
class AGEOFAETHER_API FAetherAIService{
public:bool Initialize(const FAetherAIConfig&,TSharedPtr<IAetherAIProvider>);void Shutdown();EAetherAIResult SubmitRequest(const FAetherAIRequest&,double,FAetherAIResponse&);bool AddMemory(const FString&,const FAetherAIMemoryEntry&,double);bool GetMemory(const FString&,TArray<FAetherAIMemoryEntry>&)const;void ClearMemory(const FString&);const FAetherAIConfig&GetConfig()const{return Config;}bool IsRunning()const{return bRunning;}
private:bool ValidateRequest(const FAetherAIRequest&,FAetherAIResponse&)const;bool ValidateOutput(FAetherAIResponse&)const;bool ConsumeRequestBudget(const FString&,double);FAetherAIConfig Config;TSharedPtr<IAetherAIProvider> Provider;TMap<FString,TArray<FAetherAIMemoryEntry>> Memories;TMap<FString,TArray<double>> RequestTimes;TMap<FString,uint64> LastRequestIds;bool bRunning=false;};
