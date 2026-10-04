# AGE OF AETHER — FASE 12 — PRIMEIRA DUNGEON

## Objetivo

Estabelecer no nível repository/source/architecture a primeira dungeon permanente do Age of Aether, integrada ao mundo contínuo, ao sistema de mapas/streaming existente, às criaturas, ao combate, às quests, ao loot/reward e à apresentação premium 2D.

A fase não cria um segundo sistema de mundo, combate, quest, loot ou progression.

O Unreal runtime, a criação do mapa .umap, importação de assets, navigation, collision, exploração jogável e validação visual permanecem explicitamente deferidos.

## 1. Auditoria

A arquitetura existente já possui zonas de mundo incluindo Dungeon, portais/transições, mapas Persistent/Streamed/Instance, registry/catalog/subsystem de mapas, streaming links, criaturas/spawns, combat service, skills/effects, quests, loot/reward, persistence, multiplayer/networking e apresentação 2D.

Não existia um contrato dedicado de dungeon. Esse foi o gap real identificado.

## 2. Arquitetura criada

Foram criados:

- Source/AgeOfAether/Public/World/AetherDungeonTypes.h
- Source/AgeOfAether/Private/World/AetherDungeonTypes.cpp
- Source/AgeOfAether/Public/World/AetherDungeonRegistry.h
- Source/AgeOfAether/Private/World/AetherDungeonRegistry.cpp
- Source/AgeOfAether/Public/World/AetherDungeonCatalog.h
- Source/AgeOfAether/Private/World/AetherDungeonCatalog.cpp
- Source/AgeOfAether/Public/World/AetherDungeonSubsystem.h
- Source/AgeOfAether/Private/World/AetherDungeonSubsystem.cpp

### FAetherDungeonDefinition

Define DungeonID, DisplayName, ZoneID, MapID, EntryPortalID, EntranceRoomID, ExitRoomID, BossRoomID, CompletionQuestID, LootTableID, RewardDefinitionID, MinimumLevel, RecommendedLevel, Rooms e enabled state.

### FAetherDungeonRoomDefinition

Define RoomID, DisplayName, RoomType, próximas salas, encounters, opcionalidade e checkpoint.

### FAetherDungeonEncounterDefinition

Define EncounterID, tipo Creature/EliteCreature/Boss/TriggeredEvent, TargetID, RequiredCount e se é obrigatório para progressão.

### Registry/Catalog/Subsystem

Permitem registrar, localizar e validar dungeons, detectar IDs duplicados, validar referências de salas e expor o catálogo ao WorldSubsystem.

## 3. Primeira dungeon canônica

**The Hollowed Watch — A Torre Vigia Oca**

IDs:

- DungeonID: AOA.Dungeon.FirstRegion.HollowedWatch
- ZoneID: Region.FirstPermanent.Dungeon
- MapID: AOA.Map.FirstRegion.HollowedWatch
- EntryPortalID: AOA.Portal.FirstRegion.DungeonEntrance
- CompletionQuestID: AOA.Quest.FirstRegion.HollowedWatch
- LootTableID: AOA.Loot.FirstRegion.HollowedWatch
- RewardDefinitionID: AOA.Reward.FirstRegion.HollowedWatch

É a primeira área fechada permanente da primeira região, não um mapa descartável.

## 4. Composição espacial

Salas canônicas:

1. HollowedWatch.Entrance — transição segura, orientação e checkpoint inicial.
2. HollowedWatch.Vestibule — apresentação arquitetônica, descoberta e primeira bifurcação.
3. HollowedWatch.BrokenGallery — exploração, obstáculo e primeiro encontro.
4. HollowedWatch.AbandonedBarracks — combate, props, storytelling e rota secundária.
5. HollowedWatch.Crypt — aumento de tensão, encontro mais perigoso e mecanismo.
6. HollowedWatch.GuardianHall — elite/mini-boss, checkpoint e recompensa intermediária.
7. HollowedWatch.LowerTower — exploração final e conexão com a arena.
8. HollowedWatch.BossChamber — boss e recompensa principal.
9. HollowedWatch.Exit — conclusão e retorno ao mundo externo.

## 5. Grafo

Rota principal:

Entrance → Vestibule → BrokenGallery → AbandonedBarracks → Crypt → GuardianHall → LowerTower → BossChamber → Exit

Rotas secundárias:

- Vestibule → AbandonedBarracks
- BrokenGallery → Crypt
- AbandonedBarracks → LowerTower

A validação source exige que todas as referências NextRoomIDs apontem para salas existentes.

## 6. Criaturas e encontros

A dungeon reutiliza o sistema de criaturas existente.

Primeiro encontro de referência:

AOA.Creature.FirstRegion.WildHound

Mini-boss:

AOA.Creature.FirstRegion.HollowedWatchGuardian

Boss:

AOA.Creature.FirstRegion.HollowedWatchWarden

Os três usam CreatureDefinition/CreatureSubsystem e a apresentação 2D existente. O combate continua passando pelo FAetherCombatService.

Nenhuma arquitetura paralela de dano ou boss foi criada.

## 7. Quest, loot e recompensa

Quest:

AOA.Quest.FirstRegion.HollowedWatch

Loot:

AOA.Loot.FirstRegion.HollowedWatch

Reward:

AOA.Reward.FirstRegion.HollowedWatch

A dungeon referencia esses contratos; não implementa novos sistemas.

Fluxo-alvo:

boss defeated → reward/loot existente → inventory/item existente → quest completion → persistence existente → saída

## 8. Checkpoint

Checkpoint principal:

HollowedWatch.GuardianHall

A materialização posterior deverá conectá-lo ao persistence/respawn existente. Não foi criado save system paralelo.

## 9. Mundo e streaming

Configuração conceitual:

- MapID = AOA.Map.FirstRegion.HollowedWatch
- LoadMode = Instance
- ZoneID = Region.FirstPermanent.Dungeon
- bServerRequired = true
- bClientPresentation = true

Entrada:

CombatFrontier → DungeonApproach → DungeonEntrance → Hollowed Watch

Saída:

Hollowed Watch → DungeonExit → FirstPermanent

Nenhum .umap foi fabricado.

## 10. Interação e obstáculos

A dungeon usa os sistemas existentes de Interaction para portas, mecanismos, baús, alavancas, objetos de quest, atalhos e elementos de puzzle.

A dungeon referencia IDs e contratos existentes; não cria lógica de interação paralela.

## 11. Apresentação 2D

A dungeon segue a mesma linguagem visual premium 2D:

- background/far/world/midground/foreground/overlay;
- parallax;
- depth offsets;
- translucency sorting;
- shadows;
- iluminação;
- partículas;
- VFX;
- arquitetura detalhada;
- props contextualizados;
- criaturas e boss visualmente coerentes.

A diretriz permanente continua sendo:

**qualidade visual máxima → animação extremamente fluida → consistência artística → integração técnica → performance baseada em evidência**

Nenhuma imagem ou asset externo é considerado asset Unreal integrado antes da materialização legítima.

## 12. Multiplayer e autoridade

Entrada/transição, estado da dungeon, combate, criaturas, rewards e persistence permanecem autoritativos.

A apresentação 2D é visual.

Dedicated Server não deve carregar conteúdo puramente visual.

A prova real de multiplayer fica para a janela posterior de Unreal.

## 13. O que não foi recriado

- combat
- progression/XP
- inventory
- items
- loot
- reward
- quest
- skills
- creatures
- AI
- networking
- multiplayer
- persistence
- world streaming

A nova camada é exclusivamente o contrato de dungeon que conecta esses sistemas.

## 14. Validação source

A arquitetura possui validação estrutural para:

- DungeonID;
- DisplayName;
- ZoneID;
- MapID;
- portal;
- entrance;
- exit;
- boss;
- níveis;
- existência de salas;
- IDs duplicados;
- encounters;
- referências de salas;
- catálogo;
- registry.

## 15. Unreal — DEFERIDO

Não foram declarados como PASS UHT, UBT, Editor, PIE, 2-client PIE, Dedicated Server, importação real, criação do .umap, collision, navigation, exploração, entrada/saída, combate, mini-boss, boss, loot, quest, persistence, multiplayer ou qualidade visual.

Esses gates permanecem pendentes por decisão operacional.

## 16. Conclusão

A Fase 12 está concluída no nível:

**🟩 SOURCE / REPOSITORY / ARCHITECTURE**

porque a arquitetura dedicada de dungeon foi criada, a primeira dungeon possui identidade permanente, composição e grafo definidos, encontros/mini-boss/boss possuem contratos, entrada e saída usam o mundo existente, quests/loot/rewards usam contratos existentes, checkpoints apontam para persistence, streaming usa a infraestrutura existente, a apresentação segue a pipeline 2D e nenhuma arquitetura central foi duplicada.

Runtime:

**🟥 DEFERIDO**

## Próxima fase

**FASE 13 — VERTICAL SLICE**

Fluxo-alvo:

**assentamento → quest → exploração → combate → XP/loot → dungeon → boss → retorno**

A vertical slice deverá juntar as peças já definidas sem reconstruir os sistemas centrais.
