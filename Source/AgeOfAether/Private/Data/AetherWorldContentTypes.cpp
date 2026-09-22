#include "Data/AetherWorldContentTypes.h"

namespace
{
bool NonEmpty(const FString& S){ return !S.TrimStartAndEnd().IsEmpty(); }
bool Finite2(const FVector2D& V){ return FMath::IsFinite(V.X)&&FMath::IsFinite(V.Y); }
}

bool FAetherWorldRequirement::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(Type==EAetherWorldRequirementType::LevelAtLeast||Type==EAetherWorldRequirementType::LevelAtMost){if(Value<0){E=TEXT("Level requirement cannot be negative.");return false;}}
    else if(Type==EAetherWorldRequirementType::WorldTag){if(!NonEmpty(RequiredTag)){E=TEXT("WorldTag requirement requires RequiredTag.");return false;}}
    else if(!NonEmpty(ReferenceID)){E=TEXT("ReferenceID is required for this requirement type.");return false;}
    return true;
}
bool FAetherWorldOutcome::IsStructurallyValid(FString& E) const
{
    E.Reset();
    switch(Type)
    {
    case EAetherWorldOutcomeType::None: return true;
    case EAetherWorldOutcomeType::Teleport:
        if(!NonEmpty(TargetMapID)||!NonEmpty(TargetPointID)){E=TEXT("Teleport outcome requires TargetMapID and TargetPointID.");return false;} return true;
    case EAetherWorldOutcomeType::SetWorldTag:
    case EAetherWorldOutcomeType::ClearWorldTag:
        if(!NonEmpty(Value)){E=TEXT("World-tag outcome requires Value.");return false;} return true;
    default:
        if(!NonEmpty(ReferenceID)){E=TEXT("Outcome requires ReferenceID.");return false;} return true;
    }
}
bool FAetherStreamingCellDefinition::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(!NonEmpty(DefinitionID)){E=TEXT("Streaming cell DefinitionID is required.");return false;}
    if(!Finite2(MinBounds)||!Finite2(MaxBounds)||MinBounds.X>=MaxBounds.X||MinBounds.Y>=MaxBounds.Y){E=TEXT("Streaming cell bounds are invalid.");return false;}
    if(!FMath::IsFinite(LoadRadius)||LoadRadius<0.0f){E=TEXT("LoadRadius must be finite and non-negative.");return false;}
    return true;
}
bool FAetherWorldZoneDefinition::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(!NonEmpty(DefinitionID)){E=TEXT("Zone DefinitionID is required.");return false;}
    if(!Finite2(MinBounds)||!Finite2(MaxBounds)||MinBounds.X>=MaxBounds.X||MinBounds.Y>=MaxBounds.Y){E=TEXT("Zone bounds are invalid.");return false;}
    if(MinimumLevel<0||MaximumLevel<0||(MaximumLevel>0&&MaximumLevel<MinimumLevel)){E=TEXT("Zone level bounds are invalid.");return false;}
    return true;
}
bool FAetherWorldPointDefinition::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(!NonEmpty(DefinitionID)){E=TEXT("World point DefinitionID is required.");return false;}
    if(FMath::IsFinite(Location.X)&&FMath::IsFinite(Location.Y)&&FMath::IsFinite(Location.Z)){return true;}
    E=TEXT("World point location must be finite.");return false;
}
bool FAetherMapConnectionDefinition::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(!NonEmpty(DefinitionID)||!NonEmpty(SourcePointID)||!NonEmpty(TargetMapID)||!NonEmpty(TargetPointID)){E=TEXT("Map connection requires IDs.");return false;}
    if(MinimumLevel<0){E=TEXT("Connection MinimumLevel cannot be negative.");return false;}
    for(const auto& R:Requirements){FString X;if(!R.IsStructurallyValid(X)){E=X;return false;}}
    return true;
}
bool FAetherWorldActorPlacementDefinition::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(!NonEmpty(DefinitionID)||!NonEmpty(WorldActorID)){E=TEXT("Actor placement requires DefinitionID and WorldActorID.");return false;}
    const FVector L=Transform.GetLocation(); if(!FMath::IsFinite(L.X)||!FMath::IsFinite(L.Y)||!FMath::IsFinite(L.Z)){E=TEXT("Actor placement transform must be finite.");return false;}
    return true;
}
bool FAetherWorldInteractionDefinition::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(!NonEmpty(DefinitionID)){E=TEXT("Interaction DefinitionID is required.");return false;}
    if(NonEmpty(WorldActorID)==NonEmpty(PointID)){E=TEXT("Interaction must reference exactly one WorldActorID or PointID.");return false;}
    if(!FMath::IsFinite(InteractionRadius)||InteractionRadius<=0.0f){E=TEXT("InteractionRadius must be finite and positive.");return false;}
    if(MinimumLevel<0){E=TEXT("Interaction MinimumLevel cannot be negative.");return false;}
    for(const auto& R:Requirements){FString X;if(!R.IsStructurallyValid(X)){E=X;return false;}}
    for(const auto& O:Outcomes){FString X;if(!O.IsStructurallyValid(X)){E=X;return false;}}
    return true;
}
bool FAetherMapDefinition::IsStructurallyValid(FString& E) const
{
    E.Reset();
    if(!NonEmpty(DefinitionID)||!NonEmpty(DisplayName)){E=TEXT("Map DefinitionID and DisplayName are required.");return false;}
    if(!Finite2(MinBounds)||!Finite2(MaxBounds)||MinBounds.X>=MaxBounds.X||MinBounds.Y>=MaxBounds.Y){E=TEXT("Map bounds are invalid.");return false;}
    if(RecommendedMinimumLevel<0||RecommendedMaximumLevel<0||(RecommendedMaximumLevel>0&&RecommendedMaximumLevel<RecommendedMinimumLevel)){E=TEXT("Map level bounds are invalid.");return false;}
    if(StreamingCellSize<=0||MaxStreamingCellCount<=0){E=TEXT("Streaming cell size/count must be positive.");return false;}
    if(StreamingCells.Num()>MaxStreamingCellCount){E=TEXT("Streaming cell count exceeds MaxStreamingCellCount.");return false;}
    for(const auto& C:StreamingCells){FString X;if(!C.IsStructurallyValid(X)){E=X;return false;}}
    for(const auto& Z:Zones){FString X;if(!Z.IsStructurallyValid(X)){E=X;return false;}}
    for(const auto& P:Points){FString X;if(!P.IsStructurallyValid(X)){E=X;return false;}}
    for(const auto& C:Connections){FString X;if(!C.IsStructurallyValid(X)){E=X;return false;}}
    for(const auto& A:ActorPlacements){FString X;if(!A.IsStructurallyValid(X)){E=X;return false;}}
    for(const auto& I:Interactions){FString X;if(!I.IsStructurallyValid(X)){E=X;return false;}}
    return true;
}
