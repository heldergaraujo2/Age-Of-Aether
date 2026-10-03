# AGE OF AETHER — OPEN WORLD ISOMETRIC RPG ROADMAP

## Objetivo
Transformar o AGE OF AETHER existente em um RPG de mundo aberto com identidade visual Stylized Painterly Isometric / 2.5D Hand-Painted Diorama, preservando a arquitetura MMORPG, os sistemas server-authoritative e o conteúdo já implementado.

Fonte de verdade: este repositório.
Princípio: preservar -> adaptar -> integrar -> validar -> expandir.

## FASE 0 — AUDITORIA DA BASE
Mapear Unreal, build, Automation, GameInstance/GameMode/GameState, PlayerController/PlayerState/Character, networking, accounts, progression, items, inventory, equipment, combat, world, quests, social, economy, persistence, security, AI, registries, asset pipeline, visual foundation, playable character, mapas e documentação.
Classificar tudo como PRESERVAR, ADAPTAR, CORRIGIR, SUBSTITUIR ou CRIAR.
Gate: nenhuma grande alteração de arquitetura antes da matriz de dependências e riscos.

## FASE 1 — FUNDAÇÃO REAL DO UNREAL
Fechar UHT, UBT Editor/Game, Dedicated Server, Editor startup, mapa de desenvolvimento, PIE, 2-client PIE, Automation, logs e replicação básica.
Resultado: personagem placeholder jogável em multiplayer básico.

**Estado atual:** SOURCE PREPARATION READY / REAL UNREAL RUNTIME PENDING.

A preparação e o checklist desta fase estão documentados em:
`Docs/PHASE_1_REAL_UNREAL_FOUNDATION.md`

A fase só será marcada como COMPLETE após evidência local real de UHT/UBT, Editor, mapa, PIE, 2-client PIE, Dedicated Server, replicação, Automation e logs.

## FASE 2 — DIREÇÃO VISUAL ISOMÉTRICA
Definir câmera ortográfica/isométrica, ângulo, zoom, escala, paleta, materiais, iluminação, sombras, pós-processamento, proporções, props, arquitetura, vegetação, personagens e VFX.
Regra: aparência de diorama pintado à mão sem perder jogabilidade 3D no Unreal.

**Estado:** SOURCE/DOCUMENTATION COMPLETE / UNREAL RUNTIME PENDING.
Documentação: `Docs/PHASE_2_ISOMETRIC_VISUAL_DIRECTION.md`.
A direção foi formalizada para câmera, composição, materiais, iluminação, personagens, arquitetura, props, VFX e integração com o mundo contínuo. A calibração/aceitação final permanece dependente da auditoria no ambiente real do Unreal.

## FASE 3 — PIPELINE DE ARTE REAL
Estabelecer Concept -> source/asset -> cleanup -> import -> Unreal asset -> material/skeleton/collision -> Asset Registry -> Stable AssetID -> runtime.
Validar FBX, Skeletal/Static Mesh, materiais, texturas, animações, Physics Assets, sockets, LOD/Nanite quando apropriado, VFX, ícones, origem/licença e fallbacks.

**Estado:** SOURCE/DOCUMENTATION COMPLETE / UNREAL RUNTIME PENDING.
Documentação: `Docs/PHASE_3_REAL_ART_PIPELINE.md`.
A fase formaliza o lifecycle de assets, provenance/licença, AssetID/dependencies/fallbacks, política de LOD/Nanite/collision/sockets, requisitos painterly premium e o primeiro asset de aceitação. A aceitação real continua dependente do Unreal local.

## FASE 4 — CÂMERA E EXPLORAÇÃO ISOMÉTRICA
Implementar câmera, zoom, pan, rotação se desejada, collision, movimento relativo à câmera, seleção, targeting, interação e controles.
Gate: exploração confortável e consistente em multiplayer.

**Estado:** SOURCE/DOCUMENTATION COMPLETE / UNREAL RUNTIME PENDING.
Documentação: `Docs/PHASE_4_ISOMETRIC_CAMERA_EXPLORATION.md`.
A fase reutiliza a fundação source-level da Phase 40 e formaliza câmera isométrica, zoom, collision/occlusion, movimento relativo à câmera, framing, seleção/targeting/interação, multiplayer e compatibilidade com o mundo contínuo. A aceitação real permanece dependente da auditoria no Unreal local.

## FASE 5 — PRIMEIRO DIORAMA JOGÁVEL
Construir uma pequena vila com praça, casas, loja, NPC, estrada, floresta, campo, obstáculos, vegetação, iluminação e props de storytelling.

## FASE 6 — NÚCLEO RPG JOGÁVEL
Validar level, XP, atributos, HP, recurso, ataque básico, dano, morte, respawn, equipamento, inventário e loot.

## FASE 7 — PRIMEIRO INIMIGO E COMBATE
Adicionar um inimigo comum com AI, aggro, perseguição, ataque, hit reaction, morte, XP, drop e respawn.
Gate: explorar -> combater -> vencer -> receber XP/loot -> continuar.

## FASE 8 — PRIMEIRA DUNGEON
Construir entrada, áreas, inimigos, obstáculos, loot, mini-boss/boss e retorno.

## FASE 9 — VERTICAL SLICE COMPLETO
Fluxo obrigatório: entrar -> explorar vila -> sair -> explorar mundo -> combater -> ganhar XP -> loot -> dungeon -> boss -> voltar.
Este é o primeiro trecho reconhecível como o jogo final.

## FASE 10 — PRIMEIRO TESTE REAL JOGÁVEL
Executar no Unreal: início sem erro crítico, câmera isométrica, personagem, movimento, exploração, interação, combate, inimigo, XP, loot, dungeon, boss, retorno à vila, HUD, Automation relevante, 2-client PIE, Dedicated Server smoke test e Output Log limpo de erros críticos.
GO: expandir mundo. NO-GO: corrigir fundação.

## FASE 11 — WORLD MAP
Criar estrutura de mundo aberto com cidade principal, vilas, campos, florestas, montanhas, rios, ruínas, áreas corrompidas, desertos, dungeons, boss arenas, estradas e pontos de interesse.
Usar World Partition, Landscape, Data Layers, HLOD, streaming, NavMesh, zones e spawn system.

## FASE 12 — CIDADES VIVAS
NPC schedules, comerciantes, ferreiros, quest givers, guardas, tavernas, serviços, animais, eventos, diálogos e storytelling ambiental.

## FASE 13 — MUNDO PvE
Famílias de monstros, elites, bosses, zonas por nível, loot tables, respawns, eventos, world bosses, dungeons, quests e recompensas.

## FASE 14 — PROGRESSÃO MMORPG
Integrar classes, 25 evoluções, skills, equipamentos, enhancement, crafting, economy, quests, party, guild, social e progression.

## FASE 15 — MUNDO VIVO
World events, invasões, bosses, ciclos, eventos sazonais, mudanças de estado, economia, zonas PvP, zonas seguras, descobertas e reputação.

## FASE 16 — POLIMENTO ARTÍSTICO
Polir materiais, iluminação, sombras, vegetação, arquitetura, props, personagens, VFX, UI, animações, composição e storytelling ambiental.

## FASE 17 — ESCALA E PERFORMANCE
Medir FPS, Game/Render Thread, GPU, memória, streaming, draw calls, actors, AI, replication, bandwidth, server CPU e World Partition. Otimizar somente com profiling real.

## FASE 18 — MULTIPLAYER REAL
Validar 1/2/múltiplos jogadores, party, guild, combate simultâneo, loot, eventos, bosses, dungeons, troca, crafting, reconnect, logout e restart do servidor.

## FASE 19 — CONTEÚDO INICIAL COMPLETO
Cidade principal, múltiplas regiões e dungeons, bosses, classes/evoluções, equipamentos, skills, quests, NPCs, crafting, economia, social, eventos e world bosses.

## FASE 20 — ALPHA
Loop principal completo, mundo explorável, combate, progressão, conteúdo, multiplayer, persistência e ausência de blockers críticos.

## FASE 21 — BETA
Balanceamento, bugs, performance, UX, conteúdo, economia, estabilidade, multiplayer e segurança.

## FASE 22 — RELEASE CANDIDATE
Build limpa, servidor, cliente, persistência, patch/update, recuperação de falhas, segurança, performance, conteúdo e regressão.

## FASE 23 — LANÇAMENTO E EVOLUÇÃO
Novos mapas, classes/evoluções, dungeons, bosses, eventos, temporadas e regiões sem reescrever os sistemas centrais.

## REGRA CENTRAL
Não construir o MMORPG inteiro antes de provar o jogo.
Sequência: base existente -> Unreal real -> câmera isométrica -> diorama -> personagem -> exploração -> RPG -> combate -> dungeon -> vertical slice -> PRIMEIRO TESTE REAL -> mundo aberto -> MMORPG em escala.

## RELAÇÃO COM ROADMAPS EXISTENTES
ROADMAP.md continua sendo a referência da arquitetura MMORPG e execução técnica.
ROADMAP_CONTENT_AND_CLIENT.md continua sendo a referência de conteúdo, dados e cliente.
ROADMAP_VISUAL_AND_PLAYABLE.md continua sendo a referência de implementação visual/runtime.
Este documento define a ordem de transformação do produto para Open World + Isometric 2.5D + Painterly Diorama.

## ESTADO DA FASE 0
**COMPLETE at repository/source audit level.**

A auditoria completa e a matriz de transformação estão registradas em `Docs/PHASE_0_OPEN_WORLD_ISOMETRIC_AUDIT.md`.

Decisão central: preservar a arquitetura MMORPG/server-authoritative existente e transformar principalmente câmera, mundo, assets, materiais, iluminação, animação, VFX e apresentação.

Requisito visual agora oficial: **Stylized Painterly Isometric / 2.5D Hand-Painted Diorama em qualidade gráfica premium**.

Benchmark visual inicial: **Vila → estrada → floresta → área de combate → entrada de dungeon**, pequeno em escala, mas produzido já próximo do padrão visual final.

## OPEN WORLD — SINGLE CONTINUOUS WORLD REQUIREMENT

The final world is **one continuous open-world map**, not a collection of disconnected gameplay maps.

Target scale:
- approximately **40 major cities** distributed across the same world;
- each city is a geographic, cultural and gameplay center;
- every city has a surrounding **jurisdiction/biome region**;
- the biome, creatures, NPCs, resources, weather, architecture, VFX and environmental storytelling of that jurisdiction must visually and mechanically belong to its city.

"One map" is a player/world continuity requirement, not a requirement that every world cell be loaded simultaneously. World Partition, streaming and HLOD may divide the world internally while preserving one world identity, continuous geography, persistent regional state and seamless normal overworld traversal.

Before final terrain production, create a World Master Plan with 40 city locations/identities, jurisdiction boundaries, biome transitions, roads, rivers/coasts, mountains, ecological/resource distribution, dungeons/landmarks, progression bands and travel times.

The first playable slice must be a region of the eventual single-world architecture, not a throwaway isolated map.

## REGRA DE ESCALA — CIDADES + JURISDIÇÕES DE EXPLORAÇÃO

Cada uma das aproximadamente 40 cidades deve ser o núcleo de uma **grande região explorável**:

**cidade/safezone → periferia → wilderness → sub-regiões → áreas de descoberta → fronteira natural → próxima jurisdição/cidade**

Cada jurisdição deve justificar longas expedições de exploração/caça. A distância entre grandes cidades pode exigir horas de caminhada dependendo da rota, terreno, perigos e descobertas. Não preencher a escala com terreno vazio.

Não fixar ainda números de progressão, força de monstros ou qualidade de equipamentos por cidade. Esses sistemas serão definidos nas fases apropriadas.

## CONTROLE DE CONTINUIDADE

A cada fase/gate concluído:
1. atualizar documentação da fase;
2. registrar evidência real;
3. atualizar este roadmap;
4. atualizar `PROJECT_MEMORY/00_CONTINUITY.md`;
5. registrar o commit;
6. somente então avançar para a próxima fase.

**Fase documental avançada: PHASE 4 — CÂMERA E EXPLORAÇÃO ISOMÉTRICA — SOURCE/DOCUMENTATION COMPLETE / RUNTIME PENDING.**

**Phase 4:** SOURCE/DOCUMENTATION COMPLETE / RUNTIME PENDING.
Documentação: `Docs/PHASE_4_ISOMETRIC_CAMERA_EXPLORATION.md`.

**Phase 2:** SOURCE/DOCUMENTATION COMPLETE / RUNTIME PENDING.

**Phase 3:** SOURCE/DOCUMENTATION COMPLETE / RUNTIME PENDING.
Documentação: `Docs/PHASE_3_REAL_ART_PIPELINE.md`.

**Gate runtime ainda aberto:** PHASE 1 — REAL UNREAL FOUNDATION / AUDITORIA NO AMBIENTE REAL DO UNREAL PENDENTE.

A Phase 2 foi fechada somente em nível de source/documentação; nenhuma validação visual/runtime foi inventada.
