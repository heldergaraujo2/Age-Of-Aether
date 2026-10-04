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


### FASE 6 — Ambientes e props 2D 🟥

**Objetivo:** Criar biblioteca reutilizável de cenário.

**Criar e testar:** Casas; arquitetura; estradas; vegetação; rochas; água; ruínas; dungeons; props; objetos interativos.

**Estado:** Biblioteca real pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 7 — Profundidade 2D, luz e sombra 🟥

**Objetivo:** Criar profundidade convincente sem exigir 3D.

**Criar e testar:** Camadas; render order; oclusão; sombras; iluminação; paralaxe; partículas; neblina; clima; pós-processamento.

**Estado:** Prova visual/runtime pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 8 — Câmera e exploração isométrica 🟥

**Objetivo:** Adaptar movimento, câmera, zoom, seleção e interação existentes.

**Criar e testar:** Câmera; zoom; framing; movimento relativo; collision/occlusion; targeting; interação; multiplayer; performance.

**Estado:** Runtime pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 9 — Primeira região permanente 🟥

**Objetivo:** Construir a primeira região real do mundo definitivo.

**Criar e testar:** Assentamento; praça; serviços; NPCs; saída; estrada; campo; floresta; exploração; combate; landmark; dungeon.

**Estado:** Especificação existe; conteúdo real pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 10 — Integração dos sistemas existentes 🟥

**Objetivo:** Conectar a camada 2D à raiz sem reconstruí-la.

**Criar e testar:** Progression; classes; skills; combat; inventory; equipment; loot; quests; NPCs; creatures; economy; persistence; multiplayer.

**Estado:** Integração runtime pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 11 — Primeiro inimigo e loop de combate 🟥

**Objetivo:** Provar o loop completo com a apresentação 2D.

**Criar e testar:** Explorar → encontrar → atacar → dano → skill → derrota → XP → loot; logs e multiplayer.

**Estado:** Prova runtime pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 12 — Primeira dungeon 🟥

**Objetivo:** Criar dungeon integrada ao mundo e à linguagem visual 2D.

**Criar e testar:** Entrada; áreas; salas; obstáculos; criaturas; interações; loot; eventos; mini-boss; boss; retorno.

**Estado:** Produção e validação pendentes.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 13 — Vertical slice 🟥

**Objetivo:** Provar uma fatia completa reconhecível do Age of Aether.

**Criar e testar:** Assentamento → quest → exploração → combate → XP/loot → dungeon → boss → retorno; UI/áudio/VFX.

**Estado:** Execução real pendente.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

### FASE 14 — Primeiro teste real jogável 🟥

**Objetivo:** Aceitar o primeiro slice somente após jogar e observar o fluxo no Unreal.

**Criar e testar:** Startup; câmera; personagem; exploração; NPCs; combate; skills; XP; loot; dungeon; boss; HUD; áudio; VFX; 2-client; server smoke; logs; profiling.

**Estado:** Gate não concluído.

**Evidência:** registrar commits, arquivos, testes, logs, execuções e resultados reais aplicáveis.

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

**FASE 5 — CRIATURAS, NPCs E CHEFES 2D 🟩 SOURCE / 🟥 RUNTIME**

A arquitetura visual para criaturas, NPCs e bosses foi implementada no repository/source. A materialização dos assets Paper2D e a validação real no Unreal permanecem deliberadamente deferidas.

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
