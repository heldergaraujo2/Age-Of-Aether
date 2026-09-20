#include "Data/AetherClassCatalog.h"
#include "Data/AetherClassRegistry.h"
#include <initializer_list>

namespace
{
FAetherClassDefinition MakeClass(const TCHAR* Id,const TCHAR* Name,const TCHAR* Path,EAetherClassRole Role,const TCHAR* Desc)
{ FAetherClassDefinition D; D.ClassID=Id; D.DisplayName=Name; D.PathTitle=Path; D.Description=Desc; D.Role=Role; D.StartingLevel=1; return D; }
FAetherClassEvolutionDefinition MakeEvolution(const TCHAR* Id,const TCHAR* C,const TCHAR* Name,const TCHAR* Title,int32 Stage,int32 Level,const TCHAR* Desc)
{ FAetherClassEvolutionDefinition D; D.EvolutionID=Id; D.ClassID=C; D.DisplayName=Name; D.EvolutionTitle=Title; D.Description=Desc; D.Stage=Stage; D.RequiredLevel=Level; if(Stage>1) D.PrerequisiteEvolutionIDs.Add(FString::Printf(TEXT("%s.%02d"),C,Stage-1)); return D; }
void Tags(FAetherClassDefinition& D,std::initializer_list<const TCHAR*> V){for(const TCHAR* T:V)D.Tags.Add(T);}
void Tags(FAetherClassEvolutionDefinition& D,std::initializer_list<const TCHAR*> V){for(const TCHAR* T:V)D.Tags.Add(T);}
}
void FAetherClassCatalog::BuildBaseClasses(TArray<FAetherClassDefinition>& O)
{
 O.Reset();
 auto A=MakeClass(TEXT("archer"),TEXT("ARQUEIRO"),TEXT("Caminho da Precisão"),EAetherClassRole::Damage,TEXT("Combatente de longo alcance focado em precisão, mobilidade e dano crítico."));
 A.BaseStats.Values.Add(TEXT("attack"),1.00);A.BaseStats.Values.Add(TEXT("defense"),0.80);A.BaseStats.Values.Add(TEXT("speed"),1.15);A.BaseStats.Values.Add(TEXT("critical"),1.10);Tags(A,{TEXT("ranged"),TEXT("precision"),TEXT("mobility")});O.Add(A);
 auto W=MakeClass(TEXT("warrior"),TEXT("GUERREIRO"),TEXT("Caminho da Fúria"),EAetherClassRole::Damage,TEXT("Combatente de linha de frente focado em força, armas e pressão sustentada."));
 W.BaseStats.Values.Add(TEXT("attack"),1.05);W.BaseStats.Values.Add(TEXT("defense"),1.00);W.BaseStats.Values.Add(TEXT("speed"),0.95);W.BaseStats.Values.Add(TEXT("health"),1.05);Tags(W,{TEXT("melee"),TEXT("weapon-mastery"),TEXT("frontline")});O.Add(W);
 auto M=MakeClass(TEXT("mage"),TEXT("MAGO"),TEXT("Caminho do Arcano"),EAetherClassRole::Damage,TEXT("Conjurador de alto impacto baseado em alcance, área e manipulação elemental."));
 M.BaseStats.Values.Add(TEXT("attack"),1.10);M.BaseStats.Values.Add(TEXT("defense"),0.70);M.BaseStats.Values.Add(TEXT("speed"),0.90);M.BaseStats.Values.Add(TEXT("mana"),1.25);Tags(M,{TEXT("ranged"),TEXT("magic"),TEXT("area"),TEXT("elemental")});O.Add(M);
 auto T=MakeClass(TEXT("tank"),TEXT("TANK"),TEXT("Caminho da Muralha"),EAetherClassRole::Tank,TEXT("Defensor de linha de frente focado em sobrevivência, controle e proteção do grupo."));
 T.BaseStats.Values.Add(TEXT("attack"),0.80);T.BaseStats.Values.Add(TEXT("defense"),1.30);T.BaseStats.Values.Add(TEXT("speed"),0.75);T.BaseStats.Values.Add(TEXT("health"),1.35);Tags(T,{TEXT("melee"),TEXT("shield"),TEXT("defensive"),TEXT("control")});O.Add(T);
 auto H=MakeClass(TEXT("healer"),TEXT("HEALER"),TEXT("Caminho da Luz"),EAetherClassRole::Healer,TEXT("Suporte sagrado focado em cura, purificação, proteção e recuperação do grupo."));
 H.BaseStats.Values.Add(TEXT("attack"),0.75);H.BaseStats.Values.Add(TEXT("defense"),0.80);H.BaseStats.Values.Add(TEXT("speed"),0.85);H.BaseStats.Values.Add(TEXT("healing"),1.30);H.BaseStats.Values.Add(TEXT("mana"),1.20);Tags(H,{TEXT("support"),TEXT("holy"),TEXT("healing"),TEXT("cleanse")});O.Add(H);
}
void FAetherClassCatalog::BuildEvolutions(TArray<FAetherClassEvolutionDefinition>& O)
{
 O.Reset();
 auto Add=[&](const TCHAR* C,const TCHAR* N,const TCHAR* T,int32 S,int32 L,const TCHAR* D,std::initializer_list<const TCHAR*> G){auto E=MakeEvolution(*FString::Printf(TEXT("%s.%02d"),C,S),C,N,T,S,L,D);Tags(E,G);O.Add(E);};
 Add(TEXT("archer"),TEXT("Batedor"),TEXT("Caminho da Precisão"),1,1,TEXT("Iniciante rápido e leve, voltado a alcance e mobilidade."),{TEXT("ranged"),TEXT("mobility")});
 Add(TEXT("archer"),TEXT("Rastreador"),TEXT("Caminho da Precisão"),2,20,TEXT("Especialista em caça, rastreamento e armadilhas."),{TEXT("tracking"),TEXT("traps")});
 Add(TEXT("archer"),TEXT("Caçador Espectral"),TEXT("Caminho da Precisão"),3,40,TEXT("Atirador elemental que adiciona efeitos e controle às flechas."),{TEXT("elemental"),TEXT("status")});
 Add(TEXT("archer"),TEXT("Atirador Fantasma"),TEXT("Caminho da Precisão"),4,60,TEXT("Especialista em furtividade, precisão e golpes críticos."),{TEXT("stealth"),TEXT("critical")});
 Add(TEXT("archer"),TEXT("Olho de Deus"),TEXT("Juiz dos Ventos"),5,80,TEXT("Ápice do caminho: alcance extremo de vento, sempre sujeito a limites configuráveis."),{TEXT("ultimate"),TEXT("wind"),TEXT("extreme-range")});
 Add(TEXT("warrior"),TEXT("Recruta"),TEXT("Caminho da Fúria"),1,1,TEXT("Guerreiro básico com espada e escudo."),{TEXT("melee"),TEXT("shield")});
 Add(TEXT("warrior"),TEXT("Berserker"),TEXT("Caminho da Fúria"),2,20,TEXT("Troca parte da defesa por pressão ofensiva e dano."),{TEXT("rage"),TEXT("offense")});
 Add(TEXT("warrior"),TEXT("Campeão de Guerra"),TEXT("Caminho da Fúria"),3,40,TEXT("Líder de batalha com presença ofensiva e suporte de combate."),{TEXT("leader"),TEXT("battlefield")});
 Add(TEXT("warrior"),TEXT("Lorde das Lâminas"),TEXT("Caminho da Fúria"),4,60,TEXT("Mestre de armas capaz de adaptar seu estilo ao armamento."),{TEXT("weapon-mastery"),TEXT("versatility")});
 Add(TEXT("warrior"),TEXT("Avatar da Guerra"),TEXT("Caminho da Fúria"),5,80,TEXT("Ápice destrutivo, com poder sujeito a cooldowns, recursos e escalas PvP/PvE."),{TEXT("ultimate"),TEXT("destruction")});
 Add(TEXT("mage"),TEXT("Aprendiz"),TEXT("Caminho do Arcano"),1,1,TEXT("Conjurador iniciante com projéteis arcanos simples."),{TEXT("magic"),TEXT("ranged")});
 Add(TEXT("mage"),TEXT("Feiticeiro Elemental"),TEXT("Caminho do Arcano"),2,20,TEXT("Domina fogo, gelo e relâmpago."),{TEXT("fire"),TEXT("ice"),TEXT("lightning")});
 Add(TEXT("mage"),TEXT("Arquimago"),TEXT("Caminho do Arcano"),3,40,TEXT("Amplia o poder mágico e o controle de área."),{TEXT("area"),TEXT("high-power")});
 Add(TEXT("mage"),TEXT("Tecelão do Éter"),TEXT("Caminho do Arcano"),4,60,TEXT("Manipula espaço e tempo através de efeitos configuráveis."),{TEXT("time"),TEXT("space"),TEXT("control")});
 Add(TEXT("mage"),TEXT("Senhor do Caos Primordial"),TEXT("Caminho do Arcano"),5,80,TEXT("Forma máxima do arcano, com poder extremo limitado por regras configuráveis."),{TEXT("ultimate"),TEXT("chaos")});
 Add(TEXT("tank"),TEXT("Guardião"),TEXT("Caminho da Muralha"),1,1,TEXT("Defensor com escudo e alta resistência."),{TEXT("shield"),TEXT("defense")});
 Add(TEXT("tank"),TEXT("Fortaleza de Aço"),TEXT("Caminho da Muralha"),2,20,TEXT("Especialista em mitigação e proteção."),{TEXT("mitigation"),TEXT("protection")});
 Add(TEXT("tank"),TEXT("Colosso"),TEXT("Caminho da Muralha"),3,40,TEXT("Tanque pesado com presença física e atordoamentos controlados."),{TEXT("stun"),TEXT("control")});
 Add(TEXT("tank"),TEXT("Bastião Imortal"),TEXT("Caminho da Muralha"),4,60,TEXT("Defensor de emergência capaz de recuperar aliados e sobreviver a picos de dano."),{TEXT("revive"),TEXT("team-protection")});
 Add(TEXT("tank"),TEXT("Titã Ancestral"),TEXT("Caminho da Muralha"),5,80,TEXT("Ápice defensivo, com identidade de montanha viva e limites configuráveis."),{TEXT("ultimate"),TEXT("fortress")});
 Add(TEXT("healer"),TEXT("Iniciado"),TEXT("Caminho da Luz"),1,1,TEXT("Curandeiro iniciante com cura básica."),{TEXT("healing")});
 Add(TEXT("healer"),TEXT("Clérigo da Luz"),TEXT("Caminho da Luz"),2,20,TEXT("Cura em área e remove efeitos negativos."),{TEXT("area-heal"),TEXT("cleanse")});
 Add(TEXT("healer"),TEXT("Oráculo Sagrado"),TEXT("Caminho da Luz"),3,40,TEXT("Prevê janelas de dano e cria escudos preventivos."),{TEXT("shield"),TEXT("prediction")});
 Add(TEXT("healer"),TEXT("Serafim"),TEXT("Caminho da Luz"),4,60,TEXT("Suporte avançado com ressurreição e fortalecimento do grupo."),{TEXT("resurrection"),TEXT("team-buff")});
 Add(TEXT("healer"),TEXT("Avatar da Vida Eterna"),TEXT("Caminho da Luz"),5,80,TEXT("Ápice da vida: proteção extrema limitada por duração, cooldown, recursos e regras PvP/PvE."),{TEXT("ultimate"),TEXT("immortality-bounded")});
}
bool FAetherClassCatalog::BuildRegistry(FAetherClassRegistry& R,FString& E)
{
 E.Reset();R.Reset();TArray<FAetherClassDefinition>C;TArray<FAetherClassEvolutionDefinition>V;BuildBaseClasses(C);BuildEvolutions(V);
 for(const auto& D:C)if(!R.RegisterClass(D,E))return false;for(const auto& D:V)if(!R.RegisterEvolution(D,E))return false;
 TArray<FAetherClassValidationIssue>I;if(!R.Validate(I)){E=I.Num()?I[0].Message:TEXT("Catalog validation failed.");return false;}return true;
}
