# AGE OF AETHER — FASE 41 — PRIMEIRO KIT DE ARTE 2D REAL

## Objetivo
Substituir progressivamente as primitivas geométricas da ponte F40 por arte 2D isométrica real, mantendo intactos gameplay, mundo, combate, progressão, networking e persistence.

A F41 não considera as primitivas como arte final. Elas permanecem apenas como fallback técnico até que os assets reais sejam materializados e validados.

## Primeiro kit obrigatório
1. Personagem jogável com anatomia legível, braços, pernas e silhueta completa.
2. Árvore 2D com volume, folhas, tronco e transparência limpa.
3. Rocha 2D com iluminação e contato com o chão.
4. Casa/arquitetura 2D com escala coerente com o personagem.

Para o personagem, o kit inicial prevê Idle, Walk e Attack. O sistema existente continua responsável por escolher o estado; a arte apenas apresenta o estado.

## Direção artística
- RPG isométrico 2D premium.
- Leitura 3/4/isométrica consistente.
- Acabamento realista/premium, não formas geométricas.
- Silhueta forte à distância.
- Transparência limpa.
- Iluminação coerente entre famílias.
- Base/feet anchor consistente.
- Escala relativa consistente.
- Sem perspectiva incompatível entre assets.
- Profundidade por camadas, sombra, VFX e parallax quando apropriado.
- 3D continua opcional e não é requisito.

## Manifesto canônico
`Content/Aether/Art/2D_ASSET_MANIFEST.json` define os primeiros AssetIDs, famílias, representação Paper2D, estados, destino e fallback.

O manifesto é contrato de produção. Ele não afirma que os assets Unreal já existem.

## Proveniência
Cada imagem que entrar na produção precisa registrar se é original, gerada, comissionada ou licenciada.

Uma imagem gerada é fonte de produção e ainda precisa passar por preparação, derivação 2D, importação/materialização legítima no Unreal e validação.

## Pipeline
conceito/fonte → preparação → derivação → manifesto/metadata → Paper2D no Unreal → perfil visual → runtime → evidência

Não será criado `.uasset`, `.umap`, Sprite ou Flipbook binário fora do Unreal.

## Critério de avanço
A F41 só será considerada materializada quando o PC conseguir sincronizar o commit oficial, compilar, abrir a primeira região, materializar os assets reais no Unreal, atribuir os perfis Paper2D existentes, executar PIE, mostrar personagem e ambiente sem depender das primitivas F40, caminhar pela região e registrar evidência.

Até lá, o estado correto é SOURCE READY / RUNTIME MATERIALIZATION PENDING.

## Próxima produção
Prioridade: 1) Player Mage; 2) árvore; 3) rocha; 4) casa; 5) expansão das famílias de vegetação, arquitetura, criaturas, NPCs, dungeon e VFX.

## Progress update — 2026-10-17

The first generated source-art pass now contains Player Mage Idle, Wild Hound Idle, and a First Region settlement house. Source/provenance lives in `ArtSource/Generated/` and `Content/Aether/Art/GENERATED_ART_PROVENANCE.md`; transparent import-ready PNG derivatives and their manifest entries are under `Content/Aether/Art/ImportReady/` and `Content/Aether/Art/2D_ASSET_MANIFEST.json`.

A deterministic matte-preparation tool and source-art validator/tests were added in `Tools/Art/`. The initial three-source pass validated real RGBA alpha and three preparation tests.

## Follow-up — sprite-sheet animation slice

Phase 57 adds generated Mage Walk/Cast, Elara Idle/Talk, and Wild Hound Run/Attack key poses, plus three row-based atlases. The browser preview plays these rows; see `Docs/PHASE_57_SPRITE_SHEET_ANIMATION_SLICE.md` and the updated provenance/manifest.

**Current status remains SOURCE READY / UNREAL IMPORT PENDING.** Browser animation is implemented, but UE Paper2D Sprite/Flipbook asset creation, visual-profile mapping, PIE review and Unreal runtime acceptance are still outstanding. Complete eight-direction movement, Hit/Death and the remaining tree/rock/building/NPC/mob families are also still future work. No Unreal runtime acceptance is claimed.

A prioridade é provar uma família visual completa e reutilizável antes de multiplicar dezenas de assets inconsistentes.