# AGE OF AETHER — HISTORICAL 2D ISOMETRIC VISUAL DIRECTION

> **Visual target updated (2026-10-04):** the project owner supplied a reference for a bright, richly composed **stylized 3D isometric diorama** (castle, river/bridge, village, farms and forest). That reference now governs the first-region world presentation. This document preserves the earlier Paper2D pipeline as an optional presentation path for characters, effects and future content; it is no longer the primary environment target. See `Docs/FIRST_REGION_ISOMETRIC_DIORAMA_RUNTIME_PROTOTYPE.md`.

## Objetivo original

Transformar a apresentação do AGE OF AETHER em um RPG de exploração isométrica inspirado na experiência de RPGs isométricos clássicos, utilizando conteúdo visual predominantemente 2D sobre a arquitetura sistêmica já existente do projeto.

A tecnologia visual não é uma obrigação de 3D. Personagens, criaturas, arquitetura, vegetação, props e ambientes podem ser produzidos como imagens, sprites, sprite sheets, animações 2D, camadas, paralaxe, sombras, iluminação e VFX. Elementos 3D só devem ser usados quando trouxerem benefício comprovado e não são requisito do projeto.

A meta é obter uma apresentação premium, rica e convincente, com profundidade visual simulada por composição, luz, sombra, perspectiva, camadas e efeitos.

## Regra principal: adaptar, não reconstruir

O AGE OF AETHER já possui os sistemas de gameplay e infraestrutura necessários. Não criar novamente:

- skills;
- progression/XP/levels;
- inventory;
- items/equipment;
- loot/reward/respawn;
- combat;
- quests/events;
- creatures/AI contracts;
- economy/crafting;
- persistence;
- networking/server authority;
- multiplayer;
- UI framework;
- world/streaming contracts;
- registries e contratos de conteúdo já existentes.

Esses sistemas serão reutilizados e adaptados à nova apresentação 2D somente quando a integração exigir alterações reais.

Regra: preservar -> adaptar -> integrar -> validar -> expandir.

## Pipeline visual alvo

Concept/arte original
-> imagem ou sprite source
-> organização/cleanup
-> Texture
-> Sprite/Sprite Sheet
-> Flipbook quando animado
-> material 2D/efeitos
-> perfil/Asset Registry existente
-> consumidor de gameplay existente
-> Unreal runtime.

## Princípios visuais

- isometria clara e consistente;
- composição inspirada em RPGs isométricos clássicos;
- personagens e criaturas 2D com animações coerentes;
- ambientes ricos e detalhados;
- iluminação e sombras para criar profundidade;
- paralaxe e camadas quando contribuírem para a leitura;
- oclusão e sobreposição coerentes;
- VFX e partículas para magia, clima e ambiente;
- materiais e pós-processamento quando ajudarem a integrar os elementos;
- escala, silhueta e contraste suficientes para gameplay;
- qualidade artística premium, sem aparência de placeholder;
- identidade visual própria, sem copiar arte, mapas ou personagens de outros jogos.

## Conteúdo que a pipeline deve suportar

- personagens;
- classes e evoluções visualmente distintas;
- criaturas;
- NPCs;
- chefes;
- casas e arquitetura;
- estradas;
- árvores e vegetação;
- rochas;
- rios e água representada visualmente;
- ruínas;
- dungeons;
- props;
- ícones;
- efeitos mágicos;
- elementos ambientais;
- mapas e composições de regiões;
- animações por sprite/Flipbook;
- variações de iluminação e clima.

## Limites e verdade técnica

Não fabricar .uasset, .umap ou outros binários Unreal falsos.

Uma imagem gerada não é automaticamente um asset Unreal jogável. A materialização deve ocorrer por importação e/ou criação legítima no Unreal, seguida de validação real.

Nenhuma fase runtime será declarada concluída sem evidência local real.

## Critério de sucesso

O jogador deve perceber:

personagem 2D + cenário 2D + luz + sombra + camadas + animação + VFX + composição isométrica = mundo com profundidade e presença.

A técnica utilizada é secundária perante a experiência final.
