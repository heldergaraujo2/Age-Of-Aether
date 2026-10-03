# AGE OF AETHER — CANONICAL CONTINUITY / HANDOFF

> **Arquivo canônico de continuidade do projeto.**
> Um novo chat deve ler este arquivo antes de tomar decisões, juntamente com `ROADMAP.md` e `ROADMAP_OPEN_WORLD_ISOMETRIC.md`.
> Este arquivo deve ser atualizado ao fechar cada fase/gate relevante.
> Nunca declarar validação Unreal/runtime que não tenha sido realmente executada.

---

## 0. ESTADO ATUAL — 2026-10-03

**Projeto:** AGE OF AETHER  
**Technical project:** `AgeOfAether`  
**Repository:** `heldergaraujo2/Age-Of-Aether`  
**Branch:** `main`  
**Engine alvo:** Unreal Engine 5.8.x; último estado registrado: **5.8.2-56702186+++UE5+Release-5.8-Windows**.

### Estado canônico

- A arquitetura MMORPG/server-authoritative existente deve ser **preservada e evoluída**, não reconstruída.
- As fases técnicas/source do roadmap legado **0–56 estão implementadas em nível de repositório/source**, mas isso **não significa que o jogo esteja runtime-complete**.
- O novo roadmap de transformação **Open World + Isometric 2.5D + Painterly Diorama** teve a **FASE 0 — AUDITORIA DA BASE** concluída.
- O próximo gate formal da transformação é **FASE 1 — FUNDAÇÃO REAL DO UNREAL**.
- O projeto ainda precisa de validação real no Unreal para fechar os gates de build/runtime/PIE/2-client/Dedicated Server/Automation e, depois, produção/aceitação de conteúdo real.
- A transformação visual e de mundo aberto deve acontecer **sobre a arquitetura existente**, sem apagar sistemas já implementados.

### Última evidência de execução registrada

- Build C++ Development Editor: **PASS** no último checkpoint registrado.
- Última automação registrada: **280 total / 266 PASS / 14 FAIL / 0 WARN**.
- Runtime acceptance: **NÃO CONCLUÍDA**.
- Principal bloco conhecido: **ClassBalance**.
- Outros failures registrados: `BalanceSimulation.Neutral`, `ClassCombat.PvPSwitch`, `Data.QuestDialogueEvent.CrossReferences`, `Multiplayer.HeartbeatCannotRefillRequests`, `Multiplayer.RateLimit`, `Quests.Security`, `Security.Replay`.
- A correção do fixture de ClassBalance foi build-verificada, mas o último relatório ainda continha falhas; portanto ClassBalance **não deve ser marcado como resolvido sem nova execução real**.

**Regra:** esses números são o último estado registrado, não uma alegação de que continuam verdadeiros hoje. Qualquer novo chat deve atualizar a evidência antes de declarar PASS.

---

# 1. VISÃO DEFINITIVA DO JOGO

AGE OF AETHER é um **MMORPG RPG de mundo aberto**, desenvolvido sobre Unreal Engine + C++, com:

- arquitetura server-authoritative;
- persistência;
- multiplayer;
- progressão;
- classes e evoluções;
- combate;
- skills/effects/buffs/debuffs;
- criaturas, NPCs e bosses;
- quests/events/dialogue;
- inventory/equipment/loot;
- crafting/forge/economy/shops;
- social/party/guild/chat;
- segurança/anti-cheat;
- AI;
- áudio;
- UI/UX;
- performance/streaming;
- conteúdo data-driven;
- validação automatizada;
- futura expansão contínua sem reescrever os sistemas centrais.

## Direção visual oficial

**Stylized Painterly Isometric / 2.5D Hand-Painted Diorama / 2.5D Isometric Cutaway**, em **qualidade gráfica premium**.

O objetivo não é criar um protótipo visual genérico. A qualidade final deve buscar:

- ambientes ricos e detalhados;
- materiais painterly coerentes;
- iluminação e sombras de alta qualidade;
- vegetação e arquitetura densas;
- personagens e criaturas com apresentação final;
- animações;
- VFX;
- áudio;
- composição cinematográfica;
- atmosfera;
- forte identidade artística;
- desempenho controlado por profiling real.

---

# 2. REGRA ABSOLUTA DE EVOLUÇÃO

Sempre classificar cada elemento como:

1. **JÁ IMPLEMENTADO** → preservar e reutilizar.
2. **IMPLEMENTADO, MAS INCOMPLETO** → finalizar.
3. **PROVISÓRIO/BOOTSTRAP** → substituir quando chegar o momento.
4. **PLANEJADO** → desenvolver conforme o roadmap.
5. **NOVO REQUISITO** → integrar primeiro ao planejamento; depois implementar.

Não criar uma segunda arquitetura para substituir a primeira sem necessidade comprovada.

Não criar sistemas duplicados de inventory, economy, combat, persistence, world, etc.

A regra geral é:

**preservar → adaptar → integrar → validar → expandir.**

---

# 3. ARQUITETURA NÃO NEGOCIÁVEL

### C++

Responsável por regras, autoridade e sistemas críticos:

- server runtime;
- networking;
- sessões/accounts/characters;
- progression;
- items/inventory/equipment;
- combat;
- skills/effects;
- creatures/AI contracts;
- quests/events;
- economy/crafting;
- persistence;
- security/anti-cheat;
- transactions;
- audit;
- authoritative simulation;
- automated tests.

### Blueprint / Unreal

Usado principalmente para:

- apresentação;
- composição;
- UI/UMG;
- animações;
- VFX;
- SFX;
- composição de NPCs/interactables;
- workflows seguros para designers.

Blueprint não substitui a autoridade server-side.

### Data

Define conteúdo:

- classes/evoluções;
- items;
- monsters/NPCs/bosses;
- skills;
- quests;
- events;
- loot;
- maps/world definitions;
- recipes;
- economy;
- visual profiles;
- audio;
- content packages.

**C++ = como o jogo funciona.**  
**Data = o que existe no jogo.**  
**Blueprint = como é apresentado/composto.**

---

# 4. MUNDO ABERTO — REGRA DEFINITIVA

O mundo final será **UM ÚNICO MUNDO CONTÍNUO**, não uma coleção de mapas de gameplay desconectados.

## Escala

Alvo aproximado:

**40 grandes cidades**.

Cada cidade será:

- centro geográfico;
- centro cultural;
- centro de serviços;
- centro de identidade regional;
- núcleo de uma **grande jurisdição explorável**.

### Estrutura territorial de cada cidade

**cidade/safezone → periferia → wilderness → sub-regiões → áreas de descoberta → fronteira natural → próxima jurisdição/cidade**

A cidade não termina na muralha.

Sua identidade se estende para:

- ecossistema;
- criaturas;
- NPCs;
- arquitetura periférica;
- recursos;
- clima;
- VFX;
- landmarks;
- ruínas;
- cavernas;
- eventos;
- histórias;
- perigos;
- descobertas.

## Exploração / Hunt / Desbravamento

O jogador deve sentir que:

> a cidade é um ponto seguro dentro de uma enorme região desconhecida que precisa ser desbravada.

Cada jurisdição deve ter território suficientemente grande para permitir **longas expedições de exploração e caça**.

A distância entre grandes cidades deve ser significativa. Uma viagem a pé entre grandes centros poderá durar **horas**, dependendo de:

- rota;
- terreno;
- perigos;
- obstáculos;
- descobertas;
- atalhos;
- eventos;
- condições futuras de viagem.

Não preencher escala com terreno vazio. Viagens longas precisam de conteúdo.

## Geografia contínua

O mundo deve possuir:

- estradas;
- rios;
- costas;
- montanhas;
- florestas;
- desertos;
- pântanos;
- regiões congeladas;
- regiões vulcânicas;
- passes;
- pontes;
- vales;
- barreiras naturais;
- ruínas;
- dungeons;
- landmarks;
- fronteiras naturais.

World Partition, streaming e HLOD podem dividir tecnicamente o mundo internamente, mas para o jogador deve existir:

- uma identidade mundial;
- continuidade geográfica;
- coordenadas coerentes;
- estado regional persistente;
- travessia contínua;
- ausência de telas de loading entre regiões normais do overworld.

Dungeons/interiores especiais podem usar instancing quando necessário.

---

# 5. CIDADES E JURISDIÇÕES

As ~40 cidades **não podem ser cópias**.

Cada uma deve possuir:

- silhueta própria;
- arquitetura própria;
- identidade cultural;
- paleta/material language;
- ecossistema;
- clima;
- criaturas;
- NPCs;
- recursos;
- landmarks;
- VFX;
- histórias;
- regiões externas próprias.

Exemplos conceituais apenas:

- cidade de gelo → neve, geleiras, rios congelados, cavernas de gelo, criaturas de gelo;
- cidade de fogo → vulcões, lava, cinzas, basalto, calor, criaturas de fogo.

Esses exemplos **não são uma lista definitiva nem devem ser copiados como conteúdo final**.

Ainda **não** fixar regras de força de monstros, equipamentos ou progressão por cidade. Isso pertence a fases posteriores.

---

# 6. WORLD MASTER PLAN

Antes da produção definitiva do terreno, deve existir um World Master Plan contendo:

1. aproximadamente 40 cidades;
2. nomes e identidades;
3. localização;
4. jurisdições;
5. fronteiras;
6. biomas e transições;
7. estradas;
8. rios;
9. costas;
10. cadeias montanhosas;
11. florestas;
12. desertos;
13. pântanos;
14. regiões congeladas;
15. regiões vulcânicas;
16. territórios culturais/faccionais;
17. ecologia de criaturas;
18. ecologia de NPCs;
19. recursos;
20. dungeons;
21. landmarks;
22. eventos;
23. bandas de progressão;
24. pontos de interesse;
25. tempos de viagem.

A distância entre cidades será decidida pelo World Master Plan.

O primeiro slice deve ser uma **região real do mundo definitivo**, não um mapa descartável que contradiga a escala final.

---

# 7. SISTEMAS JÁ EXISTENTES QUE DEVEM SER PRESERVADOS

A base atual já possui arquitetura/source para:

- runtime lifecycle;
- networking;
- accounts/sessions;
- characters;
- items;
- inventory;
- progression;
- combat;
- world runtime;
- quests;
- social;
- economy/crafting;
- multiplayer/server authority;
- persistence;
- security;
- scale;
- AI/GPT integration;
- content registries;
- asset pipeline;
- visual foundation;
- playable character;
- input/movement/camera;
- animation;
- equipment visuals;
- classes/evolutions;
- combat presentation;
- skills/effects;
- creatures/NPCs/bosses;
- world/maps/streaming;
- interaction/quest/events;
- inventory/loot/equipment gameplay;
- crafting/forge/economy/shops;
- UI/UX;
- audio;
- persistence;
- performance/streaming/scale;
- content package;
- release gates.

Esses sistemas não devem ser descartados para construir um protótipo separado.

---

# 8. CLASSES E EVOLUÇÕES

O sistema de classes/evoluções já existe em nível source.

### 5 classes base

1. Archer — ARQUEIRO
2. Warrior — GUERREIRO
3. Mage — MAGO
4. Tank — TANK
5. Healer — HEALER

### 25 evoluções

Cada classe possui 5 estágios:

- Stage 1 — nível 1
- Stage 2 — nível 20
- Stage 3 — nível 40
- Stage 4 — nível 60
- Stage 5 — nível 80

Archer:
- Batedor
- Rastreador
- Caçador Espectral
- Atirador Fantasma
- Olho de Deus

Warrior:
- Recruta
- Berserker
- Campeão de Guerra
- Lorde das Lâminas
- Avatar da Guerra

Mage:
- Aprendiz
- Feiticeiro Elemental
- Arquimago
- Tecelão do Éter
- Senhor do Caos Primordial

Tank:
- Guardião
- Fortaleza de Aço
- Colosso
- Bastião Imortal
- Titã Ancestral

Healer:
- Iniciado
- Clérigo da Luz
- Oráculo Sagrado
- Serafim
- Avatar da Vida Eterna

Estado:
- sistema source: **IMPLEMENTADO**;
- apresentação source: **IMPLEMENTADA**;
- runtime completo/balanceamento: **PENDENTE**;
- ClassBalance: **GATE CONHECIDO A REVALIDAR**.

---

# 9. HISTÓRICO DE FASES

## Roadmap técnico legado — fases 0–36

As fases técnicas do roadmap MMORPG original foram implementadas em nível de repository/source, cobrindo:

- fundação do repositório;
- Unreal project foundation;
- runtime;
- networking;
- accounts/sessions;
- characters;
- inventory;
- progression;
- combat;
- world;
- quests;
- social;
- economy/crafting;
- multiplayer/server authority;
- persistence/backend;
- security/anti-cheat;
- MMORPG scale/dedicated server;
- AI/GPT integration;
- production/live MMORPG;
- universal data model/content registry;
- asset pipeline;
- items/equipment/enhancement;
- monsters/NPCs/bosses/AI content;
- skills/effects/buffs/debuffs;
- loot/drop/reward/respawn;
- quest/event/dialogue/world authoring;
- crafting/forge/recipe authoring;
- world/map/interaction/streaming content;
- client core;
- UI/UX;
- client presentation/performance/integration;
- classes/evolutions;
- class balance;
- class/combat integration;
- balance simulation;
- class presentation/runtime foundation.

**Importante:** "complete at repository/source level" não equivale a "validado no Unreal".

## Phases 37–56

Também implementadas em source/repository:

37. Unreal Visual Foundation  
38. Real Asset Pipeline  
39. Playable Base Character  
40. Input, Movement & Camera  
41. Animation System  
42. Equipment & Item Visuals  
43. Five Classes & 25 Evolutions Presentation  
44. Playable Combat  
45. Skills, Buffs, Debuffs & VFX  
46. Monsters, NPCs & Bosses  
47. World, Maps & Streaming  
48. Interaction, NPC, Quest & Events  
49. Inventory, Loot & Equipment Gameplay  
50. Crafting, Forge, Economy & Shops  
51. Complete MMORPG UI/UX  
52. Audio & Ambience  
53. Multiplayer, Dedicated Server & Persistence  
54. Performance, Streaming & Scale  
55. Initial Complete Content Package  
56. Alpha, Beta & Release Candidate

**Estado:** source/repository completo; runtime/content real ainda é o gate.

---

# 10. NOVO ROADMAP DE TRANSFORMAÇÃO

Arquivo oficial: `ROADMAP_OPEN_WORLD_ISOMETRIC.md`.

### Fase 0 — Auditoria da Base
**CONCLUÍDA** em nível repository/source.

Resultado:
- arquitetura auditada;
- dependências e riscos mapeados;
- decisão de preservar a base;
- direção visual definida;
- benchmark inicial definido;
- requisitos de mundo aberto definidos.

### Fase 1 — Fundação Real do Unreal
**PRÓXIMA FASE / GATE ATUAL.**

Objetivo:
- fechar UHT;
- UBT Editor/Game;
- Editor startup;
- mapa real;
- PIE;
- 2-client PIE;
- Dedicated Server;
- Automation;
- logs;
- replicação básica;
- personagem placeholder jogável em multiplayer básico.

**Não marcar concluída sem evidência real.**

### Fases seguintes do novo roadmap

2. Direção Visual Isométrica  
3. Pipeline de Arte Real  
4. Câmera e Exploração Isométrica  
5. Primeiro Diorama Jogável  
6. Núcleo RPG Jogável  
7. Primeiro Inimigo e Combate  
8. Primeira Dungeon  
9. Vertical Slice Completo  
10. Primeiro Teste Real Jogável  
11. World Map  
12. Cidades Vivas  
13. Mundo PvE  
14. Progressão MMORPG  
15. Mundo Vivo  
16. Polimento Artístico  
17. Escala e Performance  
18. Multiplayer Real  
19. Conteúdo Inicial Completo  
20. Alpha  
21. Beta  
22. Release Candidate  
23. Lançamento e Evolução

Sequência crítica:

**base existente → Unreal real → câmera isométrica → diorama → personagem → exploração → RPG → combate → dungeon → vertical slice → PRIMEIRO TESTE REAL → mundo aberto → MMORPG em escala.**

---

# 11. BENCHMARK VISUAL INICIAL

O primeiro benchmark deve conter, em pequena escala geográfica mas próximo da qualidade final:

**cidade/vila → estrada → floresta → área de combate → entrada de dungeon**

Incluindo:

- personagem;
- classe/evolução;
- equipamento;
- NPC;
- interação;
- quest;
- criaturas;
- combate;
- skills;
- loot;
- dungeon;
- boss;
- HUD;
- iluminação;
- ambiente;
- áudio;
- VFX.

O benchmark não deve virar um protótipo descartável. Ele deve ser construído como parte do mundo definitivo.

---

# 12. O QUE NÃO DEVE SER FEITO

- Não reconstruir o Age of Aether do zero.
- Não criar um segundo projeto.
- Não substituir sistemas existentes sem auditoria.
- Não criar uma segunda implementação de inventory/economy/combat/persistence.
- Não fabricar .uasset/.umap/FBX/binários falsos.
- Não declarar runtime PASS sem executar o Unreal.
- Não tratar "source complete" como "game complete".
- Não criar 40 mapas desconectados para representar as cidades.
- Não criar pequenas cidades isoladas cercadas por outra cidade.
- Não copiar Lorencia, seus mapas ou sua arte.
- Não usar MU Online como conteúdo a ser copiado; apenas conceitos estruturais podem servir como referência de design quando apropriado.
- Não preencher o mundo com terreno vazio apenas para aumentar escala.
- Não definir prematuramente balanceamento de cidades sem passar pela fase de design/progressão apropriada.
- Não reduzir a qualidade visual para acelerar a escala.

---

# 13. VALIDAÇÃO — REGRA DE VERDADE

Sempre distinguir:

### SOURCE / REPOSITORY
Pode ser validado por:
- inspeção;
- compilação quando disponível;
- testes automatizados;
- contratos;
- documentação;
- registries;
- análise estática.

### UNREAL RUNTIME
Precisa de:
- Unreal Editor;
- UHT/UBT;
- PIE;
- 2-client PIE;
- Dedicated Server;
- replicação;
- import real de assets;
- Data Assets reais;
- mapas reais;
- World Partition real;
- profiling;
- logs;
- testes de gameplay.

Nunca promover uma fase de "PENDENTE" para "PASS" apenas porque o código parece correto.

---

# 14. WORKFLOW OBRIGATÓRIO DE FASE

Para cada fase:

1. Ler este arquivo.
2. Ler o roadmap correspondente.
3. Auditar o estado atual do repositório.
4. Identificar o que já existe.
5. Preservar sistemas existentes.
6. Implementar apenas o necessário.
7. Criar/atualizar testes.
8. Executar os testes possíveis.
9. Separar source validation de Unreal runtime validation.
10. Registrar evidências reais.
11. Atualizar documentação da fase.
12. Atualizar `ROADMAP.md` quando necessário.
13. Atualizar `ROADMAP_OPEN_WORLD_ISOMETRIC.md` quando necessário.
14. **Atualizar este `PROJECT_MEMORY/00_CONTINUITY.md`.**
15. Commitar no GitHub.
16. Registrar o commit aqui.
17. Somente então considerar o gate documental fechado.

---

# 15. COMO UM NOVO CHAT DEVE CONTINUAR

Ao iniciar um novo chat sobre Age of Aether:

### Primeiro ler

1. `PROJECT_MEMORY/00_CONTINUITY.md`
2. `ROADMAP.md`
3. `ROADMAP_OPEN_WORLD_ISOMETRIC.md`
4. documentação da fase atual;
5. código/testes relevantes.

### Depois identificar

- proposta definitiva do jogo;
- arquitetura;
- sistemas existentes;
- fases concluídas;
- fases source-complete;
- runtime gates pendentes;
- fase/gate atual;
- próximo objetivo exato;
- último commit;
- últimas evidências.

### Regra de decisão

Se este arquivo disser:

**FASE ATUAL = X**

o novo chat deve continuar por X e **não inventar uma nova fase**.

Se encontrar discrepância entre documentação e código, deve auditar o repositório antes de decidir.

Se encontrar discrepância entre source e runtime, runtime real tem precedência para declarar aceitação.

---

# 16. ÚLTIMO CHECKPOINT DOCUMENTAL

### Produto

**AGE OF AETHER — MMORPG RPG Open World Isométrico 2.5D Painterly Premium**

### Arquitetura

**Existente / preservar / server-authoritative / data-driven**

### Source roadmap

**Phases 0–56: implementadas em nível repository/source, com runtime real ainda pendente.**

### Novo roadmap de transformação

**Phase 0: COMPLETE — auditoria da base.**

### Gate atual

**PHASE 1 — REAL UNREAL FOUNDATION**

### Próximo grande objetivo

Provar o Unreal real e fechar a fundação runtime antes de avançar para a produção visual em escala.

### Depois do gate

Construir o primeiro benchmark visual jogável dentro da arquitetura do **único mundo contínuo**, já compatível com a futura estrutura de ~40 cidades e grandes jurisdições exploráveis.

### Regra de mundo

**1 mundo contínuo + ~40 cidades + grandes jurisdições + exploração/Hunt + geografia natural + World Partition/streaming interno.**

### Regra de continuidade

**Este arquivo deve ser atualizado a cada fase/gate concluído.**

---

# 17. REFERÊNCIAS CANÔNICAS

- `ROADMAP.md` — arquitetura e roadmap técnico MMORPG.
- `ROADMAP_CONTENT_AND_CLIENT.md` — conteúdo, dados e cliente.
- `ROADMAP_VISUAL_AND_PLAYABLE.md` — implementação visual/runtime.
- `ROADMAP_OPEN_WORLD_ISOMETRIC.md` — transformação Open World + Isometric + Painterly.
- `Docs/PHASE_0_OPEN_WORLD_ISOMETRIC_AUDIT.md` — auditoria da transformação.
- `Docs/PHASE_37_UNREAL_VISUAL_FOUNDATION.md`
- `Docs/PHASE_38_REAL_ASSET_PIPELINE.md`
- `Docs/PHASE_39_PLAYABLE_BASE_CHARACTER.md`
- `Docs/PHASE_40_INPUT_MOVEMENT_CAMERA.md`
- documentação das fases posteriores conforme necessário.

**Fonte de verdade:** repositório oficial GitHub.

**Última regra:** nunca perder a visão completa do AGE OF AETHER ao trabalhar em uma fase específica. Cada implementação deve contribuir para o MMORPG final, seu mundo contínuo, suas cidades, suas jurisdições, sua exploração e todos os sistemas já planejados.
