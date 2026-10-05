# AGE OF AETHER — CONTINUIDADE CANÔNICA POR FASES

> Fonte de verdade: repositório oficial GitHub, branch main.
> Este arquivo é o handoff obrigatório para qualquer novo chat.

## Marcações

🟩 CONCLUÍDO — evidência suficiente para o nível declarado.

🟥 FALTANTE / PENDENTE — existe trabalho ou gate de validação.

Uma fase pode ser concluída em source/documentação e continuar pendente em runtime. Isso deve ser declarado explicitamente.

## Regra central

O AGE OF AETHER já possui a raiz sistêmica. Não reconstruir skills, níveis/progression, inventory, items/equipment, combat, quests/events, creatures/AI, economy/crafting, persistence, networking, multiplayer, UI, world/streaming ou equivalentes já existentes, salvo se uma auditoria provar uma lacuna real.

Regra: preservar → adaptar → integrar → testar → validar → expandir.

## Diretriz permanente de qualidade visual

**REQUISITO TRANSVERSAL A TODAS AS FASES VISUAIS:** produzir imagens/arte na melhor qualidade possível e buscar animações 2D extremamente fluidas, naturais e responsivas. Essa exigência vale para personagens, criaturas, NPCs, VFX, ambientes, mapas, iluminação, sombras, UI e qualquer conteúdo visual. Qualidade e fluidez são requisitos de projeto, não apenas polish final.

**Regra:** não sacrificar qualidade visual ou fluidez por conveniência de produção sem registrar e justificar tecnicamente a decisão.

## Direção visual oficial

RPG isométrico 2D premium, inspirado na experiência de RPGs isométricos clássicos, com identidade própria. O jogo não precisa ser 3D. Pode usar imagens, sprites, sprite sheets, Flipbooks, camadas, paralaxe, luz, sombras, VFX e partículas. 3D é opcional.

## Fases

### FASE 0 — Auditoria e preservação da raiz 🟩

**Objetivo:** Auditar a arquitetura e confirmar que os sistemas já existentes serão preservados, sem reconstrução desnecessária.

**Criar e testar:** Auditoria repository/source; inventário dos sistemas; classificação preservar/adaptar/substituir; identificação de conteúdo ausente; reconciliação do roadmap; registro dos gates de teste e da separação source/runtime.

**Estado:** 🟩 **TOTALMENTE CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE.**

**Resultado:** A auditoria confirmou a raiz de progression, classes/evoluções, combat, skills/effects, inventory, items, equipment, loot, quests/events, creatures/NPC/AI contracts, economy/crafting, persistence, networking/multiplayer, world/map/streaming, UI, audio, Asset Manager, registries, visual/presentation contracts e Automation. A decisão oficial é preservar esses sistemas e adaptar a apresentação para o projeto RPG isométrico 2D premium.

**Evidência principal:** `Docs/PHASE_0_OPEN_WORLD_ISOMETRIC_AUDIT.md`, commit `87a13e3cbdd92da70472960f553015b7475e06ab`.

**Evidências históricas relacionadas:** `b3c7609f7e6655bf753cf7dbce5d88df200fef4c` (auditoria inicial), `de219133274f1616bacdd850cf1c3d51cf91b6710` (reconciliação da arquitetura) e `7a0a003173295bf0ef7f3407cf9e42d1e4ef24c9` (reconciliação do roadmap).

**Testes:** a auditoria preserva como evidência histórica 280 automações: 266 PASS, 14 FAIL, 0 WARN. Esses números não são apresentados como execução nova nesta sessão. As falhas históricas permanecem registradas como gates técnicos e não foram falsamente marcadas como resolvidas.

**Gate:** Fase 0 não exige runtime para ser encerrada. O runtime permanece explicitamente separado na Fase 1.


### FASE 1 — Fundação real do Unreal 🟥

**Objetivo:** Provar a raiz no Unreal 5.8 real, sem criar gameplay novo.

**Criar e testar:** UHT; UBT Editor/Game/Server; startup; mapa; GameMode/PlayerStart; PIE; 2-client PIE; Dedicated Server; spawn; movimento; replicação; Automation; logs.

**Estado:** Runtime é o gate atual.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 2 — Direção visual 2D isométrica 🟩

**Objetivo:** Fixar RPG isométrico premium predominantemente 2D.

**Criar e testar:** Direção visual; framing; escala; sprites; composição; profundidade; luz/sombra; paralaxe; VFX; identidade própria.

**Estado:** Documentação criada; runtime visual pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 3 — Pipeline de arte 2D 🟩 / 🟥

**Objetivo:** Criar pipeline repetível para arte virar assets 2D utilizáveis, preservando a raiz gameplay e preparando a materialização legítima no Unreal.

**Criar e testar:** Proveniência; cleanup; textures; sprites/sprite sheets; Flipbooks; materiais; AssetID; registry; fallbacks; qualidade visual; animação extremamente fluida; identidade isométrica; separação source/runtime.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/PIPELINE CONTRACT.** A especificação operacional completa foi criada. 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS**, conforme decisão de continuar o progresso antes da janela de testes Unreal.

**Evidência principal:** Docs/PHASE_3_2D_ART_PIPELINE.md, commit 295ba3a1a03797aa2a6265255cbd2f02c29415e9.

**Resultado:** lifecycle canônico de arte definido; AssetID/proveniência; preparação e derivação 2D; regras de Sprite/Sprite Sheet/Flipbook; contrato de animação; composição isométrica; profundidade; integração com Asset Manager/visual registry; fallback; automação futura; e gates P0–P6 definidos. Nenhum Unreal binary foi fabricado.

**Testes/validação:** testes que exigem Unreal foram deliberadamente adiados. A conclusão verde desta fase refere-se somente ao nível repository/source/contract; não significa que Textures, Sprites, Flipbooks ou materiais reais já existam no Unreal.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 4 — Sistema de personagens 2D 🟩 / 🟥

**Objetivo:** Adaptar o personagem gameplay existente para representação 2D sem reconstruir a raiz de gameplay.

**Criar e testar:** Idle/Walk/Run/Attack/Hit/Death/Cast/Interaction; perfil 2D; Flipbooks; escala/offset; transições; integração com AAetherCharacter; compatibilidade com classe/evolução/equipment; qualidade e fluidez.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** Foi implementada uma camada 2D aditiva sobre AAetherCharacter, mantendo o sistema visual existente. 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** Docs/PHASE_4_2D_CHARACTER_SYSTEM.md, commit 8cc61bf515e4dd68663828af123870750dc28807.

**Implementação principal:** UAether2DCharacterVisualProfile, UAether2DCharacterVisualComponent, EAether2DCharacterVisualState, dependência Paper2D e integração no AAetherCharacter. Movimento alimenta Idle/Walk/Run; ataque alimenta Attack; ataques não-looping retornam a Idle quando o Flipbook termina.

**Arquitetura preservada:** gameplay, movimento, combate, networking, autoridade, classes/evoluções e equipamento não foram reconstruídos nem substituídos. A apresentação 2D é opcional e ativada por perfil.

**Testes/validação:** validação de source/arquitetura realizada por inspeção e consistência dos arquivos modificados. Build/UHT/PIE/runtime e materialização dos assets Unreal ficam deliberadamente para a janela de testes Unreal.

### FASE 5 — Criaturas, NPCs e chefes 2D 🟩 / 🟥

**Objetivo:** Adaptar a apresentação visual de criaturas, NPCs e chefes existentes para a direção premium 2D isométrica, sem reconstruir AI, combate, quests, loot, autoridade ou qualquer sistema de gameplay já existente.

**Criar e testar:** Perfil visual 2D; componente visual reutilizável; classificação Creature/NPC/Boss; FamilyID; estados Idle/Walk/Run/Attack/Hit/Death/Cast/Interaction; integração orientada por estado; variações data-driven; qualidade visual; compatibilidade com servidor/autoria; materialização posterior no Unreal.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_5_2D_CREATURES_NPCS_BOSSES.md`.

**Implementação principal:** `UAether2DLivingVisualProfile` e `UAether2DLivingVisualComponent`. A camada é actor-agnostic e pode ser anexada a atores existentes de criatura, NPC ou boss. O gameplay continua sendo a fonte de verdade; a camada visual apenas apresenta estados.

**Resultado:** criada uma base única para criaturas, NPCs e chefes, evitando três sistemas visuais duplicados. O perfil suporta identidade estável, família visual, escala, offset, Flipbooks por estado e escolha explícita da apresentação 2D primária. O componente cria a apresentação Paper2D, dirige Idle/Walk/Run pela velocidade e expõe estados explícitos para ataque, hit, morte, cast e interação.

**Preservação:** AI, combat, damage, skills, loot, quests/events, navigation, spawning, persistence, networking e server authority não foram reconstruídos.

**Qualidade:** o contrato permanente de máxima qualidade visual e animação 2D extremamente fluida continua obrigatório para todo conteúdo de produção.

**Testes/validação:** não foram declarados UHT/UBT/PIE/runtime. Conforme decisão operacional, Unreal fica deliberadamente para a janela posterior de testes.


### FASE 6 — Ambientes e props 2D 🟩 / 🟥

**Objetivo:** Criar uma biblioteca modular e reutilizável de cenário 2D isométrico premium.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_6_2D_ENVIRONMENTS_AND_PROPS.md`.

**Implementação principal:** `UAether2DEnvironmentVisualProfile` e `UAether2DEnvironmentVisualComponent`.

**Resultado:** criado contrato data-driven para Architecture, Road, Vegetation, Rock, Water, Ruin, Dungeon, Prop e Interactive, com VisualProfileID, FamilyID, Sprite, escala, offset, render layer e sombra opcional. O componente aplica esses dados através de Paper2D sem executar apresentação no Dedicated Server.

**Regra de produção:** ambientes devem ser compostos por famílias reutilizáveis, e não por imagens únicas descartáveis. Isso permite combinar casas, muralhas, portas, torres, templos, estradas, pontes, vegetação, rochas, água, ruínas, dungeons, fogueiras, móveis, decoração e props interativos em múltiplas regiões.

**Preservação:** collision, navigation, interaction logic, destruction, economy, quests, combat, spawning, persistence, networking e world streaming existentes não foram reconstruídos.

**Qualidade:** máxima qualidade visual, perspectiva isométrica consistente, silhuetas limpas, iluminação coerente, modularidade e consistência entre famílias permanecem obrigatórias.

**Testes/validação:** não foram declarados UHT/UBT/PIE/runtime. Unreal permanece deliberadamente para a janela posterior de validação.


### FASE 7 — Profundidade 2D, luz e sombra 🟩 / 🟥

**Objetivo:** Criar a fundação de profundidade visual para a apresentação 2D isométrica sem exigir um mundo 3D.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_7_2D_DEPTH_LIGHTING_SHADOWS.md`.

**Implementação principal:** `UAether2DDepthLightingProfile` e `UAether2DDepthLightingComponent`.

**Resultado:** criada política data-driven de camadas Background/Far/World/Midground/Foreground/Overlay, ordenação de renderização, offset de profundidade, sombra opcional e paralaxe opcional. A paralaxe usa fator normalizado e permanece exclusivamente na apresentação.

**Preservação:** movement, collision, navigation, combat, AI, quests, inventory, economy, persistence, networking e world streaming existentes não foram reconstruídos.

**Extensibilidade:** a arquitetura deixa espaço para oclusão de foreground, atmosfera, neblina, clima, partículas, VFX e materiais de iluminação nas etapas de materialização e produção.

**Qualidade:** profundidade deve reforçar leitura isométrica, hierarquia visual, silhuetas, oclusão, iluminação natural e acabamento premium.

**Testes/validação:** não foram declarados UHT/UBT/PIE/runtime. Unreal permanece deliberadamente para a janela posterior de validação.

### FASE 8 — Câmera e exploração isométrica 🟩 / 🟥

**Objetivo:** Adaptar a exploração à apresentação 2D isométrica sem reconstruir movimento, targeting, interação, networking ou gameplay.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_8_2D_ISOMETRIC_CAMERA_EXPLORATION.md`.

**Implementação principal:** `UAether2DIsometricCameraProfile`, `UAether2DIsometricCameraComponent` e `EAether2DIsometricCameraMode`.

**Resultado:** criada política data-driven para câmera isométrica fixa/orbitável, yaw/pitch, distância, zoom limitado, lag e collision test do spring arm. O componente atua como camada de apresentação sobre a câmera existente.

**Compatibilidade:** a solução foi desenhada para exploração de cidades, wilderness, estradas, dungeons e landmarks dentro da arquitetura de mundo contínuo e aproximadamente 40 grandes cidades/jurisdições.

**Preservação:** movement, targeting, interaction, collision, navigation, combat, networking, multiplayer, persistence e world streaming existentes não foram reconstruídos.

**Testes/validação:** não foram declarados UHT/UBT/PIE/runtime. Unreal permanece deliberadamente para a janela posterior de validação.
### FASE 9 — Primeira região permanente 🟩 / 🟥

**Objetivo:** Definir e preparar a primeira região permanente do mundo definitivo como a primeira unidade de produção reutilizável do mundo contínuo de aproximadamente 40 grandes cidades/jurisdições.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_9_FIRST_PERMANENT_REGION.md`.

**Implementação/resultado:** a fase agora possui identidade permanente, composição espacial não-linear, zonas, gramática de streaming cells, pontos de mundo, hooks de interação, quests/events, criaturas/NPCs, economia, combate e persistência, além da gramática visual reutilizável para assentamento, natureza, fronteira, ruínas e aproximação da dungeon. O contrato utiliza as estruturas existentes de `FAetherMapDefinition`, `FAetherWorldZoneDefinition`, `FAetherStreamingCellDefinition`, `FAetherWorldPointDefinition`, `FAetherMapConnectionDefinition`, `FAetherWorldActorPlacementDefinition` e `FAetherWorldInteractionDefinition`, sem criar um segundo sistema de mundo.

**Composição canônica:** arrival → settlement → plaza/services → outskirts → gate → road → countryside → forest edge → exploration → combat frontier → ruins/landmark → dungeon approach → dungeon entrance.

**Regra de produção:** a região não é um mapa descartável nem um corredor. Ela possui rota principal, rotas secundárias e descobertas e foi preparada para escalar para as demais jurisdições.

**Direção visual:** a interpretação atual é premium 2D isométrica. Os documentos históricos da primeira região foram preservados como evidência de composição/produção e reconciliados com a direção 2D atual.

**Documentação relacionada:** `Content/Aether/Maps/README.md`, `Docs/FIRST_PLAYABLE_WORLD_REGION_PRODUCTION_SPECIFICATION.md`, `Docs/FIRST_PLAYABLE_ISOMETRIC_DIORAMA_LAYOUT.md` e `Docs/FIRST_PLAYABLE_ISOMETRIC_DIORAMA_ASSET_KIT.md`.

**Testes/validação:** nenhum Unreal runtime foi falsamente declarado. `.umap`, `.uasset`, importação de arte, collision, navigation, exploração, combate, multiplayer, PIE, Dedicated Server e validação visual permanecem deliberadamente para a janela posterior de Unreal.


### FASE 10 — Integração dos sistemas existentes 🟩 / 🟥

**Objetivo:** Conectar a apresentação premium 2D à raiz de gameplay existente sem reconstruir os sistemas já presentes.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_10_GAMEPLAY_PRESENTATION_INTEGRATION.md`.

**Implementação principal:** a apresentação de class/evolution agora aceita um perfil 2D opcional; o sistema de equipment aceita `PaperSprite` opcional por slot e pode anexá-lo à apresentação 2D; skill presentation alimenta o estado `Cast`; basic attack já alimenta `Attack`; e a câmera isométrica expõe uma política explícita FixedIsometric/Orbit, recebendo o zoom existente quando o perfil 2D está ativo.

**Integração preservada:** progression, classes/evolutions, movement, combat, skills/effects, inventory, items/equipment, loot, quests/events, creatures/NPC/AI, economy/crafting, persistence, networking, multiplayer, world/streaming e UI continuam sendo sistemas existentes e autoritativos. Nenhum desses sistemas foi recriado.

**Equipment 2D:** o contrato agora permite representação visual por PaperSprite, com escala, offset e camada de renderização data-driven. Isso é apresentação; propriedade, slot, regras de equipamento e autoridade permanecem no sistema existente.

**Skill 2D:** o cast aciona o estado visual `Cast` quando uma apresentação 2D estiver disponível. O impacto não força `Hit` no caster, evitando confundir apresentação do atacante com o estado do alvo.

**Camera/input:** FixedIsometric bloqueia free-look quando o perfil 2D está ativo; Orbit mantém free-look permitido. O zoom existente passa pelo componente isométrico quando configurado.

**Testes/validação:** nenhuma UHT/UBT/PIE/runtime foi falsamente declarada. Materialização de assets, class/evolution switching real, equipment layering real, skill presentation, combate, multiplayer e qualidade visual permanecem para a janela posterior de Unreal.

### FASE 11 — Primeiro inimigo e loop de combate 🟩 / 🟥

**Objetivo:** Estabelecer no nível repository/source/architecture o primeiro inimigo da primeira região e a ponte entre o combat service existente e o domínio de criaturas, sem criar um segundo sistema de combate.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_11_FIRST_ENEMY_COMBAT_LOOP.md`.

**Primeiro inimigo canônico:** Wild Hound — Cão Selvagem da Fronteira.

**IDs de produção:** `AOA.Creature.FirstRegion.WildHound`, `AOA.Creature.WildHound`, `AOA.Loot.FirstRegion.WildHound`, `AOA.Reward.FirstRegion.WildHound` e `AOA.Spawn.FirstRegion.WildHound.Frontier`.

**Implementação:** criada `FAetherCreatureCombatAdapter`, reutilizando `FAetherCombatService` para jogador→criatura e criatura→jogador. `AAetherCreatureActor` agora possui `ApplyCombatDamage` e snapshot de definição, e a definição de criatura aceita perfil 2D opcional.

**Apresentação 2D:** criaturas podem receber `UAether2DLivingVisualComponent`; dano alimenta `Hit`, derrota alimenta `Death` e reset alimenta `Idle`.

**Preservação:** combat, progression/XP, inventory, items, loot/reward, respawn, quests/events, AI, networking, multiplayer, persistence e world/streaming continuam sendo os sistemas existentes. Nenhum sistema paralelo de combat ou reward foi criado.

**Loop-alvo:** exploração → encontro → target → ataque/skill → dano → derrota → XP → loot → continuidade. A ligação real entre derrota e os serviços de XP/loot/persistence será validada na janela posterior de Unreal.

**Testes/validação:** não foram declarados UHT/UBT/PIE/runtime. Nenhum asset binário Unreal foi fabricado. A fase foi fechada no nível source/repository/architecture conforme a decisão de continuar o progresso enquanto Unreal está deferido.

**Commits principais da fase:** `9d81c39f53d8132b946436e56458eef252beef61`, `04124f67dfc793d8f0f7a98f9a7a80672dcb7409`, `9b64005bb508b07fa439a06caebdefb4328cdcf6`, `b477fa8af80b8ae686e241d638c5a7c0fbb30f62`, `402f4335ad9f4f4c9053e2c9f04605fb68df4e00`, `643dde8746c90f11fa2707a562da73b36cb19ba0`, `ab1cf7002e680e4a801d3381385bc9cea5496f8e`, `13d659fadfb11ebf7cb0e4e2917972a6152c7732` e `4dd32c1d35f8b243b2382832f2487d67f4304d47`.


### FASE 12 — Primeira dungeon 🟩 / 🟥

**Objetivo:** Estabelecer no nível repository/source/architecture a primeira dungeon permanente, integrada ao mundo contínuo e aos sistemas existentes, sem recriar combat, quests, loot, progression ou streaming.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_12_FIRST_DUNGEON.md`.

**Primeira dungeon canônica:** The Hollowed Watch — A Torre Vigia Oca.

**IDs:** `AOA.Dungeon.FirstRegion.HollowedWatch`, `AOA.Map.FirstRegion.HollowedWatch`, `AOA.Portal.FirstRegion.DungeonEntrance`, `AOA.Quest.FirstRegion.HollowedWatch`, `AOA.Loot.FirstRegion.HollowedWatch` e `AOA.Reward.FirstRegion.HollowedWatch`.

**Arquitetura criada:** `FAetherDungeonDefinition`, `FAetherDungeonRoomDefinition`, `FAetherDungeonEncounterDefinition`, `FAetherDungeonRegistry`, `UAetherDungeonCatalog` e `UAetherDungeonSubsystem`, com validação de identidade, níveis, salas, encounters, referências e grafo.

**Composição:** Entrance → Vestibule → BrokenGallery → AbandonedBarracks → Crypt → GuardianHall → LowerTower → BossChamber → Exit, com rotas secundárias para exploração e descoberta.

**Combate:** reutiliza `FAetherCombatService` e `UAetherCreatureSubsystem`. Mini-boss `AOA.Creature.FirstRegion.HollowedWatchGuardian` e boss `AOA.Creature.FirstRegion.HollowedWatchWarden` são contratos de conteúdo, não um novo sistema de boss.

**Integração:** dungeon aponta para quest, loot, reward, inventory, persistence, interaction e world streaming existentes. Configuração conceitual usa LoadMode Instance e a zona `Region.FirstPermanent.Dungeon`.

**Visual:** segue a pipeline premium 2D, com profundidade/camadas, sombras, iluminação, VFX e animação fluida. Nenhum asset binário Unreal foi fabricado.

**Testes/validação:** não foram declarados UHT/UBT/PIE/runtime. Entrada/saída, navigation, collision, combate, mini-boss, boss, loot, quest, persistence, multiplayer e qualidade visual permanecem para a janela posterior de Unreal.

**Próxima fase:** Fase 13 — Vertical Slice.

### FASE 13 — Vertical slice 🟩 / 🟥

**Objetivo:** Consolidar no nível repository/source/architecture uma primeira experiência completa: assentamento → quest → exploração → combate → recompensa → dungeon → boss → retorno.

**Estado:** 🟩 **CONCLUÍDA NO NÍVEL REPOSITORY/SOURCE/ARCHITECTURE.** 🟥 **MATERIALIZAÇÃO E VALIDAÇÃO REAL NO UNREAL DEFERIDAS.**

**Evidência principal:** `Docs/PHASE_13_VERTICAL_SLICE.md`.

**Contrato canônico:** `AOA.VerticalSlice.FirstPermanent`.

**Arquitetura criada:** `FAetherVerticalSliceStageDefinition`, `FAetherVerticalSliceDefinition`, `FAetherVerticalSliceRegistry`, `UAetherVerticalSliceCatalog` e `UAetherVerticalSliceSubsystem`, incluindo validação de oito etapas, unicidade, ordem e identidades de integração.

**Factory canônica:** `FAetherVerticalSliceDefinition::CreateFirstPermanentSlice()`.

**Fluxo:** Settlement → Quest → Exploration → Combat → Reward → Dungeon → Boss → Return.

**Integrações:** Quest usa `FAetherQuestService`; combate usa `FAetherCombatService` + `FAetherCreatureCombatAdapter`; criaturas usam `UAetherCreatureSubsystem`; dungeon usa `UAetherDungeonSubsystem`; world/streaming, progression, inventory/items, loot/reward, persistence e multiplayer permanecem nos sistemas existentes.

**IDs principais:** `AOA.Quest.FirstRegion.FirstHunt`, `AOA.Creature.FirstRegion.WildHound`, `AOA.Loot.FirstRegion.WildHound`, `AOA.Dungeon.FirstRegion.HollowedWatch`, `AOA.Creature.FirstRegion.HollowedWatchWarden` e `AOA.Quest.FirstRegion.HollowedWatch`.

**Preservação:** nenhum novo combat, quest, reward, progression, inventory, world, streaming, networking, multiplayer ou persistence system foi criado.

**Testes/validação:** não foram declarados UHT/UBT/PIE/runtime. Não foram fabricados .uasset/.umap ou outros binários Unreal. Materialização, exploração, combate, dungeon, boss, retorno, HUD, áudio, VFX, multiplayer e persistence permanecem para a janela posterior de Unreal.

**Próxima fase:** Fase 14 — Primeiro teste real jogável.

### FASE 14 — Primeiro teste real jogável 🟨

**Objetivo:** preparar a primeira validação real da vertical slice no Unreal/PC, sem declarar runtime PASS antes da execução.

**Estado:** 🟨 **PREPARADA PARA VALIDAÇÃO REAL NO UNREAL / PC.** 🟥 **RUNTIME AINDA NÃO VALIDADO.**

**Documento canônico:** `Docs/PHASE_14_FIRST_PLAYABLE_TEST.md`.

**Escopo de validação:** sincronização oficial → UHT/UBT → startup → personagem/câmera → assentamento → quest → exploração → Wild Hound → XP/loot/reward → entrada na Hollowed Watch → dungeon → Warden → retorno → persistence → 2-client → Dedicated Server → logs → qualidade visual → performance inicial.

**Regra de evidência:** cada gate deve ser classificado como 🟩 PASS, 🟥 FAIL, 🟨 BLOCKED ou ⬜ NOT RUN, com commit, configuração, resultado e evidência real. Caminhar/explorar fisicamente os mapas faz parte da validação; apenas carregar/buildar mapa não é suficiente.

**Regra de correção:** falhas devem ser corrigidas no GitHub, sincronizadas no PC e revalidadas, com regressão dos gates dependentes.

**Preparação concluída:** matriz de testes, critérios de fechamento, ordem de execução, requisitos de evidência e critérios de qualidade/performance foram formalizados. Nenhum teste Unreal foi executado nesta fase.

**Gate de fechamento:** a Fase 14 somente recebe 🟩 após a vertical slice ser realmente percorrida no Unreal e os gates obrigatórios possuírem evidência.


### FASE 15 — World Master Plan 🟥

**Objetivo:** Planejar o único mundo contínuo e suas grandes jurisdições.

**Criar e testar:** ~40 cidades; localizações; fronteiras; biomas; rios; costas; montanhas; estradas; ecologia; recursos; dungeons; landmarks; eventos; viagens.

**Estado:** Planejamento definitivo pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 16 — Expansão das jurisdições 🟥

**Objetivo:** Expandir cidades em grandes regiões exploráveis conectadas.

**Criar e testar:** Cidade → periferia → wilderness → sub-regiões → descobertas → fronteira; conteúdo denso.

**Estado:** Produção pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 17 — Cidades vivas 🟥

**Objetivo:** Criar cidades com identidade visual e comportamento social.

**Criar e testar:** Comerciantes; ferreiros; tavernas; guardas; quest givers; serviços; animais; diálogos; eventos; storytelling.

**Estado:** Conteúdo e validação pendentes.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 18 — Mundo PvE 🟥

**Objetivo:** Escalar criaturas, bosses, dungeons e eventos usando a raiz existente.

**Criar e testar:** Famílias de criaturas; elites; bosses; dungeons; eventos; loot; respawn; quests; descoberta; assets 2D.

**Estado:** Conteúdo em escala pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 19 — Mundo vivo e social 🟥

**Objetivo:** Integrar conteúdo social e eventos ao mundo contínuo.

**Criar e testar:** Party; guild; eventos; invasões; bosses; reputação; economia; ciclos; apresentação 2D.

**Estado:** Integração e validação pendentes.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 20 — Polimento visual 2D premium 🟥

**Objetivo:** Elevar o conteúdo até a qualidade artística final.

**Criar e testar:** Personagens; criaturas; arquitetura; ambientes; sombras; iluminação; VFX; animações; UI; clima; áudio.

**Estado:** Produção e polish pendentes.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 21 — Escala e performance 🟥

**Objetivo:** Otimizar com evidência real.

**Criar e testar:** FPS; Game/Render/GPU; memória; draw calls; sprites; partículas; streaming; AI; replication; bandwidth; server CPU.

**Estado:** Profiling de escala pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 22 — Conteúdo inicial completo 🟥

**Objetivo:** Produzir a primeira grande entrega sem reconstruir sistemas centrais.

**Criar e testar:** Cidades; regiões; dungeons; criaturas; bosses; NPCs; personagens; quests; equipamentos; crafting; economia; eventos; conteúdo 2D.

**Estado:** Produção pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 23 — Alpha 🟥

**Objetivo:** Consolidar o primeiro produto jogável amplo.

**Criar e testar:** Loop principal; mundo; conteúdo; combate; progressão existente; multiplayer; persistência; estabilidade; blockers.

**Estado:** Não iniciado.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 24 — Beta 🟥

**Objetivo:** Testar qualidade e estabilidade em escala.

**Criar e testar:** Bugs; balanceamento; performance; UX; economia; multiplayer; segurança; conteúdo; estabilidade.

**Estado:** Não iniciado.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 25 — Release Candidate 🟥

**Objetivo:** Provar build final reproduzível e segura.

**Criar e testar:** Build limpa; cliente; servidor; persistência; recuperação; patch/update; regressão; segurança; performance; conteúdo.

**Estado:** Não iniciado.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 26 — Lançamento e evolução contínua 🟥

**Objetivo:** Lançar e continuar produzindo conteúdo sem reescrever a raiz.

**Criar e testar:** Novas regiões; cidades; dungeons; criaturas; bosses; personagens; eventos; histórias; assets 2D; regressão contínua.

**Estado:** Não iniciado.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

## Gate atual

**FASE 1 — FUNDAÇÃO REAL DO UNREAL 🟥 — RUNTIME DEFERIDO**

Os gates de UHT/UBT/PIE/multiplayer/server e demais validações reais continuam pendentes e não bloqueiam o progresso repository/source.


**Regra operacional:** continuar o progresso repository/source enquanto os gates de runtime estiverem explicitamente deferidos; nunca transformar um gate deferido em PASS.


## Regra de fechamento

Uma fase só recebe 🟩 quando todos os itens obrigatórios da fase foram criados e testados no nível declarado. Se qualquer item obrigatório estiver faltando, permanece 🟥.

Ao fechar: atualizar a fase → registrar evidências → atualizar roadmap quando necessário → atualizar este arquivo → commit no GitHub → só então avançar.

## Regra de verdade

Nunca declarar runtime PASS sem execução real no Unreal. Nunca fabricar .uasset, .umap, FBX ou outro binário. Nunca transformar source complete em game complete.

## Documentos canônicos

- ROADMAP_OPEN_WORLD_ISOMETRIC.md — roadmap oficial por fases.
- Docs/AGE_OF_AETHER_2D_ISOMETRIC_VISUAL_DIRECTION.md — direção visual 2D.
- PROJECT_MEMORY/00_CONTINUITY.md — este arquivo.


## F40 — PRIMEIRA MATERIALIZAÇÃO VISUAL ISOMÉTRICA

**Estado repository/source:** 🟩 IMPLEMENTADO.

**Estado Unreal runtime:** 🟨 AGUARDANDO VALIDAÇÃO REAL.

A validação real da Fase 14 revelou que a câmera ainda estava livre/perspectiva e que o mapa continha essencialmente o chão de desenvolvimento, sem personagem visual ou composição ambiental suficiente.

Foi implementada no GitHub a primeira ponte legítima de materialização visual:
- fallback runtime de câmera isométrica fixa quando não há DataAsset de câmera;
- rotação absoluta da câmera para impedir que a orientação do personagem gire a visão;
- zoom isométrico;
- personagem runtime visível usando primitivas e materiais nativos do Unreal;
- primeira composição visual runtime com praça, estrada, casas, árvores, água, rochas, portão e landmark;
- nenhuma fabricação de .uasset, .umap, FBX ou textura binária.

Documento: Docs/PHASE_40_RUNTIME_ISOMETRIC_VISUAL_FOUNDATION.md.

**Regra:** F40 só poderá receber PASS de runtime depois de recompilar, abrir a primeira região, executar PIE e caminhar fisicamente pela composição no Unreal, com evidência.


## F41 — CORREÇÃO DA APARÊNCIA RUNTIME

**Evidência real:** após sincronizar `00e7c65` e recompilar com UBT (`UBT_EXIT=0`), o usuário executou a primeira região no Unreal. A câmera, personagem e composição do mapa passaram a aparecer, mas o personagem e os elementos ambientais apareceram como primitivas brancas/sem aparência visual aplicada.

**Diagnóstico:** a ponte F40 atribuía o parâmetro `BaseColor` ao `BasicShapeMaterial` nativo. A instância runtime deve receber o parâmetro `Color` para aplicar as cores configuradas.

**Correção F41:** atualizado o visual runtime do personagem e o materializador da primeira região para usar `Color`. Nenhum asset binário foi fabricado.

**Estado:** correção em GitHub aguardando sincronização, recompilação e nova validação real no Unreal. A F40/F41 ainda não recebe PASS visual final. Depois da correção imediata, a produção deve avançar para sprites/texturas/camadas/iluminação/VFX reais, mantendo a direção 2D isométrica premium.


## F41 — PRIMEIRO KIT DE ARTE 2D REAL

Após a validação real da ponte F40/F41, ficou comprovado que a geometria nativa serve apenas como fallback técnico: personagem e ambiente já aparecem, mas ainda não representam a qualidade visual final. A produção foi avançada para a primeira materialização de arte 2D real sem reconstruir a arquitetura existente.

Foi criado `Content/Aether/Art/2D_ASSET_MANIFEST.json` com AssetIDs canônicos para Player Mage (Idle/Walk/Attack), árvore, rocha e casa. Também foi criado `Docs/PHASE_41_FIRST_REAL_2D_ART_KIT.md` definindo a produção, qualidade, proveniência, materialização legítima no Unreal e critérios de evidência.

**Estado:** 🟩 contrato/source do kit definido; 🟥 imagens-fonte reais ainda precisam ser produzidas/ingressadas e materializadas no Unreal. Nenhum `.uasset`, `.umap`, Sprite ou Flipbook binário foi fabricado.

**Próxima prioridade:** produzir o primeiro conjunto visual real e então validar a importação Paper2D, perfis, escala, anchors, ordenação, animação e caminhada na primeira região.

## F42 — PRIMEIRA SUBSTITUIÇÃO REAL DAS PRIMITIVAS POR ARTE 2D

**Mudança oficial:** a ponte F40/F41 foi evoluída para uma materialização 2D dentro do próprio módulo Unreal. O projeto agora habilita explicitamente o plugin Paper2D e possui uma biblioteca `FAetherRuntime2DArt` que cria texturas transitórias e `UPaperSprite` para personagem, árvores, rochas e casas.

**Mapa:** `AetherDevelopmentWorldActor` deixou de usar cilindro + esfera como representação final de árvores, esfera como rocha e cubo + cone como casa. Esses elementos agora são instanciados como sprites 2D; chão, estrada, água, portão e demais elementos estruturais continuam como geometria de suporte.

**Personagem:** o corpo/cabeça/manto geométricos F40 passam a ficar ocultos quando o sprite 2D runtime é criado. A apresentação é uma imagem 2D com cabeça, cabelo, braços, pernas, botas, manto e espada, mantendo o `AAetherCharacter` e toda a lógica de gameplay existentes.

**Estado:** 🟩 implementação source oficial concluída; 🟨 validação real no Unreal pendente. O resultado visual ainda precisa ser julgado no PC: escala, orientação isométrica, leitura à distância, transparência, sorting e aparência premium. Não declarar PASS de runtime antes dessa validação.

**Nota:** estes sprites são uma ponte de materialização 2D procedural para eliminar os placeholders geométricos. A evolução posterior poderá substituir as texturas transitórias por arte-fonte premium importada, sem alterar os contratos de gameplay.


## F42 — CORREÇÃO DE COMPILAÇÃO REAL NO PC

Após sincronizar a correção da declaração de `AddSpriteArt(...)`, o segundo UBT real da F42 foi executado no PC oficial.

**PC:** `D:\Nova pasta (4)\Projeto Age of Aether\Age-Of-Aether-github-main`

**Commit testado:** `5cee6e4` (F42: record first PC compile failure and correction).

**Comando:** UnrealBuildTool `AgeOfAetherEditor Win64 Development`, projeto oficial, `-NoHotReloadFromIDE`.

**Evidência real:** UBT executou 6 ações: 3 compilações de módulos, link da biblioteca, link da DLL e WriteMetadata. Resultado: **Succeeded**. Tempo total: **19.02 s**. `UBT_EXIT` foi encerrado após o resultado de sucesso.

**Estado F42:** 🟩 **COMPILAÇÃO REAL PASSOU.** 🟨 **VALIDAÇÃO RUNTIME NO UNREAL AINDA PENDENTE.**

Próximo gate obrigatório: abrir a primeira região no Unreal, executar PIE e verificar os sprites Paper2D runtime de personagem, árvores, rochas e casas, incluindo escala, orientação isométrica, transparência, sorting, legibilidade e caminhada/exploração física. Nenhum PASS visual/runtime deve ser declarado antes dessa evidência.

**Preservação:** os arquivos locais modificados/não rastreados do usuário não foram alterados, limpos ou resetados.


## F43 — Runtime Paper2D persistence correction
Real PIE evidence: only geometric support primitives remained visible after F42; intended Paper2D environment art disappeared.
Diagnosis: AddSpriteArt created dynamic UPaperSpriteComponent instances and registered them, but did not add them as Actor instance components or retain them in a UPROPERTY collection. Primitive components were retained separately.
Correction: AetherDevelopmentWorldActor now calls AddInstanceComponent(Component) and retains sprite components in RuntimeSpriteComponents.
Correction commits: 8b816177f13586ceecce757da0779795c05bbc03 and 55c86540b22011de08180b4a48247735abeb88c7.
Status: runtime correction committed; real PC compile and runtime revalidation pending.
Next gate: pull main, compile AgeOfAetherEditor Win64 Development, launch Unreal, run PIE on AetherWorld_FirstRegion, and verify character/tree/rock/house sprites persist and are explorable.
Preserve all existing local modified and untracked files.


## F44 — runtime 2D resource lifetime/render hardening
- User real-PC validation reported no visible change after F43: runtime still showed the previous geometric presentation instead of the intended Paper2D sprites.
- Repository audit found F42/F43 created runtime UTexture2D resources with reused explicit names for procedural variants and did not explicitly force the transient texture resource update.
- F44 hardens the runtime bridge by using unique transient texture creation, disabling mip generation/filtering for crisp pixel data, calling UpdateResource(), retaining generated UPaperSprite and source UTexture2D objects in transient UPROPERTY ownership on the world actor/character, and forcing sprite component visibility/render-state refresh.
- This remains a procedural runtime bridge, not final premium art.
- Real PC validation required: pull latest main, compile with UE 5.8 UBT, open the first-region map, PIE, verify sprites are actually visible and persistent, then physically walk/explore. No runtime PASS until observed evidence.


## F44 — real PC compile correction
- Real PC compilation after pull of F44 failed with `UBT_EXIT=6` because `AetherCharacter.cpp` invoked `UPaperSprite::GetSourceTexture()` while only a forward declaration existed in the public header.
- Exact compiler error: `AetherCharacter.cpp(223,35): error C2027: uso de tipo indefinido 'UPaperSprite'`.
- GitHub-first correction: added the concrete `PaperSprite.h` include to `AetherCharacter.cpp`.
- Correction commit: `3caf882d479d325b349ef347c31dd1366fd062f3`.
- Runtime validation remains blocked until this compile succeeds and the F44 PIE result is observed.


## F45 — Explicit Paper2D runtime material correction
- Real PC validation after F44: UBT passed, but the user reported that PIE still showed no visible change; procedural Paper2D sprites remained absent while prior geometry remained.
- Repository audit found runtime sprite creation via FSpriteAssetInitParameters was not explicitly assigning a Paper2D sprite material.
- F45 GitHub-first correction: AetherRuntime2DArt now explicitly loads /Paper2D/DefaultSpriteMaterial.DefaultSpriteMaterial and assigns it through Params.DefaultMaterialOverride before InitializeSprite().
- This targets the confirmed Paper2D rendering pipeline: sprite assets require a sprite material for visible rendering; UE 5.8 documents DefaultSpriteMaterial as the unlit Paper2D sprite material.
- Runtime remains unvalidated until the user pulls, recompiles, opens Unreal, enters PIE, and physically explores the first region.


## F46 — Paper2D runtime texture-source initialization correction
- Real PC F45 compilation passed, but user runtime validation reported no intended 2D character/tree/rock/house art; only the small ground/support geometry and residual geometric forms were visible.
- Root cause identified in the runtime sprite bridge: UTexture2D::CreateTransient does not provide the source-image metadata expected by the Paper2D sprite initialization path. Without explicit source dimensions/format, generated sprites can have invalid source/UV data and fail to render even though the texture object and sprite object exist.
- GitHub-first correction: AetherRuntime2DArt now initializes Texture->Source with the generated canvas width/height and BGRA8 source format before updating the texture resource and constructing the UPaperSprite.
- Correction commit: 332021b6d482987ed47a56cca400d0d52ec20f85.
- This is still a procedural runtime bridge, not final premium art.
- Required next evidence: pull main, compile with UE 5.8 UBT, open AetherWorld_FirstRegion, run PIE, verify character/tree/rock/house sprites are visible, correctly oriented/scaled, persistent, and physically explorable. No runtime PASS until observed.
