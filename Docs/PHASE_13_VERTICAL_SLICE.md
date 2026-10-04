# AGE OF AETHER — FASE 13 — VERTICAL SLICE

## Objetivo

Consolidar no nível repository/source/architecture uma primeira vertical slice coerente do Age of Aether, conectando os sistemas já existentes em uma experiência única:

**assentamento → quest → exploração → combate → recompensa → dungeon → boss → retorno**

A fase não cria um novo gameplay framework. Ela cria o contrato de orquestração da slice e fixa as identidades necessárias para que a materialização posterior no Unreal possa montar a experiência sem decisões ad hoc.

Unreal runtime, importação/materialização de assets, criação de mapas, PIE, exploração real, combate real, boss real, áudio/VFX reais e validação visual permanecem explicitamente deferidos.

---

## 1. Auditoria

A auditoria confirmou que as fases anteriores já estabeleceram:

- primeira região permanente;
- zonas de Settlement, Wilderness, CombatFrontier e Dungeon;
- primeira criatura Wild Hound;
- combat service;
- adapter Creature → Combat;
- XP/progression;
- loot/reward;
- quests/events;
- primeira dungeon The Hollowed Watch;
- mini-boss/boss contracts;
- câmera isométrica;
- personagens/criaturas 2D;
- ambientes 2D;
- depth/light/shadow;
- streaming/map contracts;
- multiplayer/server authority.

O gap desta fase não era um novo sistema de gameplay.

O gap era a ausência de um **contrato único de vertical slice** que declarasse a ordem, os pontos de entrada/saída e as dependências entre esses sistemas.

---

## 2. Arquitetura criada

Foram criados:

- `Source/AgeOfAether/Public/World/AetherVerticalSliceTypes.h`
- `Source/AgeOfAether/Private/World/AetherVerticalSliceTypes.cpp`
- `Source/AgeOfAether/Public/World/AetherVerticalSliceRegistry.h`
- `Source/AgeOfAether/Private/World/AetherVerticalSliceRegistry.cpp`
- `Source/AgeOfAether/Public/World/AetherVerticalSliceCatalog.h`
- `Source/AgeOfAether/Private/World/AetherVerticalSliceCatalog.cpp`
- `Source/AgeOfAether/Public/World/AetherVerticalSliceSubsystem.h`
- `Source/AgeOfAether/Private/World/AetherVerticalSliceSubsystem.cpp`

### FAetherVerticalSliceStageDefinition

Cada etapa registra:

- StageID;
- tipo da etapa;
- nome;
- ZoneID;
- MapID;
- EntryID;
- CompletionID;
- obrigatoriedade.

### FAetherVerticalSliceDefinition

Registra:

- SliceID;
- nome;
- nível mínimo;
- settlement;
- quest inicial;
- primeira criatura;
- loot;
- dungeon;
- boss;
- quest de conclusão;
- oito etapas canônicas.

### Registry/Catalog/Subsystem

Permitem:

- registrar slices;
- localizar por ID;
- validar conteúdo;
- detectar IDs duplicados;
- validar oito etapas;
- validar unicidade das etapas;
- validar ordem canônica.

---

## 3. Vertical slice canônica

ID:

`AOA.VerticalSlice.FirstPermanent`

Nome:

**First Permanent Vertical Slice**

O source fornece uma factory canônica:

`FAetherVerticalSliceDefinition::CreateFirstPermanentSlice()`

Assim, a composição principal não depende de um asset binário Unreal para existir como contrato de produção.

---

## 4. Etapas canônicas

### 1 — Settlement

`AOA.VerticalSlice.Stage.Settlement`

Zona:

`Region.FirstPermanent.Settlement`

Entrada:

`AOA.Point.FirstRegion.SettlementEntry`

Conclusão:

`AOA.Point.FirstRegion.QuestHub`

Objetivo:

- apresentar o jogador ao assentamento;
- posicionar o Quest Hub;
- estabelecer o ponto inicial da jornada.

---

### 2 — Quest

`AOA.VerticalSlice.Stage.Quest`

Quest:

`AOA.Quest.FirstRegion.FirstHunt`

Objetivo:

- receber a primeira missão;
- criar o motivo para sair do assentamento;
- encaminhar o jogador para a fronteira.

O quest system existente permanece responsável pela missão.

---

### 3 — Exploration

`AOA.VerticalSlice.Stage.Exploration`

Zona:

`Region.FirstPermanent.Wilderness`

Entrada:

`AOA.Point.FirstRegion.ForestEntry`

Conclusão:

`AOA.Point.FirstRegion.CombatFrontier`

Objetivo:

- apresentar exploração isométrica;
- demonstrar profundidade 2D;
- apresentar ambiente;
- conduzir organicamente ao primeiro encontro.

---

### 4 — Combat

`AOA.VerticalSlice.Stage.Combat`

Criatura:

`AOA.Creature.FirstRegion.WildHound`

Spawn:

`AOA.Spawn.FirstRegion.WildHound.Frontier`

Objetivo:

- primeiro target;
- primeiro ataque;
- dano;
- Hit;
- Death;
- integração com XP/quest.

O combate permanece no `FAetherCombatService`.

---

### 5 — Reward

`AOA.VerticalSlice.Stage.Reward`

Loot:

`AOA.Loot.FirstRegion.WildHound`

Reward:

`AOA.Reward.FirstRegion.WildHound`

Objetivo:

- demonstrar a consequência do primeiro combate;
- alimentar inventory/items/progression existentes;
- atualizar a quest existente.

Nenhum reward system paralelo foi criado.

---

### 6 — Dungeon

`AOA.VerticalSlice.Stage.Dungeon`

Dungeon:

`AOA.Dungeon.FirstRegion.HollowedWatch`

Portal:

`AOA.Portal.FirstRegion.DungeonEntrance`

Objetivo:

- transicionar da região externa para o primeiro espaço fechado;
- demonstrar streaming/instance;
- continuar o loop sem quebrar a continuidade do mundo.

---

### 7 — Boss

`AOA.VerticalSlice.Stage.Boss`

Boss:

`AOA.Creature.FirstRegion.HollowedWatchWarden`

Quest:

`AOA.Quest.FirstRegion.HollowedWatch`

Objetivo:

- demonstrar escalada de desafio;
- utilizar skills/effects;
- aplicar combate existente;
- derrotar o primeiro boss;
- concluir a etapa principal da dungeon.

---

### 8 — Return

`AOA.VerticalSlice.Stage.Return`

Destino:

`Region.FirstPermanent.Settlement`

Entrada:

`AOA.Point.FirstRegion.SettlementGate`

Conclusão:

`AOA.Quest.FirstRegion.HollowedWatch`

Objetivo:

- retornar ao mundo exterior;
- concluir o arco da slice;
- preservar estado através da persistence existente.

---

## 5. Fluxo completo

O contrato final é:

**Settlement**

↓

**Quest**

↓

**Exploration**

↓

**Combat**

↓

**Reward**

↓

**Dungeon**

↓

**Boss**

↓

**Return**

Isso forma a primeira experiência coerente de RPG, sem criar um segundo gameplay stack.

---

## 6. Integração dos sistemas

### Character

Usa o personagem existente e sua apresentação 2D.

### Camera

Usa a câmera isométrica existente.

### Quest

Usa `FAetherQuestService`.

### Exploration

Usa mundo, movimento, interação, streaming e câmera existentes.

### Combat

Usa `FAetherCombatService` + `FAetherCreatureCombatAdapter`.

### Creature

Usa `UAetherCreatureSubsystem` e a apresentação 2D.

### Reward

Usa os sistemas existentes de loot/reward/inventory/progression.

### Dungeon

Usa `UAetherDungeonSubsystem` e os contratos de mapas/streaming.

### Boss

É uma criatura existente com papel Boss; não possui um combat system próprio.

### Persistence

Continua sendo responsável pelo estado persistente.

### Multiplayer

Continua seguindo a autoridade do servidor existente.

---

## 7. Identidade do conteúdo

A slice agora possui uma cadeia de IDs estável:

`AOA.VerticalSlice.FirstPermanent`

→ `AOA.Quest.FirstRegion.FirstHunt`

→ `AOA.Creature.FirstRegion.WildHound`

→ `AOA.Loot.FirstRegion.WildHound`

→ `AOA.Dungeon.FirstRegion.HollowedWatch`

→ `AOA.Creature.FirstRegion.HollowedWatchWarden`

→ `AOA.Quest.FirstRegion.HollowedWatch`

Isso elimina decisões improvisadas quando a materialização começar.

---

## 8. Direção visual

A vertical slice deve ser a primeira composição que demonstre conjuntamente a direção visual premium 2D.

Obrigatório na materialização:

- câmera isométrica consistente;
- personagens 2D de alta qualidade;
- criaturas 2D;
- arquitetura;
- vegetação;
- props;
- profundidade;
- camadas;
- parallax;
- sombras;
- iluminação;
- VFX;
- animações extremamente fluidas;
- leitura clara de combate;
- dungeon visualmente distinta;
- boss visualmente memorável.

A prioridade permanece:

**qualidade visual máxima → animação extremamente fluida → consistência artística → integração técnica → performance baseada em evidência**

---

## 9. Áudio e VFX

A slice define os pontos de integração, sem fabricar assets binários:

### Settlement

- ambience;
- UI feedback;
- NPC interaction.

### Exploration

- ambience;
- footsteps;
- environmental effects.

### Combat

- attack feedback;
- hit;
- death;
- combat VFX.

### Dungeon

- ambience próprio;
- environmental effects;
- transition feedback.

### Boss

- telegraph;
- attack VFX;
- hit/death feedback;
- reward/completion feedback.

A materialização deve utilizar a pipeline legítima de assets.

---

## 10. Multiplayer

A vertical slice permanece compatível com o modelo server-authoritative.

O servidor permanece responsável por:

- quest state;
- character state;
- combat;
- creature state;
- reward;
- dungeon progression;
- persistence.

O cliente é responsável pela apresentação.

Não foi criado nenhum caminho que permita ao cliente declarar conclusão de etapa ou recompensa por conta própria.

---

## 11. Persistência

A slice não cria save/load paralelo.

Os estados que deverão persistir posteriormente incluem:

- quest state;
- progression;
- inventory;
- rewards;
- dungeon progression/checkpoint quando aplicável;
- boss defeat quando o design exigir persistência;
- retorno ao mundo.

A integração real permanece um gate posterior.

---

## 12. O que não foi recriado

Não foram recriados:

- combat;
- progression;
- XP;
- inventory;
- items;
- loot;
- reward;
- quests;
- skills;
- creatures;
- AI;
- camera;
- world;
- streaming;
- networking;
- multiplayer;
- persistence;
- UI framework.

A fase cria apenas a **orquestração declarativa da primeira experiência completa**.

---

## 13. Validação source

A arquitetura possui validação estrutural para:

- SliceID;
- nome;
- nível mínimo;
- identidades de integração;
- oito etapas;
- IDs únicos;
- tipos de etapa únicos;
- ordem canônica;
- ZoneID;
- MapID;
- EntryID;
- CompletionID;
- catálogo;
- registry.

A factory canônica também deixa o conteúdo da primeira slice reproduzível no source.

---

## 14. Unreal — DEFERIDO

Conforme solicitado, nenhum teste de Unreal foi executado.

Não foram declarados como PASS:

- UHT;
- UBT;
- Editor;
- PIE;
- 2-client;
- Dedicated Server;
- criação/materialização dos mapas;
- exploração;
- quest;
- combate;
- XP;
- loot;
- dungeon;
- boss;
- retorno;
- HUD;
- áudio;
- VFX;
- multiplayer;
- persistence;
- qualidade visual.

Esses gates continuam pendentes para a janela posterior de validação real.

---

## 15. Conclusão

A Fase 13 está concluída no nível:

**🟩 SOURCE / REPOSITORY / ARCHITECTURE**

porque:

- a primeira experiência completa foi definida como uma única vertical slice;
- suas oito etapas estão formalizadas;
- cada etapa possui identidade, zona, mapa, entrada e conclusão;
- quest, exploração, combate, reward, dungeon, boss e retorno estão conectados;
- todos os sistemas centrais existentes foram preservados;
- o contrato possui registry/catalog/subsystem;
- existe validação estrutural;
- existe uma factory canônica da primeira slice;
- continuidade e roadmap foram atualizados;
- nenhum binário Unreal foi fabricado.

Runtime:

**🟥 DEFERIDO**

---

## Próxima fase

**FASE 14 — PRIMEIRO TESTE REAL JOGÁVEL**

Essa será a fase em que a vertical slice poderá finalmente ser materializada e percorrida no Unreal, incluindo:

**startup → assentamento → quest → exploração → combate → reward → dungeon → boss → retorno**

com testes reais de gameplay, multiplayer, server e qualidade visual.
