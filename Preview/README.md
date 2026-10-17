# Age of Aether — prévia jogável WEB SLICE 0.5

Esta pasta contém uma fatia vertical Canvas 2D para navegador com um **overworld cartográfico de 100 km × 100 km**, teleporte para cidades/áreas de caça, combate, mobs de farm, inventário/equipamento, mascote, asas, VFX e duas jornadas opcionais. É um protótipo de apresentação e gameplay; **não é um build nem uma validação do Unreal Engine**.

## Como abrir

Sirva a pasta com qualquer servidor HTTP estático. Por exemplo, na raiz do repositório:

```bash
python3 -m http.server 4173 --bind 0.0.0.0 --directory Preview
```

Depois, abra `http://localhost:4173` ou a prévia ao vivo disponibilizada pelo Workspace. A página atual usa `?v=0.5.0` nos recursos CSS/JS para evitar cache antigo.

## Controles

- **WASD** ou **setas**: mover; segure **Shift** para correr.
- **Clique no chão**: caminhar; clique em um mob para selecionar e aproximar-se.
- **E**: conversar, entregar/coletar itens ou ativar um marco quando estiver perto.
- **1**: Raio Arcano; **2**: Explosão de Éter; **3**: Sopro Vital; **H**: poção de vida.
- **M** ou **MAPA**: abrir o mapa do mundo; **Escape**: fechar; escolha uma cidade/área de caça e use **TELEPORTAR**.
- **P**: pausar/retomar. Use os botões do HUD para abrir inventário/equipamento e ficha do personagem.

## Overworld e teleporte

O mapa-múndi tem limites de 0–100 km em cada eixo, grade de 10 km, regiões de terreno, rotas e marcador do jogador. Há dez destinos: as cidades de **Aetherfall** e **Lunária**, mais oito áreas de caça. Selecione um destino no mapa ou na lista lateral e confirme o teleporte. Um clique livre cria uma rota/waypoint; não teleporta para coordenadas arbitrárias.

As camadas **Região Aetherfall** e **Células Lógicas** preservam o mapa regional anterior, com nove zonas canônicas, 12 células lógicas e 22 pontos de interesse. A cidade e as áreas de caça materializam a cena Canvas local existente em pontos de entrada próprios; a coordenada global é de apresentação e não representa 100 km de terreno contínuo percorrível nesta versão.

## Farm, XP e quests opcionais

Cada um dos oito pontos de farm começa com **10 mobs ativos**. Cada mob abatido retorna individualmente após **3 segundos**. Os mobs perseguem/atacam, podem ser selecionados, concedem XP e ouro e têm chance de deixar equipamento, materiais ou poções no chão. Pressione **E** perto de um drop para guardá-lo.

A progressão de nível por XP vem do farm. As duas histórias da Vigia Elara e da Oca da Vigília são conteúdo opcional de jornada: mortes/entregas de missão **não concedem XP**; as recompensas são moedas e itens.

## Inventário, equipamentos, mascote e asas

O inventário usa o catálogo de itens local. Armas, armaduras, botas, mantos e anéis têm ações de equipar/remover e modificam dano, vida, mana ou velocidade. A Raposa de Brasa acompanha Aelwyn e ajuda no combate; asas equipadas aparecem no personagem e aumentam a velocidade. Itens de missão e drops de farm também aparecem na bolsa.

As duas cidades têm mais construções, torres/portão, mercado, fonte, guilda e moradores animados. As magias têm efeitos de trajetória/impacto/cura; a subida de nível mostra um burst dourado e banner animado.

## Animações por sprites

Os atlas transparentes em `assets/spritesheets/` são carregados localmente, sem CDN:

- **Aelwyn:** Idle, Walk e Cast.
- **Vigia Elara:** Idle e Talk.
- **Cão Selvagem:** Idle, Run e Attack.
- **Wisp de Éter:** Idle, Glide e Pulse.
- Mobs de farm reutilizam as animações de criatura com variações visuais; a raposa, asas, magia e level-up têm apresentação Canvas.

As folhas usam células 384×320 em 4×3 (Mage e mobs) ou 4×2 (Elara). `assets/spritesheets/manifest.json` registra células, linhas, FPS, loop e fontes. Alguns frames de Idle/hold reutilizam poses; não é um conjunto final desenhado quadro a quadro ou com oito direções. Fontes e PNGs preparados ficam em `ArtSource/Generated/Animations/` e `Content/Aether/Art/ImportReady/Animations/`; o packer está em `Tools/Art/build_sprite_sheets.py`.

## Verificação e limites

Contrato do mapa:

```bash
node Tools/Map/test_first_region_map.js
```

A validação automatizada deste slice também usa `node --check` e `git diff --check`; o smoke de interação foi feito com DOM/Canvas simulado. Não há conta, servidor de jogo, persistência, dependência de CDN ou validação de aparência em navegador gráfico real nesta execução. Atualizar a página reinicia a aventura.

As coordenadas e a jogabilidade são da prévia web. **Não** criam `.umap`/`.uasset`, não executam World Partition, Data Layers, colisão/NavMesh, terreno Unreal de 100 km, multiplayer ou autoridade de servidor. Importação Paper2D, Flipbooks/perfis, Unreal 5.8 build, PIE e aceitação runtime permanecem pendentes e não foram executados neste ambiente.
