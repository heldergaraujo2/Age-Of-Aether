#include "UI/AetherHUDPresentationComponent.h"
UAetherHUDPresentationComponent::UAetherHUDPresentationComponent(){PrimaryComponentTick.bCanEverTick=false;}
void UAetherHUDPresentationComponent::SetHUDVisible(bool bInVisible){bVisible=bInVisible;}