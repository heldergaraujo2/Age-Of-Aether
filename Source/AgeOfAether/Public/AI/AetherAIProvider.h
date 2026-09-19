#pragma once
#include "CoreMinimal.h"
#include "AI/AetherAITypes.h"
class AGEOFAETHER_API IAetherAIProvider{public:virtual~IAetherAIProvider()=default;virtual bool Generate(const FAetherAIRequest&,const FAetherAIConfig&,FAetherAIResponse&)=0;};
