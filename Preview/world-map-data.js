/*
 * Browser-only cartographic data for the first permanent-region slice.
 * These positions are presentation/gameplay coordinates, not Unreal transforms.
 * The Unreal .umap, Data Layers and streaming assets must be authored in-editor.
 */
window.AETHER_FIRST_REGION_MAP = {
  mapId: 'AOA.Region.FirstPermanent',
  displayName: 'Aetherfall',
  subtitle: 'Primeira região permanente',
  mapType: 'open-world',
  bounds: { minX: 0, minY: 0, maxX: 18, maxY: 18 },
  coordinateScale: { x: 44, y: 26, offsetX: 54, offsetY: 42, viewWidth: 900, viewHeight: 560 },
  unrealMaterialized: false,
  zones: [
    {
      id: 'AOA.Region.FirstPermanent.Settlement', name: 'Assentamento', shortName: 'AETHERFALL',
      role: 'Praça, serviços e convivência', combat: 'Paz', tone: 'settlement', label: [5.0, 4.25],
      polygon: [[2.7, 2.9], [6.8, 2.6], [8.0, 4.1], [8.0, 6.7], [6.8, 7.6], [3.4, 7.1], [2.5, 5.0]],
    },
    {
      id: 'AOA.Region.FirstPermanent.Outskirts', name: 'Arredores', shortName: 'ARREDORES',
      role: 'Transição entre a praça e a estrada', combat: 'Paz', tone: 'outskirts', label: [7.35, 8.35],
      polygon: [[4.2, 6.7], [7.8, 6.6], [10.0, 8.6], [9.6, 10.3], [7.2, 10.9], [4.7, 9.1], [3.9, 7.7]],
    },
    {
      id: 'AOA.Region.FirstPermanent.Countryside', name: 'Campos do Poente', shortName: 'CAMPOS',
      role: 'Prados, rochedos e trilhas secundárias', combat: 'Paz / PvE controlado', tone: 'countryside', label: [9.1, 14.45],
      polygon: [[6.7, 10.3], [9.5, 9.8], [12.7, 11.4], [13.3, 14.8], [10.8, 17.0], [7.7, 16.3], [6.4, 13.4]],
    },
    {
      id: 'AOA.Region.FirstPermanent.ForestEdge', name: 'Orla da Floresta', shortName: 'ORLA DA FLORESTA',
      role: 'Primeira faixa selvagem', combat: 'PvE', tone: 'forest-edge', label: [14.55, 6.15],
      polygon: [[10.9, 3.9], [14.3, 3.4], [17.4, 4.9], [17.7, 7.7], [15.8, 9.8], [12.6, 10.0], [10.8, 7.7]],
    },
    {
      id: 'AOA.Region.FirstPermanent.Wilderness', name: 'Mata da Fronteira', shortName: 'MATA',
      role: 'Exploração e clareiras', combat: 'PvE', tone: 'wilderness', label: [3.25, 11.1],
      polygon: [[0.6, 7.7], [3.4, 7.3], [5.0, 9.8], [4.7, 12.8], [5.0, 17.2], [4.1, 17.2], [2.5, 14.1], [0.7, 12.4]],
    },
    {
      id: 'AOA.Region.FirstPermanent.CombatFrontier', name: 'Fronteira de Combate', shortName: 'FRONTEIRA',
      role: 'Encontros em espaço aberto', combat: 'PvE', tone: 'frontier', label: [12.0, 12.35],
      polygon: [[10.2, 9.4], [13.1, 9.4], [15.0, 11.1], [14.7, 14.6], [12.2, 15.4], [9.9, 13.4], [9.2, 11.2]],
    },
    {
      id: 'AOA.Region.FirstPermanent.Ruins', name: 'Ruínas da Vigília', shortName: 'RUÍNAS',
      role: 'Marco antigo e descoberta', combat: 'PvE controlado', tone: 'ruins', label: [15.65, 13.65],
      polygon: [[14.0, 11.8], [16.7, 11.4], [17.7, 13.6], [17.2, 16.2], [15.2, 17.1], [14.2, 17.2], [13.8, 15.2]],
    },
    {
      id: 'AOA.Region.FirstPermanent.DungeonApproach', name: 'Abordagem da Masmorra', shortName: 'ABORDAGEM',
      role: 'Transição futura para conteúdo de masmorra', combat: 'PvE', tone: 'dungeon-approach', label: [15.65, 17.0],
      polygon: [[15.2, 15.0], [17.9, 14.8], [18.0, 18.0], [15.2, 18.0]],
    },
    {
      id: 'AOA.Region.FirstPermanent.DungeonEntrance', name: 'Entrada da Masmorra', shortName: 'ENTRADA SELADA',
      role: 'Gancho de integração; não existe na fatia jogável', combat: 'Masmorra', tone: 'dungeon-entrance', label: [17.35, 16.55],
      polygon: [[16.7, 16.5], [18.0, 16.2], [18.0, 18.0], [16.5, 18.0]], planned: true,
    },
  ],
  cells: [
    { id: 'settlement-west', label: 'Assentamento · Oeste', role: 'Settlement-West', col: 0, row: 0 },
    { id: 'settlement-east', label: 'Assentamento · Leste', role: 'Settlement-East', col: 1, row: 0 },
    { id: 'forest-edge', label: 'Orla da Floresta', role: 'Forest-Edge', col: 2, row: 0 },
    { id: 'arrival', label: 'Chegada', role: 'Arrival', col: 0, row: 1 },
    { id: 'settlement-center', label: 'Praça Central', role: 'Settlement-Center', col: 1, row: 1 },
    { id: 'wilderness-north', label: 'Mata · Norte', role: 'Wilderness-North', col: 2, row: 1 },
    { id: 'outskirts', label: 'Arredores', role: 'Outskirts', col: 0, row: 2 },
    { id: 'main-road', label: 'Estrada Principal', role: 'Main-Road', col: 1, row: 2 },
    { id: 'combat-frontier', label: 'Fronteira de Combate', role: 'Combat-Frontier', col: 2, row: 2 },
    { id: 'wilderness-south', label: 'Clareiras do Sul', role: 'Wilderness-South', col: 0, row: 3 },
    { id: 'countryside', label: 'Campos do Poente', role: 'Countryside', col: 1, row: 3 },
    { id: 'ruins-dungeon-approach', label: 'Ruínas · Abordagem', role: 'Ruins-Dungeon-Approach', col: 2, row: 3 },
  ],
  points: [
    { id: 'arrival-spawn', name: 'Ponto de Chegada', kind: 'spawn', icon: '⌖', x: 6.05, y: 5.84, status: 'playable', mapMarker: true, mapList: false, zoneId: 'AOA.Region.FirstPermanent.Settlement' },
    { id: 'settlement-entry', name: 'Entrada de Aetherfall', kind: 'entry', icon: '⌂', x: 5.0, y: 6.85, status: 'playable', mapMarker: true, mapList: false, zoneId: 'AOA.Region.FirstPermanent.Settlement' },
    { id: 'settlement-plaza', name: 'Praça de Aetherfall', kind: 'settlement', icon: '✦', x: 5.55, y: 5.55, status: 'playable', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Settlement' },
    { id: 'market', name: 'Mercado', kind: 'service', icon: '▦', x: 3.9, y: 4.45, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Settlement' },
    { id: 'forge', name: 'Forja', kind: 'service', icon: '⚒', x: 7.6, y: 4.8, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Settlement' },
    { id: 'tavern', name: 'Taverna', kind: 'service', icon: '⌂', x: 4.0, y: 6.25, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Settlement' },
    { id: 'quest-hub', name: 'Vigia Elara', kind: 'quest', icon: '!', x: 6.25, y: 4.72, status: 'playable', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Settlement' },
    { id: 'settlement-gate', name: 'Portão Norte', kind: 'gate', icon: '⌑', x: 9.0, y: 9.0, status: 'playable', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Outskirts' },
    { id: 'road-frontier', name: 'Estrada da Fronteira', kind: 'road', icon: '⌁', x: 11.15, y: 11.0, status: 'playable', mapMarker: true, mapList: false, zoneId: 'AOA.Region.FirstPermanent.Countryside' },
    { id: 'forest-entry', name: 'Orla da Floresta', kind: 'forest', icon: '♣', x: 13.5, y: 9.15, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.ForestEdge' },
    { id: 'wilderness-north', name: 'Clareira Norte', kind: 'exploration', icon: '◇', x: 3.2, y: 10.5, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Wilderness' },
    { id: 'wilderness-south', name: 'Trilha das Clareiras', kind: 'exploration', icon: '◇', x: 4.2, y: 15.8, status: 'planned', mapMarker: true, mapList: false, zoneId: 'AOA.Region.FirstPermanent.Wilderness' },
    { id: 'combat-frontier', name: 'Território de Caça', kind: 'combat', icon: '◆', x: 13.1, y: 12.8, status: 'playable', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.CombatFrontier' },
    { id: 'hound-hunt', name: 'Cão Selvagem', kind: 'quest-target', icon: '◆', x: 13.1, y: 12.8, status: 'quest-locked', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.CombatFrontier' },
    { id: 'ruins-landmark', name: 'Oca da Vigília', kind: 'landmark', icon: '⌑', x: 16.0, y: 15.7, status: 'playable', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Ruins' },
    { id: 'wisp-nest', name: 'Wisp de Éter', kind: 'quest-target', icon: '✧', x: 14.6, y: 16.8, status: 'quest-locked', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Ruins' },
    { id: 'dungeon-approach', name: 'Abordagem da Masmorra', kind: 'future', icon: '▣', x: 16.8, y: 16.8, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.DungeonApproach' },
    { id: 'dungeon-entrance', name: 'Entrada da Masmorra', kind: 'future', icon: '⌑', x: 17.55, y: 17.35, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.DungeonEntrance' },
    { id: 'elite-ground', name: 'Área Elite', kind: 'future', icon: '♜', x: 12.8, y: 13.8, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.CombatFrontier' },
    { id: 'boss-arena', name: 'Arena de Boss', kind: 'future', icon: '♕', x: 16.3, y: 13.6, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Ruins' },
    { id: 'pvp-event-ground', name: 'Área PvP / Evento', kind: 'future', icon: '⚑', x: 2.7, y: 9.5, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.Wilderness' },
    { id: 'expansion-trailhead', name: 'Rota de Expansão', kind: 'future', icon: '⇢', x: 17.1, y: 6.0, status: 'planned', mapMarker: true, mapList: true, zoneId: 'AOA.Region.FirstPermanent.ForestEdge' },
  ],
  paths: [
    {
      id: 'main-road', name: 'Estrada principal', kind: 'main', width: 1.15,
      points: [[3.4, 3.5], [4.2, 4.45], [5.45, 5.85], [6.75, 6.7], [8.2, 8.0], [9.0, 9.0], [10.25, 10.35], [11.55, 11.6], [12.9, 12.85], [14.25, 14.15], [16.0, 15.7]],
    },
    {
      id: 'north-loop', name: 'Trilha da mata', kind: 'secondary', width: 0.62,
      points: [[9.0, 9.0], [9.45, 7.55], [10.8, 6.55], [12.25, 6.7], [13.25, 7.65], [13.75, 9.15], [13.3, 10.65], [12.65, 12.45]],
    },
    {
      id: 'meadow-loop', name: 'Trilha dos prados', kind: 'secondary', width: 0.62,
      points: [[8.9, 9.15], [8.05, 10.45], [8.45, 11.9], [9.75, 13.0], [11.15, 13.55], [12.6, 12.75]],
    },
  ],
  waterway: [[0.5, 10.8], [1.35, 11.65], [1.15, 12.6], [2.4, 13.25], [1.9, 14.35], [2.95, 15.15], [2.65, 16.25], [4.0, 17.45]],
  woodlands: [
    { x: 1.5, y: 4.3, scale: 1.05 }, { x: 2.4, y: 6.1, scale: 0.83 }, { x: 3.2, y: 8.3, scale: 0.96 }, { x: 1.7, y: 9.1, scale: 0.82 },
    { x: 4.2, y: 10.0, scale: 0.86 }, { x: 7.8, y: 9.0, scale: 0.94 }, { x: 9.2, y: 10.0, scale: 1.1 }, { x: 10.0, y: 8.1, scale: 0.92 },
    { x: 11.3, y: 9.2, scale: 1.02 }, { x: 14.7, y: 9.2, scale: 0.98 }, { x: 15.9, y: 11.2, scale: 0.9 }, { x: 10.5, y: 14.3, scale: 0.98 },
    { x: 15.1, y: 14.3, scale: 1.08 }, { x: 8.7, y: 15.8, scale: 0.85 }, { x: 12.0, y: 16.0, scale: 1.1 }, { x: 17.2, y: 12.1, scale: 0.9 },
  ],
  rocks: [
    { x: 8.3, y: 11.2, scale: 0.72 }, { x: 10.2, y: 12.1, scale: 0.56 }, { x: 14.8, y: 12.2, scale: 0.68 },
    { x: 11.1, y: 15.0, scale: 0.52 }, { x: 3.8, y: 12.8, scale: 0.7 },
  ],
};

/*
 * Global travel layer for the browser slice. Coordinates are kilometers on a
 * 100 km × 100 km authored preview map; they are not Unreal world transforms.
 * Each hunting destination materializes one 10-creature local farming camp.
 */
window.AETHER_OVERWORLD_MAP = {
  mapId: 'AOA.Overworld.Aetherfall100',
  displayName: 'Continente de Aetherfall',
  boundsKm: { minX: 0, minY: 0, maxX: 100, maxY: 100 },
  gridStepKm: 10,
  localRegionMapId: 'AOA.Region.FirstPermanent',
  destinations: [
    {
      id: 'aetherfall-city', name: 'Aetherfall', kind: 'city', icon: '✦', xKm: 10, yKm: 12,
      localX: 6.05, localY: 5.84, biome: 'settlement', description: 'Capital murada, praça central, guilda, mercado, estalagem e forja.',
      teleportCost: 0, safe: true,
    },
    {
      id: 'moonwood-hunt', name: 'Bosque da Lua', kind: 'hunt', icon: '♣', xKm: 23, yKm: 19,
      localX: 12.3, localY: 12.3, biome: 'moonwood', spotId: 'moonwood-pack', mobName: 'Lobo da Lua', mobKind: 'hound',
      levelMin: 1, levelMax: 3, mobCount: 10, respawnSeconds: 3, description: 'Clareira de caça para iniciantes · lobos errantes.', teleportCost: 0, safe: false,
      loot: ['wolf-pelt', 'health-potion', 'ether-shard'],
    },
    {
      id: 'sunmeadow-hunt', name: 'Campos do Poente', kind: 'hunt', icon: '◇', xKm: 42, yKm: 28,
      localX: 10.7, localY: 14.1, biome: 'sunmeadow', spotId: 'sunmeadow-boars', mobName: 'Javali do Poente', mobKind: 'boar',
      levelMin: 4, levelMax: 6, mobCount: 10, respawnSeconds: 3, description: 'Pradarias abertas com javalis e saqueadores.', teleportCost: 0, safe: false,
      loot: ['sunstone-ring', 'health-potion', 'wolf-pelt'],
    },
    {
      id: 'crystal-coast-hunt', name: 'Costa de Cristal', kind: 'hunt', icon: '≈', xKm: 17, yKm: 48,
      localX: 3.4, localY: 12.5, biome: 'crystal-coast', spotId: 'crystal-coast-wisps', mobName: 'Wisp da Maré', mobKind: 'wisp',
      levelMin: 7, levelMax: 9, mobCount: 10, respawnSeconds: 3, description: 'Enseada de éter azul com espíritos de maré.', teleportCost: 0, safe: false,
      loot: ['ether-shard', 'tide-cloak', 'health-potion'],
    },
    {
      id: 'ember-highlands-hunt', name: 'Altos de Brasa', kind: 'hunt', icon: '♜', xKm: 74, yKm: 24,
      localX: 14.2, localY: 10.6, biome: 'ember-highlands', spotId: 'ember-highlands-drakes', mobName: 'Draco de Brasa', mobKind: 'drake',
      levelMin: 10, levelMax: 12, mobCount: 10, respawnSeconds: 3, description: 'Encostas vulcânicas patrulhadas por dracos jovens.', teleportCost: 0, safe: false,
      loot: ['ember-wings', 'drake-scale', 'health-potion'],
    },
    {
      id: 'mourning-marsh-hunt', name: 'Brejo Velado', kind: 'hunt', icon: '♧', xKm: 86, yKm: 51,
      localX: 3.7, localY: 12.8, biome: 'mourning-marsh', spotId: 'mourning-marsh-spiders', mobName: 'Aranha do Brejo', mobKind: 'spider',
      levelMin: 13, levelMax: 15, mobCount: 10, respawnSeconds: 3, description: 'Pântano de névoa densa e aranhas venenosas.', teleportCost: 0, safe: false,
      loot: ['marsh-boots', 'poison-sac', 'health-potion'],
    },
    {
      id: 'watch-ruins-hunt', name: 'Ruínas da Vigília', kind: 'hunt', icon: '⌑', xKm: 58, yKm: 68,
      localX: 15.7, localY: 15.1, biome: 'watch-ruins', spotId: 'watch-ruins-guardians', mobName: 'Guardião Rúnico', mobKind: 'golem',
      levelMin: 16, levelMax: 18, mobCount: 10, respawnSeconds: 3, description: 'Pátios antigos ocupados por guardiões de pedra.', teleportCost: 0, safe: false,
      loot: ['watch-cloak', 'rune-staff', 'ether-shard'],
    },
    {
      id: 'glimmergrove-hunt', name: 'Bosque Cintilante', kind: 'hunt', icon: '✧', xKm: 34, yKm: 83,
      localX: 3.3, localY: 9.1, biome: 'glimmergrove', spotId: 'glimmergrove-spirits', mobName: 'Espírito Cintilante', mobKind: 'wisp',
      levelMin: 19, levelMax: 21, mobCount: 10, respawnSeconds: 3, description: 'Bosque feérico onde o éter assume forma viva.', teleportCost: 0, safe: false,
      loot: ['glimmer-wings', 'moonstone-ring', 'ether-shard'],
    },
    {
      id: 'dragonkeep-hunt', name: 'Fortaleza Dracônica', kind: 'hunt', icon: '♕', xKm: 82, yKm: 87,
      localX: 16.2, localY: 16.4, biome: 'dragonkeep', spotId: 'dragonkeep-wyrms', mobName: 'Serpe Dracônica', mobKind: 'drake',
      levelMin: 22, levelMax: 25, mobCount: 10, respawnSeconds: 3, description: 'Campos de elite diante da cidadela dos dragões.', teleportCost: 0, safe: false,
      loot: ['dragon-wings', 'drake-scale', 'rune-staff'],
    },
    {
      id: 'lunaria-city', name: 'Lunária', kind: 'city', icon: '⌂', xKm: 56, yKm: 43,
      localX: 6.05, localY: 5.84, biome: 'moon-city', description: 'Grande cidade portuária com distrito comercial, cais e salão de aventureiros.',
      teleportCost: 0, safe: true,
    },
  ],
  terrain: [
    { id: 'western-woods', name: 'Florestas Ocidentais', xKm: 25, yKm: 28, rxKm: 18, ryKm: 23, tone: 'forest' },
    { id: 'sunmeadow', name: 'Planícies do Poente', xKm: 46, yKm: 28, rxKm: 18, ryKm: 14, tone: 'meadow' },
    { id: 'ember-range', name: 'Cordilheira de Brasa', xKm: 78, yKm: 29, rxKm: 15, ryKm: 22, tone: 'mountain' },
    { id: 'crystal-bay', name: 'Baía de Cristal', xKm: 8, yKm: 50, rxKm: 18, ryKm: 28, tone: 'water' },
    { id: 'mourning-bog', name: 'Brejo Velado', xKm: 84, yKm: 53, rxKm: 17, ryKm: 18, tone: 'marsh' },
    { id: 'ancient-watch', name: 'Terras da Vigília', xKm: 57, yKm: 67, rxKm: 20, ryKm: 17, tone: 'ruins' },
    { id: 'glimmerwild', name: 'Bosque Cintilante', xKm: 33, yKm: 82, rxKm: 21, ryKm: 15, tone: 'magic' },
    { id: 'dragon-reach', name: 'Ermos Dracônicos', xKm: 80, yKm: 84, rxKm: 18, ryKm: 18, tone: 'mountain' },
  ],
};

window.AETHER_ITEMS = {
  'apprentice-focus': { id: 'apprentice-focus', name: 'Foco de Aprendiz', slot: 'weapon', icon: '✧', rarity: 'Comum', description: 'Um foco leve para canalizar magia.', stats: { attack: 5 } },
  'ranger-robe': { id: 'ranger-robe', name: 'Manto do Viajante', slot: 'armor', icon: '◈', rarity: 'Comum', description: 'Tecido reforçado para a primeira jornada.', stats: { hp: 18 } },
  'trail-boots': { id: 'trail-boots', name: 'Botas de Trilha', slot: 'boots', icon: '⌁', rarity: 'Comum', description: 'Couro flexível que acelera os passos.', stats: { speed: 0.3 } },
  'ember-fox': { id: 'ember-fox', name: 'Raposa de Brasa', slot: 'pet', icon: '♧', rarity: 'Raro', description: 'Companheira que segue Aelwyn e ajuda no combate.', stats: { petDamage: 5 } },
  'wind-wings': { id: 'wind-wings', name: 'Asas de Éter', slot: 'wings', icon: '✦', rarity: 'Raro', description: 'Asas luminosas que aumentam a velocidade.', stats: { speed: 0.45 } },
  'health-potion': { id: 'health-potion', name: 'Poção de Vida', slot: 'consumable', icon: '⚗', rarity: 'Comum', description: 'Recupera 48 pontos de vida.', stats: {} },
  'wolf-pelt': { id: 'wolf-pelt', name: 'Pele de Lobo', slot: 'material', icon: '◆', rarity: 'Comum', description: 'Material obtido em caçadas.', stats: {} },
  'ether-shard': { id: 'ether-shard', name: 'Fragmento de Éter', slot: 'material', icon: '✧', rarity: 'Incomum', description: 'Fragmento cintilante encontrado no mundo.', stats: {} },
  'watch-core': { id: 'watch-core', name: 'Núcleo de Vigília', slot: 'quest', icon: '✧', rarity: 'Missão', description: 'Núcleo antigo usado para restaurar o selo da Oca.', stats: {} },
  'sunstone-ring': { id: 'sunstone-ring', name: 'Anel de Pedra Solar', slot: 'ring', icon: '◉', rarity: 'Incomum', description: 'Um anel aquecido pela luz do poente.', stats: { attack: 2, mana: 8 } },
  'moonstone-ring': { id: 'moonstone-ring', name: 'Anel da Lua', slot: 'ring', icon: '◉', rarity: 'Incomum', description: 'Recompensa da jornada da fronteira.', stats: { mana: 12, hp: 4 } },
  'tide-cloak': { id: 'tide-cloak', name: 'Manto da Maré', slot: 'cloak', icon: '◈', rarity: 'Raro', description: 'Manto encantado pela costa de cristal.', stats: { hp: 12, mana: 8 } },
  'ember-wings': { id: 'ember-wings', name: 'Asas de Brasa', slot: 'wings', icon: '✦', rarity: 'Raro', description: 'Asas ígneas que aceleram o movimento.', stats: { speed: 0.55 } },
  'marsh-boots': { id: 'marsh-boots', name: 'Botas do Brejo', slot: 'boots', icon: '⌁', rarity: 'Incomum', description: 'Botas resistentes à lama.', stats: { hp: 8, speed: 0.18 } },
  'watch-cloak': { id: 'watch-cloak', name: 'Manto da Vigília', slot: 'cloak', icon: '◈', rarity: 'Raro', description: 'Recompensa por restaurar as runas antigas.', stats: { hp: 16, mana: 8 } },
  'rune-staff': { id: 'rune-staff', name: 'Bastão Rúnico', slot: 'weapon', icon: '✧', rarity: 'Raro', description: 'Bastão retirado das ruínas da Vigília.', stats: { attack: 12, mana: 10 } },
  'glimmer-wings': { id: 'glimmer-wings', name: 'Asas Cintilantes', slot: 'wings', icon: '✦', rarity: 'Épico', description: 'Asas élficas de grande velocidade.', stats: { speed: 0.7, mana: 10 } },
  'dragon-wings': { id: 'dragon-wings', name: 'Asas Dracônicas', slot: 'wings', icon: '✦', rarity: 'Épico', description: 'Asas poderosas forjadas para as terras altas.', stats: { speed: 0.85, hp: 20 } },
  'drake-scale': { id: 'drake-scale', name: 'Escama de Draco', slot: 'material', icon: '⬟', rarity: 'Incomum', description: 'Uma escama resistente de criatura dracônica.', stats: {} },
  'poison-sac': { id: 'poison-sac', name: 'Saco Venenoso', slot: 'material', icon: '◉', rarity: 'Incomum', description: 'Componente raro das aranhas do brejo.', stats: {} },
};
