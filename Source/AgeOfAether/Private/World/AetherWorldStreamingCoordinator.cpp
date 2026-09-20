#include "World/AetherWorldStreamingCoordinator.h"
bool UAetherWorldStreamingCoordinator::PrepareStream(const FString& LinkID){if(!GetWorld())return false;const auto* Maps=GetWorld()->GetSubsystem<UAetherWorldMapSubsystem>();return Maps&&Maps->CanStreamLink(LinkID);}
bool UAetherWorldStreamingCoordinator::ActivateTarget(const FString& MapID){if(!GetWorld())return false;const auto* Maps=GetWorld()->GetSubsystem<UAetherWorldMapSubsystem>();return Maps&&Maps->SetMapActive(MapID,true);}
bool UAetherWorldStreamingCoordinator::DeactivateMap(const FString& MapID){if(!GetWorld())return false;const auto* Maps=GetWorld()->GetSubsystem<UAetherWorldMapSubsystem>();return Maps&&Maps->SetMapActive(MapID,false);}
