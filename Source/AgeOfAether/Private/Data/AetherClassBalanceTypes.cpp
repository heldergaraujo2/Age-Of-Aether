#include "Data/AetherClassBalanceTypes.h"
#include "Math/UnrealMathUtility.h"

namespace
{
bool Safe(double V){return FMath::IsFinite(V);}
bool ValidModifier(double V){return Safe(V) && V >= 0.0 && V <= 10.0;}
}
bool FAetherClassBalanceModifiers::IsFinite() const
{
    return Safe(Damage)&&Safe(CriticalDamage)&&Safe(Defense)&&Safe(Healing)&&Safe(Shield)&&Safe(AreaDamage)&&Safe(CrowdControl)&&Safe(CrowdControlResistance)&&Safe(MovementSpeed)&&Safe(AttackSpeed)&&Safe(CastSpeed)&&Safe(Threat)&&Safe(LifeSteal)&&Safe(Regeneration)&&Safe(BuffEffectiveness)&&Safe(DebuffEffectiveness)&&Safe(OutgoingDamage)&&Safe(IncomingDamage)&&Safe(ResourceCost)&&Safe(ResourceRecovery);
}
bool FAetherClassBalanceModifiers::IsWithinSafetyBounds(FString& E) const
{
    const double V[]={Damage,CriticalDamage,Defense,Healing,Shield,AreaDamage,CrowdControl,CrowdControlResistance,MovementSpeed,AttackSpeed,CastSpeed,Threat,LifeSteal,Regeneration,BuffEffectiveness,DebuffEffectiveness,OutgoingDamage,IncomingDamage,ResourceCost,ResourceRecovery};
    for(double X:V)if(!ValidModifier(X)){E=TEXT("Balance multiplier must be finite and within [0,10].");return false;} E.Reset(); return true;
}
FAetherClassBalanceModifiers FAetherClassBalanceModifiers::Multiply(const FAetherClassBalanceModifiers& O) const
{
    FAetherClassBalanceModifiers R;
    R.Damage=Damage*O.Damage;R.CriticalDamage=CriticalDamage*O.CriticalDamage;R.Defense=Defense*O.Defense;R.Healing=Healing*O.Healing;R.Shield=Shield*O.Shield;R.AreaDamage=AreaDamage*O.AreaDamage;R.CrowdControl=CrowdControl*O.CrowdControl;R.CrowdControlResistance=CrowdControlResistance*O.CrowdControlResistance;R.MovementSpeed=MovementSpeed*O.MovementSpeed;R.AttackSpeed=AttackSpeed*O.AttackSpeed;R.CastSpeed=CastSpeed*O.CastSpeed;R.Threat=Threat*O.Threat;R.LifeSteal=LifeSteal*O.LifeSteal;R.Regeneration=Regeneration*O.Regeneration;R.BuffEffectiveness=BuffEffectiveness*O.BuffEffectiveness;R.DebuffEffectiveness=DebuffEffectiveness*O.DebuffEffectiveness;R.OutgoingDamage=OutgoingDamage*O.OutgoingDamage;R.IncomingDamage=IncomingDamage*O.IncomingDamage;R.ResourceCost=ResourceCost*O.ResourceCost;R.ResourceRecovery=ResourceRecovery*O.ResourceRecovery;return R;
}
bool FAetherClassBalanceDefinition::IsStructurallyValid(FString& E) const
{
    if(DefinitionID.TrimStartAndEnd().IsEmpty()||ClassID.TrimStartAndEnd().IsEmpty()){E=TEXT("Balance definition requires DefinitionID and ClassID.");return false;}
    if(!EvolutionID.IsEmpty()&&EvolutionID.TrimStartAndEnd().IsEmpty()){E=TEXT("EvolutionID cannot be whitespace.");return false;}
    if(!PvE.IsWithinSafetyBounds(E))return false; if(!PvP.IsWithinSafetyBounds(E))return false; E.Reset();return true;
}
