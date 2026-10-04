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
- O gate runtime obrigatório continua sendo **FASE 1 — FUNDAÇÃO REAL DO UNREAL**, cuja auditoria no ambiente Unreal real ainda está pendente.
- Em nível source/documentation, a transformação avançou até **FASE 5 — PRIMEIRO DIORAMA JOGÁVEL**; as Fases 2, 3, 4 e 5 estão fechadas somente em nível de source/documentação. A Fase 6 do novo roadmap foi reconciliada como integração/auditoria dos sistemas RPG já existentes, não como uma nova implementação.
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

2. Direção Visual Isométrica — SOURCE/DOCUMENTATION COMPLETE / RUNTIME PENDING  
3. Pipeline de Arte Real — SOURCE/DOCUMENTATION COMPLETE / RUNTIME PENDING  
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

## 10A. AVANÇO DOCUMENTAL — PHASE 2

### Phase 2 — Direção Visual Isométrica

**SOURCE/DOCUMENTATION: COMPLETE**  
**UNREAL RUNTIME: PENDING**

A direção visual isométrica foi formalizada em Docs/PHASE_2_ISOMETRIC_VISUAL_DIRECTION.md, cobrindo:

- câmera e leitura isométrica;
- composição de diorama;
- princípios painterly premium;
- paleta e materiais;
- iluminação e profundidade;
- personagens e criaturas;
- arquitetura e props;
- VFX;
- integração com o único mundo contínuo.

Esta conclusão é **documental/source-level**. Não constitui aceitação visual no Unreal.

**Pendência explícita:** continua faltando a **auditoria no ambiente real do Unreal Engine**. A auditoria deve comprovar a execução real da câmera, zoom, ângulo, movimento relativo, clipping/oclusão, iluminação, PIE, 2-client PIE e logs. Nenhuma dessas evidências foi fabricada ou presumida.

A próxima fase documental, agora concluída, foi **PHASE 3 — REAL ART PIPELINE**. A **PHASE 1 runtime gate permanece aberta** até a auditoria real do Unreal.

### Phase 3 — Pipeline de Arte Real

**SOURCE/DOCUMENTATION: COMPLETE**  
**UNREAL RUNTIME / REAL ASSET ACCEPTANCE: PENDING**

A documentação PHASE_3_REAL_ART_PIPELINE.md formaliza o pipeline único de Concept/Source → Cleanup → Import → Unreal Asset → Materials/Skeleton/Collision → Asset Registry → Stable AssetID → Runtime.

Foram explicitados:
- provenance/licença/origem e versão;
- lifecycle e critérios por classe de asset;
- integração com os contratos existentes da Phase 38;
- AssetID, dependências e fallbacks;
- materiais/texturas e requisitos painterly premium;
- skeleton/Physics Asset/animações;
- sockets e collision;
- política de LOD/Nanite baseada em profiling real;
- VFX e integração com os consumidores existentes;
- primeiro asset real de aceitação e ordem de expansão;
- separação rigorosa entre validação source e Unreal runtime.

Nenhum .uasset, .umap, FBX ou outro binário Unreal foi fabricado.

**Pendência explícita:** continua faltando a **auditoria no ambiente real do Unreal Engine**. A Phase 3 não é considerada runtime-validada até existir evidência local de import real de asset, inspeção, Asset Manager, PIE, 2-client PIE, Dedicated Server e logs, além do gate de fundação da Phase 1.

**Próxima fase documental:** PHASE 5 — PRIMEIRO DIORAMA JOGÁVEL.

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

**Phase 1: IN PROGRESS — preparação concluída no repositório; runtime Unreal ainda pendente.**

**Phase 2: SOURCE/DOCUMENTATION COMPLETE — direção visual isométrica formalizada; aceitação runtime ainda pendente.**

**Phase 3: SOURCE/DOCUMENTATION COMPLETE — pipeline de arte real formalizado; aceitação de assets no Unreal runtime ainda pendente.**

**Phase 4: SOURCE/DOCUMENTATION COMPLETE — câmera e exploração isométrica formalizadas; aceitação no Unreal runtime ainda pendente.**



### Phase 4 — Câmera e Exploração Isométrica

**SOURCE/DOCUMENTATION: COMPLETE**  
**UNREAL RUNTIME: PENDING**

A documentação `Docs/PHASE_4_ISOMETRIC_CAMERA_EXPLORATION.md` formaliza a experiência de exploração isométrica usando a fundação source-level existente da Phase 40, sem criar uma segunda implementação de movimento/câmera.

Foram definidos:
- modelo de câmera isométrica e framing;
- zoom limitado e independente da autoridade de gameplay;
- collision/occlusion;
- movimento relativo à câmera;
- política de rotação;
- seleção, targeting e interação;
- multiplayer e limites de autoridade;
- compatibilidade com o mundo contínuo e World Partition/streaming;
- requisitos de performance e matriz de aceitação runtime.

Nenhum `.uasset`, `.umap`, FBX ou outro binário Unreal foi fabricado e nenhuma validação runtime foi presumida.

**Pendência explícita e não negociável:** continua faltando a **auditoria no ambiente real do Unreal Engine**. A Phase 4 só poderá receber aceitação runtime após execução local real da câmera, zoom, collision/occlusion, movimento relativo, seleção/targeting/interação, PIE, 2-client PIE, Dedicated Server, logs e profiling, além do gate de fundação da Phase 1.

**Próxima fase documental:** PHASE 5 — PRIMEIRO DIORAMA JOGÁVEL.

### Auditoria obrigatória do ambiente real do Unreal

**PENDÊNCIA EXPLÍCITA E NÃO NEGOCIÁVEL:** ainda falta realizar a **auditoria no ambiente real do Unreal Engine**.

Isso significa que o próximo avanço não pode ser considerado validado apenas pelo GitHub/source. É necessário executar localmente, no ambiente Unreal instalado pelo usuário, a auditoria dos artefatos e do runtime reais, incluindo:

- versão/toolchain efetivamente instalada;
- geração/atualização dos project files;
- UHT;
- UBT Editor;
- UBT Game;
- UBT Dedicated Server;
- abertura real do `AgeOfAether.uproject`;
- existência/abertura do mapa de desenvolvimento real;
- GameMode/PlayerStart reais;
- PIE de 1 cliente;
- PIE de 2 clientes;
- Dedicated Server + cliente;
- spawn/posse/movimento/câmera reais;
- replicação básica observável;
- Automation Framework real;
- Output Log e erros críticos;
- reprodutibilidade após novo pull limpo.

**Regra:** até essa auditoria ser executada e suas evidências serem registradas, a Phase 1 permanece **IN PROGRESS / RUNTIME GATE PENDING**. Não avançar documentalmente para uma aceitação runtime fictícia e não transformar "source/repository complete" em "Unreal validated".

### Gate atual

**PHASE 1 — REAL UNREAL FOUNDATION / RUNTIME GATE PENDING**

Preparação source/repository: **READY**.

Documentação: `Docs/PHASE_1_REAL_UNREAL_FOUNDATION.md`.

A fase não pode ser marcada COMPLETE até existir evidência local real de UHT, UBT Editor/Game/Server, Editor startup, mapa real, PIE, 2-client PIE, Dedicated Server, replicação, Automation e logs.

### Próximo grande objetivo

Continuar a preparação documental/source da transformação visual, mantendo explicitamente o gate de **auditoria no ambiente real do Unreal** aberto. A aceitação runtime da fundação e da direção visual só ocorrerá após execução local real.

### Depois do gate

Construir o primeiro benchmark visual jogável dentro da arquitetura do **único mundo contínuo**, já compatível com a futura estrutura de ~40 cidades e grandes jurisdições exploráveis.

### Regra de mundo

**1 mundo contínuo + ~40 cidades + grandes jurisdições + exploração/Hunt + geografia natural + World Partition/streaming interno.**

### Regra de continuidade

**Este arquivo deve ser atualizado a cada fase/gate concluído.**

### Último avanço documental

- PHASE_4_ISOMETRIC_CAMERA_EXPLORATION.md criado e fechado em nível source/documentation.
- PHASE_3_REAL_ART_PIPELINE.md permanece fechado em nível source/documentation.
- PHASE_1_REAL_UNREAL_FOUNDATION.md permanece como referência do gate runtime.
- `ROADMAP_OPEN_WORLD_ISOMETRIC.md` atualizado para refletir a Phase 1 como runtime gate pendente.
- Nenhum mapa `.umap` ou asset Unreal binário foi fabricado.
- Nenhum runtime PASS foi declarado sem execução local.

---

# 17. REFERÊNCIAS CANÔNICAS

- `ROADMAP.md` — arquitetura e roadmap técnico MMORPG.
- `ROADMAP_CONTENT_AND_CLIENT.md` — conteúdo, dados e cliente.
- `ROADMAP_VISUAL_AND_PLAYABLE.md` — implementação visual/runtime.
- `ROADMAP_OPEN_WORLD_ISOMETRIC.md` — transformação Open World + Isometric + Painterly.
- `Docs/PHASE_0_OPEN_WORLD_ISOMETRIC_AUDIT.md` — auditoria da transformação.
- `Docs/PHASE_3_REAL_ART_PIPELINE.md` — pipeline de arte real.
- `Docs/PHASE_4_ISOMETRIC_CAMERA_EXPLORATION.md` — câmera e exploração isométrica.
- `Docs/PHASE_37_UNREAL_VISUAL_FOUNDATION.md`
- `Docs/PHASE_38_REAL_ASSET_PIPELINE.md`
- `Docs/PHASE_39_PLAYABLE_BASE_CHARACTER.md`
- `Docs/PHASE_40_INPUT_MOVEMENT_CAMERA.md`
- documentação das fases posteriores conforme necessário.

**Fonte de verdade:** repositório oficial GitHub.

**Última regra:** nunca perder a visão completa do AGE OF AETHER ao trabalhar em uma fase específica. Cada implementação deve contribuir para o MMORPG final, seu mundo contínuo, suas cidades, suas jurisdições, sua exploração e todos os sistemas já planejados.


---

# 10. PHASE 5 — PRIMEIRO DIORAMA JOGÁVEL

**Status:** SOURCE/DOCUMENTATION COMPLETE / UNREAL RUNTIME PENDING.

Documentação: `Docs/PHASE_5_FIRST_PLAYABLE_DIORAMA.md`.

Phase 5 define o primeiro diorama jogável de produção como região extensível do mundo contínuo: vila/núcleo urbano → praça → serviços/NPCs → estrada → floresta/campo → área de combate → entrada de dungeon. O benchmark visual oficial permanece Stylized Painterly Isometric / 2.5D Hand-Painted Diorama / 2.5D Isometric Cutaway. Referência conceitual de imagem: 4096 × 2304 (4K, 16:9), sem transformar isso em requisito de runtime/textura.

A aceitação Unreal/runtime permanece pendente, incluindo mapa, assets, câmera, player/NPCs, interação, PIE, 2-client PIE, Dedicated Server, logs, Automation e profiling. O gate da Phase 1 — auditoria no ambiente real do Unreal — continua aberto.


---

# 11. RECONCILIAÇÃO DA AUDITORIA DO REPOSITÓRIO — 2026-10-03

A auditoria minuciosa do repositório confirmou uma distinção fundamental entre o **roadmap técnico legado** e o **novo roadmap de transformação Open World/Isometric/Painterly**.

## Sistemas que já existem em source/repository

Não devem ser reconstruídos:

- progression / XP;
- combat / server authority;
- items;
- inventory;
- equipment;
- loot / rewards / respawn;
- character/player state;
- creatures / NPC / boss / AI contracts;
- skills/effects;
- quests/events/interactions;
- economy/crafting;
- world maps/streaming;
- persistence;
- networking/multiplayer;
- UI/presentation;
- asset pipeline;
- visual foundation;
- playable character;
- input/movement/camera.

A auditoria encontrou implementação C++ específica para esses domínios e testes dedicados. Entre os arquivos confirmados estão:

- `AetherProgressionSubsystem/Service`;
- `AetherCombatSubsystem/Service`;
- `AetherInventorySubsystem`;
- `AetherItemSubsystem`;
- `AetherLootSubsystem`;
- `AetherCharacterPlayerState`;
- `AetherCreatureSubsystem`;
- `AetherWorldMapSubsystem/Registry/Catalog`;
- `AetherWorldStreamingCoordinator`;
- `AetherPersistenceSubsystem/Service`;
- `AetherNetworkGameMode`.

Também existem testes específicos para Progression, Combat, Inventory/Loot, Item/Inventory, Creatures, World Map e Persistence.

## O que a auditoria NÃO permite afirmar

A existência desses sistemas no GitHub não comprova execução correta no Unreal.

O último estado registrado continua indicando:
- Development Editor build: PASS no checkpoint registrado;
- Automation: 280 total / 266 PASS / 14 FAIL / 0 WARN no último relatório registrado;
- runtime acceptance: pendente;
- ClassBalance e outros failures históricos ainda precisam de nova execução para serem classificados.

## Conteúdo Unreal real

A árvore atual `Content/Aether` contém principalmente READMEs organizacionais para Maps, Characters, Items, Classes, Monsters, NPC, Materials, Textures, VFX, UI, Audio e Animations. Não há, no repositório atual, um conjunto suficiente de mapas/meshes/materials/animations/VFX binários para declarar o primeiro diorama como conteúdo Unreal real já produzido.

## Decisão de continuidade

O projeto deve seguir a regra:

**auditar o que já existe → reutilizar → integrar → validar no Unreal → corrigir lacunas reais → só então criar o que realmente faltar.**

O novo roadmap não deve reinterpretar uma funcionalidade já implementada no roadmap legado como uma solicitação para reconstruí-la.

### Gate prioritário

**PHASE 1 — REAL UNREAL FOUNDATION / RUNTIME GATE PENDING**

Antes de novas implementações de gameplay que já existem em source, é prioritário executar a auditoria real do Unreal e usar os resultados para determinar as próximas correções.

### Próximo trabalho após o gate

Com a fundação runtime comprovada, avançar para o primeiro conteúdo real do diorama usando o pipeline de arte existente, sem fabricar binários Unreal e sem criar uma arquitetura paralela.


## 2026-10-03 — FIRST PLAYABLE WORLD REGION PRODUCTION START

A canonical production specification was added at Docs/FIRST_PLAYABLE_WORLD_REGION_PRODUCTION_SPECIFICATION.md. It defines the first permanent world region as settlement -> plaza/services -> outskirts -> road -> natural transition -> wilderness -> exploration -> combat territory -> dungeon entrance, with the official premium painterly-isometric visual bar. No fabricated Unreal binary content was introduced. The real .umap remains pending materialization in Unreal Engine 5.8 and runtime validation.


---

# 2026-10-04 — REDEFINIÇÃO OFICIAL PARA PROJETO 2D ISOMÉTRICO

A direção visual anterior "Stylized Painterly Isometric / 2.5D Hand-Painted Diorama" foi reconciliada e substituída como objetivo de produção visual pela direção:

**RPG ISOMÉTRICO 2D PREMIUM**

A referência de experiência continua sendo a linguagem de exploração isométrica de RPGs como Lords of Xulima, mas o AGE OF AETHER terá identidade própria e não copiará arte, mapas ou personagens de nenhum jogo.

## Decisão técnica

O AGE OF AETHER não precisa ser um jogo 3D.

A representação visual pode ser predominantemente:
- imagens;
- sprites;
- sprite sheets;
- Flipbooks;
- camadas;
- paralaxe;
- sombras;
- iluminação;
- VFX;
- partículas;
- composição isométrica.

3D é opcional e só deve ser usado quando trouxer benefício comprovado.

A Unreal Engine continua sendo a plataforma do projeto e a raiz sistêmica existente continua sendo a base do jogo.

## Regra de adaptação

Não reconstruir:
- skills;
- progression/XP/levels;
- inventory;
- items/equipment;
- combat;
- quests/events;
- creatures/NPC/AI contracts;
- economy/crafting;
- persistence;
- networking;
- multiplayer;
- UI framework;
- world/streaming contracts;
- registries e sistemas equivalentes já existentes.

O trabalho novo é principalmente:
**adaptação visual 2D + produção de conteúdo + integração + validação.**

## Novo roadmap

O roadmap oficial agora é ROADMAP_OPEN_WORLD_ISOMETRIC.md, reorganizado em fases 0–26:

0. Auditoria e preservação da raiz
1. Fundação real do Unreal
2. Direção visual 2D isométrica
3. Pipeline de arte 2D
4. Sistema de personagens 2D
5. Criaturas, NPCs e chefes 2D
6. Ambientes e props 2D
7. Profundidade 2D, luz e sombra
8. Câmera e exploração isométrica
9. Primeira região permanente
10. Integração dos sistemas existentes
11. Primeiro inimigo e loop de combate
12. Primeira dungeon
13. Vertical slice
14. Primeiro teste real jogável
15. World Master Plan
16. Expansão das jurisdições
17. Cidades vivas
18. Mundo PvE
19. Mundo vivo e social
20. Polimento visual 2D premium
21. Escala e performance
22. Conteúdo inicial completo
23. Alpha
24. Beta
25. Release Candidate
26. Lançamento e evolução contínua

A antiga sequência de fases continua preservada como histórico de implementação/source, mas não deve ser interpretada como autorização para reconstruir os sistemas já existentes.

## Documentação canônica da direção 2D

Docs/AGE_OF_AETHER_2D_ISOMETRIC_VISUAL_DIRECTION.md

Esta documentação define a pipeline visual e os limites de fabricação/validação de assets.

## Gate atual

O gate de runtime da FASE 1 — FUNDAÇÃO REAL DO UNREAL continua aberto.

Nenhuma validação runtime foi inventada por esta mudança documental.

## Próxima regra

Antes de produzir grandes quantidades de conteúdo visual, seguir a FASE 1 e depois a FASE 2/3 do novo roadmap. O primeiro personagem, mapa ou asset deve ser produzido como prova da nova pipeline 2D, e não como trabalho isolado descartável.
