#include "Audio/AetherAudioTypes.h"
bool FAetherAudioDefinition::IsValid() const
{
    return !AudioID.TrimStartAndEnd().IsEmpty() && Sound.ToSoftObjectPath().IsValid() &&
        FMath::IsFinite(Volume) && Volume >= 0.0f && Volume <= 10.0f &&
        FMath::IsFinite(Pitch) && Pitch > 0.0f && Pitch <= 4.0f &&
        FMath::IsFinite(MaxDistance) && MaxDistance > 0.0f && MaxDistance <= 100000.0f;
}
