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
Prioridade: 1. Player Mage; 2. árvore; 3. rocha; 4. casa; 5. expansão das famílias de vegetação, arquitetura, criaturas, NPCs, dungeon e VFX.

A prioridade é provar uma família visual completa e reutilizável antes de multiplicar dezenas de assets inconsistentes.