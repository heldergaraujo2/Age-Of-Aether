# AGE OF AETHER — ROADMAP VISUAL & PLAYABLE
## Unreal Engine 5.8.1 • Implementação Visual, Jogável e Runtime

Repository: https://github.com/heldergaraujo2/Age-Of-Aether
Base: Phases 0–36 implemented at repository/source level.
Purpose: canonical execution plan for turning the validated gameplay architecture into the actual visual and playable Unreal game.

## 0. GATE DE INÍCIO

Este roadmap começa somente depois que os testes técnicos do Unreal Engine 5.8.1 forem executados localmente.

Gate obrigatório:
- UHT PASS
- UBT Editor PASS
- UBT Game PASS
- Dedicated Server build PASS
- Editor abre sem erro crítico
- mapa de teste abre
- PIE 1 cliente PASS
- PIE servidor + cliente PASS
- Automation Framework PASS
- replicação básica PASS
- logs sem erros críticos
- clean build reproduzível

Se houver falha estrutural, corrigir a fundação antes da Fase 37.

## 1. PRINCÍPIO

C++ = regras e autoridade.
Data = conteúdo.
Blueprint = composição e apresentação.
Assets = aparência.
Unreal = runtime.

Fluxo:
Input → Client Request → Server Validation → Authoritative Service → State Mutation → Persistence → Replication → Presentation → UI/Animation/VFX/Audio.

O cliente nunca é autoridade sobre dano, XP, inventário, moeda, classe, evolução, cooldown, loot, quest, recompensa ou resultado de combate.

## 2. SEQUÊNCIA OFICIAL

37 Fundação Visual
38 Pipeline de Assets
39 Personagem Base Jogável
40 Input, Movimento e Câmera
41 Animação
42 Equipamentos e Visual dos Itens
43 Classes e 25 Evoluções na Apresentação
44 Combate Jogável
45 Skills, Buffs, Debuffs e VFX
46 Monstros, NPCs e Bosses
47 Mundo, Mapas e Streaming
48 Interação, NPC, Quest e Eventos
49 Inventário, Loot e Equipamento Jogável
50 Crafting, Forge, Economia e Shops
51 UI/UX MMORPG Completa
52 Áudio e Ambiência
53 Multiplayer, Dedicated Server e Persistência
54 Performance, Streaming e Escala
55 Conteúdo Inicial Completo
56 Alpha, Beta e Release Candidate

A ordem existe para reduzir retrabalho.

---

# PHASE 37 — UNREAL VISUAL FOUNDATION

Objetivo: transformar a fundação técnica em uma base Unreal pronta para conteúdo.

Implementar:
- GameInstance, GameMode, GameState, PlayerController e PlayerState;
- Character/Pawn;
- câmera inicial;
- Enhanced Input;
- mapa de desenvolvimento;
- Game Default Map;
- collision channels/profiles;
- Asset Manager;
- Primary Asset Types;
- Data Assets conectados aos registries;
- estrutura Content/Aether/Core, Characters, Classes, Items, Monsters, NPCs, Maps, UI, Animations, VFX, Audio, Materials, Textures e Testing;
- convenções de nomes;
- mapas de teste;
- logging visual;
- debug widgets.

Aceitação:
um personagem placeholder entra no mapa, recebe input, câmera funciona e o servidor controla sua existência.

---

# PHASE 38 — ASSET PIPELINE REAL

Objetivo: estabelecer o processo definitivo de FBX/source até runtime.

Pipeline:
FBX/source → Unreal Import → Mesh/Skeleton/Material → UAsset → Asset Registry → Stable AssetID → Presentation Registry → Runtime.

Implementar:
- import settings;
- escala/orientação;
- naming;
- skeleton compatibility;
- Physics Asset;
- LOD;
- collision;
- materiais e texturas;
- sockets;
- Asset Manager;
- fallback;
- validação de dependências;
- checklist de asset;
- documentação do pipeline;
- registro de origem/licença;
- versão do asset.

Cada asset deve possuir:
AssetID, tipo, origem, versão, dependências, uso, fallback e status de validação.

Regra: não criar referências para assets inexistentes.

---

# PHASE 39 — PERSONAGEM BASE JOGÁVEL

**Status:** COMPLETE at repository/source level; Unreal 5.8.1 visual/runtime acceptance pending local execution.

Implementado:
- visual profile Data Asset para Skeletal Mesh, Animation Blueprint e materiais;
- visual component reutilizável integrado diretamente ao AAetherCharacter;
- validação de perfil antes da aplicação;
- proteção para não carregar apresentação no Dedicated Server;
- testes de integração do componente;
- preservação da cápsula, movimento, câmera, CharacterID e replicação já existentes.

Workflow visual simplificado:
1. importar um FBX real;
2. criar um AetherPlayableCharacterVisualProfile;
3. apontar para o Skeletal Mesh;
4. apontar para o Animation Blueprint quando disponível;
5. opcionalmente aplicar materiais;
6. atribuir o profile ao Visual Component;
7. Play.

Aceitação local pendente:
- skeleton/Physics Asset reais;
- Character Blueprint/instância no mapa;
- movimento/câmera/jump;
- PIE;
- 2-client PIE;
- Dedicated Server + client;
- Automation;
- logs.

Detalhamento: Docs/PHASE_39_PLAYABLE_BASE_CHARACTER.md.


---

# PHASE 40 — INPUT, MOVIMENTO E CÂMERA

Implementar:
- teclado;
- mouse;
- controle;
- remapeamento;
- sensibilidade;
- câmera livre;
- câmera de combate;
- movimento relativo à câmera;
- corrida;
- interação;
- hotkeys;
- skill shortcuts;
- camera collision;
- limites;
- correção de movimento multiplayer.

Validar:
- servidor autoritativo;
- nenhuma posição arbitrária do cliente vira verdade;
- correção de movimento aceitável;
- convergência cliente/servidor.

---

# PHASE 41 — SISTEMA DE ANIMAÇÃO

Implementar:
- idle;
- walk;
- run;
- jump/fall/land;
- attack;
- hit;
- death;
- cast;
- interact;
- gather;
- equip/unequip;
- emotes;
- Animation Blueprint;
- State Machines;
- Blend Spaces;
- Montages;
- Notifies;
- sockets.

Regra:
Animation Notify pode sincronizar apresentação, mas não concede autoridade de gameplay.

---

# PHASE 42 — EQUIPAMENTOS E VISUAL DOS ITENS

Pipeline:
ItemDefinition → VisualDefinition → AssetID → Mesh/Material → Socket → Character.

Implementar:
- main hand;
- off hand;
- head;
- chest;
- legs;
- hands;
- feet;
- back;
- rings;
- necklace;
- mounts/pets quando aplicável;
- sockets;
- ocultação de partes;
- material overrides controlados;
- preview;
- troca de equipamento;
- armas;
- escudos;
- asas/capas quando fizerem parte do conteúdo.

Regra:
primeiro servidor aceita o equipamento; depois o cliente apresenta.

---

# PHASE 43 — CINCO CLASSES E 25 EVOLUÇÕES

Classes:
- ARQUEIRO;
- GUERREIRO;
- MAGO;
- TANK;
- HEALER.

Cada uma possui 5 evoluções já definidas.

Implementar:
- seleção de classe;
- seleção de evolução;
- ícones;
- modelos;
- armaduras;
- armas;
- animações;
- VFX;
- SFX;
- preview;
- descrição;
- requisitos;
- bloqueio/desbloqueio;
- transição visual;
- apresentação dos bônus.

Aceitação:
a mudança de evolução altera a apresentação correspondente sem alterar a autoridade do servidor.

---

# PHASE 44 — COMBATE JOGÁVEL

Implementar:
- targeting;
- seleção de alvo;
- ataque básico;
- range;
- facing;
- hit reaction;
- damage numbers;
- health bars;
- death;
- respawn;
- critical presentation;
- shield presentation;
- cooldown feedback;
- combate PvE;
- combate PvP;
- cancelamento/interrupção quando suportados.

Fluxo:
Input → request → server validation → authoritative calculation → result → replication → animation/VFX/UI.

Nunca calcular dano no cliente como autoridade.

---

# PHASE 45 — SKILLS, BUFFS, DEBUFFS E VFX

Implementar:
- cast animation;
- projectile;
- beam;
- area effects;
- impact;
- ground effects;
- buff aura;
- debuff visual;
- heal;
- shield;
- crowd control;
- dispel;
- status icons;
- cooldown;
- cast bar;
- resource feedback;
- VFX;
- SFX.

Regra:
o VFX representa o resultado; não determina o resultado.

---

# PHASE 46 — MONSTROS, NPCs E BOSSES

Monstros:
- mesh;
- skeleton;
- locomotion;
- attacks;
- hit/death;
- AI;
- health bar;
- nameplate;
- loot;
- respawn;
- effects;
- audio.

NPCs:
- mesh;
- idle/walk;
- interaction;
- nameplate;
- dialogue;
- shop;
- quest;
- crafting;
- services.

Bosses:
- arena;
- phases;
- telegraphs;
- attacks;
- mechanics;
- enrage quando definido;
- death;
- rewards;
- VFX/SFX.

---

# PHASE 47 — MUNDO, MAPAS E STREAMING

Ordem:
1. mapa de testes;
2. cidade inicial;
3. área inicial;
4. campo;
5. dungeon;
6. área elite;
7. boss arena;
8. PvP/evento;
9. conexões;
10. expansão.

Implementar:
- World Partition;
- Landscape;
- foliage;
- buildings;
- roads;
- water;
- lighting;
- sky/atmosphere;
- collision;
- NavMesh;
- Data Layers;
- HLOD;
- streaming;
- portals;
- teleports;
- spawns;
- safe zones;
- PvP zones;
- dungeons.

Regra:
primeiro gameplay blockout, depois decoração e polimento.

---

# PHASE 48 — INTERAÇÃO, NPC, QUEST E EVENTOS

Implementar:
- interaction trace;
- prompt;
- NPC interaction;
- dialogue;
- choices;
- quest accept/progress/complete;
- reward presentation;
- world events;
- announcements;
- objective markers;
- map markers;
- quest tracker.

UI sempre reflete estado autoritativo.

---

# PHASE 49 — INVENTÁRIO, LOOT E EQUIPAMENTO JOGÁVEL

Implementar:
- inventory UI;
- grid;
- drag/drop;
- stack;
- split;
- move;
- equip/unequip;
- tooltip;
- comparison;
- pickup;
- world loot;
- loot window;
- destruction;
- binding;
- durability;
- enhancement presentation.

Fluxo:
World Drop → server ownership → pickup request → validation → inventory mutation → persistence → UI → visual.

---

# PHASE 50 — CRAFTING, FORGE, ECONOMIA E SHOPS

Implementar:
- crafting station;
- recipe list;
- requirements;
- ingredient preview;
- success presentation;
- result;
- failure presentation;
- forge;
- enhancement;
- NPC shop;
- buy/sell;
- currency;
- transaction result;
- confirmation dialogs.

A UI não calcula o resultado final.

---

# PHASE 51 — UI/UX MMORPG COMPLETA

Telas:
- Login;
- Character Select/Create;
- Loading;
- HUD;
- Character;
- Inventory;
- Equipment;
- Skills;
- Class/Evolution;
- Craft;
- NPC/Shop;
- Quests;
- Map;
- Party;
- Guild;
- Friends;
- Chat;
- Notifications;
- Settings;
- Help;
- Death;
- Respawn;
- Reconnect/Error.

HUD:
HP, resource, XP, level, target, buffs, debuffs, skill bar, quests, minimap, notifications, chat, party e boss bar.

Validar:
mouse/keyboard/controller, resoluções, escala, acessibilidade básica, loading states, erros e confirmações destrutivas.

---

# PHASE 52 — ÁUDIO E AMBIÊNCIA

Implementar:
- música;
- ambiente;
- passos;
- ataques;
- impactos;
- skills;
- UI;
- NPC;
- monstros;
- bosses;
- cidade;
- dungeon;
- água;
- vento;
- eventos;
- transições;
- volume por categoria;
- mute;
- spatial audio quando aplicável.

Áudio segue o estado do jogo e não cria autoridade.

---

# PHASE 53 — MULTIPLAYER, DEDICATED SERVER E PERSISTÊNCIA

Cenários:
- 1 cliente;
- 2 clientes;
- 4 clientes;
- múltiplos jogadores;
- Dedicated Server;
- reconnect;
- logout;
- morte;
- loot simultâneo;
- party;
- guild;
- combate simultâneo;
- troca de equipamentos;
- crafting simultâneo;
- eventos;
- mudança de mapa.

Persistir e restaurar:
- personagem;
- level/XP;
- classe/evolução;
- stats;
- inventory;
- equipment;
- enhancement;
- quests;
- moeda;
- social;
- world state necessário.

Falhas obrigatórias:
- disconnect durante combate;
- loot;
- craft;
- enhancement;
- restart do servidor;
- save atrasado;
- reconnect;
- estado inválido.

---

# PHASE 54 — PERFORMANCE, STREAMING E ESCALA

Medir:
- FPS;
- frame time;
- Game Thread;
- Render Thread;
- GPU;
- memória;
- streaming;
- draw calls;
- skeletal mesh;
- animation;
- VFX;
- audio;
- network bandwidth;
- actor count;
- replication;
- server tick;
- Dedicated Server CPU.

Otimizar conforme profiling:
- LOD;
- HLOD;
- culling;
- Nanite quando apropriado;
- texture streaming;
- virtual textures quando apropriado;
- animation optimization;
- VFX scalability;
- relevancy;
- replication frequency;
- dormancy;
- pooling;
- World Partition;
- Data Layers.

Nenhuma otimização importante deve ser baseada apenas em suposição.

---

# PHASE 55 — CONTEÚDO INICIAL COMPLETO

Pacote mínimo:

Jogadores:
- 5 classes;
- 25 evoluções;
- equipamentos iniciais;
- skills iniciais;
- progressão.

Mundo:
- cidade inicial;
- mapa inicial;
- áreas de combate;
- dungeon;
- boss arena.

PvE:
- monstros comuns;
- elites;
- boss;
- drops;
- XP;
- respawns.

NPCs:
- NPC inicial;
- quest giver;
- shop;
- crafting;
- serviços.

Sistemas:
- inventory;
- equipment;
- skills;
- combat;
- quest;
- crafting;
- loot;
- social;
- chat;
- party;
- guild;
- persistence.

Ciclo mínimo:
Criar personagem → escolher classe → entrar no mundo → explorar → combater → ganhar XP → receber loot → equipar → usar skills → fazer quest → craftar → enfrentar conteúdo maior → evoluir → salvar → sair → reconectar → continuar.

---

# PHASE 56 — ALPHA, BETA E RELEASE CANDIDATE

Alpha:
- sistemas principais;
- fluxo completo;
- crashes;
- bugs críticos;
- segurança;
- persistence;
- multiplayer;
- Dedicated Server.

Beta:
- maior volume;
- conteúdo;
- balanceamento;
- stress;
- performance;
- economia;
- exploit testing;
- long sessions;
- reconnect.

Release Candidate:
- congelar schema;
- IDs;
- versões;
- builds;
- conteúdo aprovado;
- configurações de produção;
- clean build;
- package;
- Dedicated Server;
- migration;
- backup/restore;
- smoke tests;
- regression suite;
- multiplayer acceptance;
- security acceptance;
- performance acceptance.

Somente depois:
Release Candidate → Production.

---

# 3. CHECKLIST OBRIGATÓRIO DE CADA FASE

Cada fase 37–56 deve registrar:

STATUS
IMPLEMENTADO
COMPILADO
TESTADO
VALIDADO
FALHAS
PENDÊNCIAS
RISCOS
PRÓXIMO PASSO

Nenhuma fase pode ser concluída apenas por documentação.

---

# 4. REGRA DE ASSETS

Antes de criar um asset:
Asset → finalidade → AssetID → consumidor → mapa/character/UI → teste.

Isso evita produzir assets sem integração.

---

# 5. CHECKLIST DE FBX

1. escala;
2. orientação;
3. origem;
4. polígonos;
5. UV;
6. materiais;
7. texturas;
8. skeleton;
9. import;
10. Physics Asset;
11. LOD;
12. sockets;
13. AssetID;
14. definição;
15. Blueprint;
16. runtime;
17. multiplayer;
18. performance;
19. aprovação.

---

# 6. NOVO ITEM

ItemDefinition + VisualAsset + Icon + Animation/VFX/SFX quando necessários
→ validation
→ registry
→ server
→ inventory/equipment
→ replication
→ presentation.

Criar item normal não deve exigir alterar C++ de inventário.

---

# 7. NOVA CLASSE/EVOLUÇÃO

ClassDefinition + BalanceDefinition + Skills + Equipment Rules + Presentation + Assets + Validation.

A arquitetura deve permitir uma sexta classe sem reconstruir o sistema.

---

# 8. NOVO MONSTRO

MonsterDefinition + AI Profile + Mesh + Animations + Skills + DropTable + Respawn + VFX/SFX + World Spawn.

---

# 9. NOVA QUEST

QuestDefinition + Objectives + NPC + Dialogue + World Marker + Rewards + Presentation.

---

# 10. NOVO MAPA

MapDefinition + World Partition + Landscape + Gameplay Zones + Spawn Groups + NPCs + Monsters + Portals + Quests + Events + NavMesh + HLOD + Lighting + Audio + Validation.

---

# 11. DEFINITION OF DONE — VISUAL

Um asset visual está pronto quando:
- importa;
- dependências são válidas;
- possui AssetID;
- resolve no registry;
- aparece no runtime;
- funciona no sistema;
- não gera erro;
- fallback existe quando necessário;
- custo de performance é conhecido.

# 12. DEFINITION OF DONE — JOGÁVEL

Um sistema está pronto quando:
- funciona em PIE;
- funciona em multiplayer PIE;
- funciona no Dedicated Server quando aplicável;
- servidor permanece autoritativo;
- estado replica;
- UI está correta;
- persistence funciona quando aplicável;
- reconnect foi testado;
- falhas não corrompem estado;
- regression tests passam;
- performance está dentro do orçamento.

---

# 13. ORDEM DE PRODUÇÃO DE ASSETS

Primeiro:
- personagem base;
- skeleton;
- animações fundamentais;
- mapa blockout;
- UI base.

Segundo:
- equipamentos;
- monstros;
- NPCs;
- armas;
- skills.

Terceiro:
- ambientes;
- VFX;
- áudio;
- decoração.

Quarto:
- polimento;
- variações;
- cosméticos;
- detalhes.

---

# 14. PRIMEIRO VERTICAL SLICE

Antes de multiplicar conteúdo, concluir um pequeno trecho totalmente funcional:

1 personagem
1 classe
1 evolução
1 arma
1 armadura
1 mapa pequeno
3 monstros
1 NPC
1 quest
3 skills
1 loot table
1 receita
1 boss
UI essencial
áudio básico
Dedicated Server
persistence
reconnect

Este vertical slice é o laboratório oficial do pipeline.

---

# 15. CICLO DE PRODUÇÃO

Importar → Integrar → Testar → Corrigir → Testar novamente → Validar → Documentar → Multiplicar.

Nunca produzir centenas de assets antes de validar o pipeline.

---

# 16. PORTÃO FINAL

O jogo só será considerado visualmente/jogavelmente pronto quando em Unreal 5.8.1 funcionar:

Launcher/Client
→ Login
→ Character Select
→ Character
→ Class
→ Evolution
→ World
→ Movement
→ Camera
→ NPC
→ Quest
→ Combat
→ Skills
→ Monster
→ Loot
→ Inventory
→ Equipment
→ Crafting
→ Social
→ Party/Guild
→ Map Transition
→ Persistence
→ Logout
→ Reconnect
→ Dedicated Server.

Além disso:
- sem autoridade crítica no cliente;
- sem corrupção de inventário;
- sem duplicação simples de itens;
- sem perda de progresso em reconnect normal;
- sem assets quebrados;
- sem referências inválidas;
- sem crashes críticos conhecidos;
- performance medida;
- build reproduzível;
- regression tests passando.

---

# 17. REGRA DE EXECUÇÃO

Cada /goal deve executar uma fase completa por vez:

1. auditar;
2. implementar;
3. testar;
4. corrigir;
5. testar novamente;
6. validar no Unreal quando disponível;
7. atualizar documentação;
8. atualizar ROADMAP.md;
9. atualizar PROJECT_MEMORY/00_CONTINUITY.md;
10. registrar resultado;
11. somente então avançar.

---

# 18. ESTADO INICIAL

Phases 0–36: fundação, conteúdo, classes, cliente e presentation contracts implementados no repositório.

Unreal runtime gate: ainda precisa ser executado localmente.

Phases 37–56: programa oficial de implementação visual e jogável.

Próximo alvo depois do gate:
PHASE 37 — UNREAL VISUAL FOUNDATION.

---

# 19. VISÃO FINAL

O objetivo é transformar a arquitetura existente em um MMORPG completo mantendo:

REGRAS = C++
CONTEÚDO = Data
APARÊNCIA = Assets
APRESENTAÇÃO = Blueprint/Unreal
AUTORIDADE = Server
PERSISTÊNCIA = Backend/Repository
VALIDAÇÃO = Automation + Unreal Runtime

Assim, o crescimento do Age of AetheR será principalmente mais conteúdo, assets, mapas e experiências — e não reescrita do motor a cada novo conteúdo.


## PHASE 40 — INPUT, MOVEMENT & CAMERA — COMPLETE AT REPOSITORY/SOURCE LEVEL

Implemented:
- optional AetherMovementCameraProfile Data Asset;
- safe validation for movement/camera ranges;
- configurable walk/sprint/jump/rotation values;
- configurable camera zoom range/step and pitch limits;
- W/A/S/D movement, mouse look, Space jump, Left Shift sprint, Mouse Wheel zoom;
- server-authoritative sprint transition through a boolean-only RPC;
- server chooses sprint speed from trusted profile/fallback;
- camera pitch clamping;
- Phase 40 Automation coverage;
- character README and phase documentation updated.

Low-bureaucracy workflow:
1. keep the existing runtime-created Enhanced Input foundation;
2. optionally create one movement/camera profile;
3. tune values in the editor;
4. place the character and press Play.

No per-character input C++ is required.

Validation truth:
- repository/source implementation complete;
- Unreal 5.8.1 UHT/UBT/PIE/2-client/Dedicated Server runtime remains NOT VERIFIED here;
- no fake .uasset/.umap assets added.

Detailed report:
Docs/PHASE_40_INPUT_MOVEMENT_CAMERA.md

Next target:
**PHASE 41 — ANIMATION SYSTEM**


## Phase 42 — Equipment & Item Visuals — COMPLETE at repository/source level
- Added data-driven equipment visual profile and reusable equipment visual component.
- Supports skeletal/static meshes, slots, sockets, transforms and material overrides.
- Integrated the component into AAetherCharacter without changing gameplay authority.
- Added Automation tests and Phase 42 documentation.
- No fake Unreal binaries were committed.
- Unreal 5.8.1 runtime import/attachment/PIE/Dedicated Server remains a local acceptance gate.
- Next target: Phase 43 — Five Classes & 25 Evolutions Presentation.


## Phase 46 — Monsters, NPCs & Bosses — COMPLETE at repository/source level

- Added stable data-driven creature definitions for Monster, NPC and Boss roles.
- Added generic AI profile and bounded creature stats.
- Added dialogue/shop/quest/interaction IDs for later world systems.
- Added optional mesh/animation/montage/VFX/SFX presentation references.
- Added spawn definitions with zone, transform, MaxAlive and bounded respawn timing.
- Added normalized creature registry and UAetherCreatureSubsystem World Subsystem.
- Added reusable AAetherCreatureActor and UAetherCreatureCatalog Data Asset.
- Added Automation tests and Unreal setup documentation.
- No fake Unreal binary assets.
- Unreal 5.8.1 runtime remains NOT VERIFIED; local acceptance is required for actual asset import, spawn, replication and Dedicated Server behavior.

Next target: **Phase 47 — World, Maps & Streaming**


## Phase 47 — World, Maps & Streaming — COMPLETE at repository/source level

- Added stable map definitions and Persistent/Streamed/Instance load modes.
- Added map registry, validated streaming links and active-map gating.
- Added Data Asset catalog for low-bureaucracy map authoring.
- Added Automation tests and Unreal setup documentation.
- No fake .umap/.uasset assets.
- Unreal 5.8.1 runtime remains NOT VERIFIED; real maps, streaming and multiplayer tests are local acceptance.

Next target: **Phase 48 — Interaction, NPC, Quest & Events**
