# F42 — PRIMEIRA SUBSTITUIÇÃO DAS PRIMITIVAS POR ARTE 2D

## Objetivo
A primeira região não deve parecer um conjunto de formas geométricas. F42 substitui os placeholders mais visíveis por sprites 2D materializados dentro do runtime Unreal usando o Paper2D existente.

## O que mudou
- Paper2D explicitamente habilitado no projeto.
- Nova biblioteca `FAetherRuntime2DArt`.
- Personagem: sprite 2D com cabeça, cabelo, braços, pernas, botas, manto e espada.
- Árvores: tronco, ramificações e múltiplos grupos de folhagem com variações de iluminação.
- Rochas: silhueta irregular com facetas e musgo.
- Casas: paredes, estrutura de madeira, telhado, portas e janelas.
- O `AetherDevelopmentWorldActor` instancia esses elementos como `UPaperSpriteComponent`.
- A lógica existente de movimentação, combate, quests, inventário, progressão e networking não foi reconstruída.

## Estado
**Source:** implementado no `main`.

**Runtime:** ainda não validado. O primeiro teste deve verificar se o Paper2D aparece corretamente na câmera isométrica, se os sprites ficam apoiados no chão e se a orientação/escala estão coerentes.

## Regra
O teste de runtime deve ser feito no PC real após sincronização e compilação. Só depois de abrir a primeira região, executar PIE e caminhar pelo mapa será possível classificar a qualidade visual.

## Próxima evolução
Depois da validação, substituir o art procedural de ponte por fontes visuais premium importadas como texturas reais, preservando o mesmo contrato de Sprite/Flipbook e sem alterar os sistemas de gameplay.