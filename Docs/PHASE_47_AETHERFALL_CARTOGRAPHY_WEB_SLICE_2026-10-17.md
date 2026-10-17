# Phase 47 — Cartografia de Aetherfall na prévia web

**Data:** 2026-10-17<br>
**Escopo deste registro:** incremento de apresentação e gameplay local em `Preview/`; não é aceitação do mapa Unreal.

## Entregue na prévia

- Fonte cartográfica de apresentação em `Preview/world-map-data.js`, com `AOA.Region.FirstPermanent`, os **nove IDs canônicos de zona**, **12 células lógicas** e pontos/rotas referenciados aos limites locais da prévia. `wilderness-south` é um ponto dentro de `Wilderness`, não uma décima zona.
- Cartografia SVG interativa com assentamento, arredores, campos, orla da floresta, mata, fronteira de combate, ruínas e indicação futura da abordagem/entrada da masmorra; também apresenta topografia, rio, estrada principal, trilhas secundárias, rochedos, vegetação e pontos de interesse.
- POIs de roadmap para área elite, arena de boss, PvP/evento e expansão; todos aparecem explicitamente planejados, sem interação ou área de combate implementada.
- Alternância entre terreno e células lógicas; localização/coordenadas do personagem; marcadores de POI e estado planejado; janela responsiva e controles de teclado **M**/**Escape**.
- Waypoint por clique no mapa, objetivo ou POI; indicação de rota no mapa, minimapa e mundo Canvas, com opção para limpar. A rota é uma linha de orientação, **sem pathfinding, teleporte ou transição de mapa**.
- O alvo da cartografia e `drawQuestMarkers` agora usam a mesma resolução de objetivo. A verificação inclui Elara, Cão Selvagem, fragmento móvel, Oca da Vigília, Wisp móvel e Núcleo de Vigília; o fragmento é identificado pelo tipo real `wild-fragment`.
- Posições do início, NPC, inimigos, ruína, vegetação, rochas e estradas da prévia são lidas dos dados cartográficos. O ponto de revive continua sendo o ponto de chegada definido nesses mesmos dados.

## O que ainda não foi materializado

A prévia cobre visualmente a região inicial e seu percurso jogável atual, mas não satisfaz todo o escopo de mapas da Phase 47. A abordagem/entrada da masmorra, serviços, área elite, arena de boss, PvP/evento e expansão aparecem apenas como POIs planejados; não há gameplay, regras de zona ou área construída para esses marcadores. Conteúdo/fluxo reais de dungeon também não foram implementados nesta fatia.

`world-map-data.js` é deliberadamente uma fonte de apresentação do navegador: suas coordenadas locais **não são transforms Unreal**. O arquivo declara `unrealMaterialized: false`. Nenhum `.umap`/`.uasset` foi inventado. A criação dos mapas, Landscape, buildings, foliage, collision, NavMesh, World Partition/Data Layers/HLOD, streaming e portais continua a exigir o Unreal Editor; a ordem planejada permanece gameplay blockout antes de decoração.

## Verificações desta fatia

- `node --check Preview/game.js`
- `node --check Preview/world-map-data.js`
- `node Tools/Map/test_first_region_map.js` — passou: 9 zonas, 12 células, pontos dentro dos limites, referências de zona conhecidas e rota principal conectada.
- Smoke de DOM/gameplay com JSDOM e Canvas stub: abertura/fechamento do mapa, camadas, POIs planejados, objetivo compartilhado, drops dinâmicos, waypoint livre, rota do minimapa e limpeza passaram. O smoke foi executado localmente como ferramenta temporária, não é uma suíte de navegador mantida no repositório.
- A superfície SVG foi rasterizada para revisão de contraste/colisões dos rótulos. O CSS contém breakpoints para desktop, tablet e celular; **não houve inspeção em navegador real nem validação Unreal**.
- A prévia HTTP local respondeu 200 para a página, `game.js`, `world-map-data.js` e `styles.css`.

## Próximos passos de aceitação Unreal

1. Materializar primeiro o blockout de gameplay dos mapas e conexões previstos no Roadmap.
2. Alinhar os bounds e identificadores aos contratos canônicos de `AOA.Region.FirstPermanent` e `Region.FirstPermanent`.
3. Validar abertura, links, streaming e Data Layers no editor/PIE, incluindo colisão, navegação, spawns e multiplayer/dedicated server conforme os critérios da Phase 47.
4. Só então avançar a decoração/polimento e atualizar este registro com evidências reais do Unreal 5.8.x.
