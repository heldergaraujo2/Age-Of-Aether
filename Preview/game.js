(() => {
  'use strict';

  const $ = (selector) => document.querySelector(selector);
  const regionMap = window.AETHER_FIRST_REGION_MAP;
  const overworldMap = window.AETHER_OVERWORLD_MAP;
  const itemCatalog = window.AETHER_ITEMS || {};
  if (!regionMap) throw new Error('Aetherfall map presentation data is missing.');
  if (!overworldMap) throw new Error('Aetherfall 100 km overworld data is missing.');
  const pointsById = new Map(regionMap.points.map((point) => [point.id, point]));
  const destinationsById = new Map(overworldMap.destinations.map((destination) => [destination.id, destination]));
  const worldPoint = (id) => {
    const point = pointsById.get(id);
    if (!point) throw new Error(`Unknown Aetherfall map point: ${id}`);
    return { x: point.x, y: point.y };
  };
  const canvas = $('#world');
  const ctx = canvas.getContext('2d', { alpha: false });
  const images = {
    mage: loadImage('assets/player-mage.png'),
    hound: loadImage('assets/wild-hound.png'),
    house: loadImage('assets/settlement-house.png'),
    mageSheet: loadImage('assets/spritesheets/player-mage.png'),
    elaraSheet: loadImage('assets/spritesheets/elara.png'),
    houndSheet: loadImage('assets/spritesheets/wild-hound.png'),
    wispSheet: loadImage('assets/spritesheets/aether-wisp.png'),
  };
  const spriteSheets = {
    mage: {
      frameWidth: 256, frameHeight: 320,
      animations: {
        idle: { row: 0, frames: 4, fps: 4, loop: true },
        walk: { row: 1, frames: 4, fps: 9, loop: true },
        cast: { row: 2, frames: 4, fps: 12, loop: false },
      },
    },
    elara: {
      frameWidth: 256, frameHeight: 320,
      animations: {
        idle: { row: 0, frames: 4, fps: 3.5, loop: true },
        talk: { row: 1, frames: 4, fps: 5, loop: false },
      },
    },
    hound: {
      frameWidth: 384, frameHeight: 320,
      animations: {
        idle: { row: 0, frames: 4, fps: 4, loop: true },
        run: { row: 1, frames: 4, fps: 10, loop: true },
        attack: { row: 2, frames: 4, fps: 12, loop: false },
      },
    },
    wisp: {
      frameWidth: 384, frameHeight: 320,
      animations: {
        idle: { row: 0, frames: 4, fps: 4, loop: true },
        glide: { row: 1, frames: 4, fps: 9, loop: true },
        pulse: { row: 2, frames: 4, fps: 12, loop: false },
      },
    },
  };

  const TILE_W = 76;
  const TILE_H = 38;
  const HALF_W = TILE_W / 2;
  const HALF_H = TILE_H / 2;
  const MAX_WORLD = regionMap.bounds.maxX;
  const startPoint = worldPoint('arrival-spawn');
  const initialDestination = destinationsById.get('aetherfall-city');
  const questNpc = worldPoint('quest-hub');
  const houndSpawn = worldPoint('hound-hunt');
  const hollowedWatch = worldPoint('ruins-landmark');
  const wispSpawn = worldPoint('wisp-nest');
  const keys = new Set();
  let width = 0;
  let height = 0;
  let dpr = 1;
  let lastFrame = performance.now();
  let lastUiUpdate = 0;
  let toastClearTimer = 0;
  let audioContext = null;
  let soundsEnabled = false;
  let fpsFrames = 0;
  let fpsTimer = 0;
  let modalCloseCallback = null;
  let worldMapRootHandlers = null;

  const state = {
    started: false,
    dead: false,
    paused: false,
    player: {
      x: startPoint.x, y: startPoint.y, hp: 100, maxHp: 100, mana: 60, maxMana: 60,
      xp: 12, level: 1, gold: 25, shards: 0, watchCores: 0, potions: 1,
      inventory: [
        { id: 'apprentice-focus', quantity: 1 }, { id: 'ranger-robe', quantity: 1 },
        { id: 'trail-boots', quantity: 1 }, { id: 'ember-fox', quantity: 1 },
        { id: 'wind-wings', quantity: 1 }, { id: 'health-potion', quantity: 1 },
      ],
      equipped: { weapon: 'apprentice-focus', armor: 'ranger-robe', boots: 'trail-boots', cloak: null, ring: null, pet: 'ember-fox', wings: 'wind-wings' },
      moving: false, direction: 1, attackPoseUntil: 0, attackPoseStartedAt: 0,
    },
    camera: { x: startPoint.x, y: startPoint.y },
    currentDestinationId: initialDestination.id,
    worldLocation: { xKm: initialDestination.xKm, yKm: initialDestination.yKm },
    localOrigin: { x: startPoint.x, y: startPoint.y, xKm: initialDestination.xKm, yKm: initialDestination.yKm },
    activeSpotId: null,
    selectedWorldDestinationId: 'moonwood-hunt',
    worldWaypoint: null,
    mobs: [],
    itemDrops: [],
    itemDropsByDestination: {},
    spellVfx: [],
    levelUpEffect: null,
    pet: { x: startPoint.x - .7, y: startPoint.y + .7, nextAttack: 0 },
    elaraTalk: { startedAt: 0, until: 0 },
    questStage: 0, // First quest: 0 available, 1 hunt, 2 collect, 3 return, 4 complete.
    wispQuestStage: 0, // Follow-up: 0 available at the watch, 1 hunt, 2 collect, 3 attune, 4 complete.
    hound: { x: houndSpawn.x, y: houndSpawn.y, hp: 72, maxHp: 72, alive: false, aggro: false, nextAttack: 0, attackPoseUntil: 0, attackPoseStartedAt: 0 },
    wisp: { x: wispSpawn.x, y: wispSpawn.y, hp: 84, maxHp: 84, alive: false, aggro: false, nextAttack: 0, attackPoseUntil: 0, attackPoseStartedAt: 0 },
    drop: null,
    projectiles: [],
    target: null,
    moveTarget: null,
    waypoint: null,
    cooldowns: { basic: 0, bolt: 0, heal: 0, potion: 0 },
    floating: [],
    journal: [
      { icon: '✦', title: 'Aetherfall desperta', body: 'Você chega à praça ao amanhecer.', time: 'AGORA' },
      { icon: '⌖', title: 'Uma voz na praça', body: 'Elara observa a estrada do norte.', time: '—', muted: true },
    ],
    messages: [],
    animTime: 0,
  };

  const trees = regionMap.woodlands.map(({ x, y, scale }) => [x, y, scale]);
  const rocks = regionMap.rocks.map(({ x, y, scale }) => [x, y, scale]);
  const grassTufts = [];
  const settlementBuildings = [
    [2.7, 2.9, .78], [5.0, 2.65, .91], [7.8, 3.15, .76], [9.9, 4.8, .82],
    [2.2, 5.7, .68], [9.8, 7.3, .86], [3.1, 8.0, .84], [6.4, 8.8, .72],
    [11.0, 6.2, .72], [11.8, 9.1, .74], [1.9, 10.2, .63], [7.7, 10.5, .8],
  ];
  const cityFolk = [[4.0, 5.1, '#9c6d57'], [8.5, 5.6, '#617f77'], [6.8, 7.8, '#7d6b9a']];
  for (let i = 0; i < 54; i += 1) {
    const x = 1 + ((i * 37 + 13) % 169) / 10;
    const y = 1 + ((i * 53 + 27) % 169) / 10;
    grassTufts.push({ x, y, seed: i * 17.13, size: .45 + (i % 5) * .08 });
  }

  function loadImage(src) {
    const image = new Image();
    image.src = src;
    return image;
  }

  function clamp(value, min, max) { return Math.max(min, Math.min(max, value)); }
  function distance(a, b) { return Math.hypot(a.x - b.x, a.y - b.y); }

  function getEquippedStat(stat) {
    return Object.values(state.player.equipped).reduce((sum, itemId) => sum + (itemCatalog[itemId]?.stats?.[stat] || 0), 0);
  }

  function recalculatePlayerStats(restore = false) {
    const player = state.player;
    const previousMaxHp = player.maxHp || 100;
    const previousMaxMana = player.maxMana || 60;
    const previousHp = player.hp;
    const previousMana = player.mana;
    player.maxHp = 100 + Math.max(0, player.level - 1) * 10 + getEquippedStat('hp');
    player.maxMana = 60 + Math.max(0, player.level - 1) * 5 + getEquippedStat('mana');
    player.hp = restore ? player.maxHp : clamp(previousHp + player.maxHp - previousMaxHp, 0, player.maxHp);
    player.mana = restore ? player.maxMana : clamp(previousMana + player.maxMana - previousMaxMana, 0, player.maxMana);
  }

  function addInventoryItem(itemId, quantity = 1) {
    if (!itemCatalog[itemId] || quantity < 1) return false;
    const stack = state.player.inventory.find((item) => item.id === itemId);
    if (stack) stack.quantity += quantity;
    else state.player.inventory.push({ id: itemId, quantity });
    if (itemId === 'health-potion') state.player.potions += quantity;
    return true;
  }

  function removeInventoryItem(itemId, quantity = 1) {
    const stack = state.player.inventory.find((item) => item.id === itemId);
    if (!stack || stack.quantity < quantity) return false;
    stack.quantity -= quantity;
    if (!stack.quantity) state.player.inventory = state.player.inventory.filter((item) => item.id !== itemId);
    if (itemId === 'health-potion') state.player.potions = Math.max(0, state.player.potions - quantity);
    return true;
  }

  function inventorySlotCount() {
    return state.player.inventory.reduce((total, item) => total + item.quantity, 0);
  }

  function updateWorldLocationFromPlayer() {
    const origin = state.localOrigin;
    state.worldLocation.xKm = clamp(origin.xKm + (state.player.x - origin.x) * .55, overworldMap.boundsKm.minX, overworldMap.boundsKm.maxX);
    state.worldLocation.yKm = clamp(origin.yKm + (state.player.y - origin.y) * .55, overworldMap.boundsKm.minY, overworldMap.boundsKm.maxY);
  }

  function getEnemy(targetId = state.target) {
    if (targetId === 'hound') return state.hound;
    if (targetId === 'wisp') return state.wisp;
    return state.mobs.find((mob) => mob.id === targetId) || null;
  }

  function currentCombatTarget() {
    if (state.target && getEnemy(state.target)?.alive) return state.target;
    if (state.currentDestinationId === 'aetherfall-city' && state.wispQuestStage === 1 && state.wisp.alive) return 'wisp';
    if (state.currentDestinationId === 'aetherfall-city' && state.questStage === 1 && state.hound.alive) return 'hound';
    const closestMob = state.mobs.filter((mob) => mob.alive).sort((a, b) => distance(state.player, a) - distance(state.player, b))[0];
    return closestMob?.id || null;
  }

  recalculatePlayerStats(true);

  function drawAtlasFrame(image, sheet, animationName, time, startedAt, x, y, drawWidth, drawHeight, flip = false) {
    const animation = sheet.animations[animationName] || sheet.animations.idle;
    const elapsed = animation.loop ? time : Math.max(0, time - startedAt);
    const step = Math.floor(elapsed * animation.fps);
    const frame = animation.loop ? step % animation.frames : Math.min(animation.frames - 1, step);
    const sourceX = frame * sheet.frameWidth;
    const sourceY = animation.row * sheet.frameHeight;
    ctx.save();
    if (flip) {
      ctx.translate(x + drawWidth / 2, 0);
      ctx.scale(-1, 1);
      ctx.translate(-x - drawWidth / 2, 0);
    }
    ctx.drawImage(image, sourceX, sourceY, sheet.frameWidth, sheet.frameHeight, x, y, drawWidth, drawHeight);
    ctx.restore();
  }

  function project(x, y, z = 0) {
    const dx = x - state.camera.x;
    const dy = y - state.camera.y;
    return {
      x: width / 2 + (dx - dy) * HALF_W,
      y: height * .5 + 35 + (dx + dy) * HALF_H - z,
    };
  }

  function inverseProject(screenX, screenY) {
    const dx = (screenX - width / 2) / HALF_W;
    const dy = (screenY - (height * .5 + 35)) / HALF_H;
    return {
      x: state.camera.x + (dx + dy) * .5,
      y: state.camera.y + (dy - dx) * .5,
    };
  }

  function resizeCanvas() {
    const rect = canvas.getBoundingClientRect();
    if (!rect.width || !rect.height) return;
    dpr = Math.min(window.devicePixelRatio || 1, 2);
    width = rect.width;
    height = rect.height;
    canvas.width = Math.round(width * dpr);
    canvas.height = Math.round(height * dpr);
    ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
    draw();
  }

  function addToast(text, tone = 'gold') {
    const element = $('#scene-toast');
    if (!element) return;
    element.textContent = text;
    element.className = `scene-toast show ${tone}`;
    window.clearTimeout(toastClearTimer);
    toastClearTimer = window.setTimeout(() => element.classList.remove('show'), 2300);
  }

  function addJournal(title, body, icon = '✦') {
    state.journal.unshift({ icon, title, body, time: 'AGORA' });
    state.journal = state.journal.slice(0, 4);
    updateJournal();
  }

  function updateJournal() {
    $('#journal-list').innerHTML = state.journal.map((item) => `
      <div class="journal-entry ${item.muted ? 'muted-entry' : ''}">
        <span class="journal-symbol">${item.icon}</span>
        <div><strong>${escapeHtml(item.title)}</strong><small>${escapeHtml(item.body)}</small></div>
        <time>${escapeHtml(item.time)}</time>
      </div>`).join('');
  }

  function escapeHtml(value) {
    return String(value).replace(/[&<>"']/g, (character) => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[character]));
  }

  function xpToNextLevel() { return 100; }

  function grantXp(amount) {
    const player = state.player;
    player.xp += amount;
    while (player.xp >= xpToNextLevel()) {
      player.xp -= xpToNextLevel();
      player.level += 1;
      recalculatePlayerStats(true);
      state.levelUpEffect = { born: performance.now() / 1000, duration: 2.5, level: player.level };
      const banner = $('#level-up-banner');
      if (banner) {
        banner.textContent = `NÍVEL ${player.level} ALCANÇADO`;
        banner.classList.remove('show');
        void banner.offsetWidth;
        banner.classList.add('show');
      }
      addJournal('Novo nível alcançado', `Aelwyn agora é nível ${player.level}.`, '✧');
      addToast(`NÍVEL ${player.level} ALCANÇADO`, 'gold');
      playTone(740, .16, 'sine');
    }
  }

  function updateQuestUI() {
    const firstQuest = state.questStage < 4;
    const stage = firstQuest ? state.questStage : state.wispQuestStage;
    const objectives = firstQuest
      ? [
        'Fale com a Vigia Elara',
        'Derrote o Cão Selvagem',
        'Recolha o fragmento de éter',
        'Volte à Vigia Elara',
      ]
      : [
        'Investigue a Oca da Vigília',
        'Derrote o Wisp de Éter',
        'Recolha o núcleo de vigília',
        'Ative o selo antigo',
      ];
    $('#objective-list').innerHTML = objectives.map((label, index) => {
      const complete = index < stage;
      const active = index === stage && stage < 4;
      return `<li class="objective ${complete ? 'done' : ''} ${active ? 'active' : ''}"><span class="objective-ring"></span><span>${label}</span></li>`;
    }).join('');

    $('#quest-step').textContent = stage >= 4 ? 'CONCLUÍDA' : `0${Math.min(stage + 1, 4)} / 04`;
    $('#quest-title').textContent = firstQuest ? 'Uma promessa na fronteira' : 'O sussurro da Vigília';
    if (firstQuest) {
      $('#quest-description').textContent = stage === 0
        ? 'A Vigia Elara procura alguém capaz de afastar a ameaça que ronda a estrada do norte.'
        : stage === 1
          ? 'Siga a estrada ao norte. Um Cão Selvagem foi visto rondando a mata.'
          : stage === 2
            ? 'A criatura caiu. Recolha o fragmento de éter que ela deixou para trás.'
            : 'O fragmento está seguro. Volte à praça e conte a Elara o que aconteceu.';
      $('#quest-reward-copy').innerHTML = '35 ouro <b>·</b> Anel da Lua';
    } else {
      $('#quest-description').textContent = stage === 0
        ? 'Uma pulsação azul atravessa as runas da Oca. Investigue o selo que mantém a passagem adormecida.'
        : stage === 1
          ? 'Um Wisp de Éter protege a torre em ruínas. Interrompa seu pulso antes que a energia se espalhe.'
          : stage === 2
            ? 'O Wisp se desfez. Recolha o núcleo que flutua sobre a trilha.'
            : stage === 3
              ? 'O núcleo reage às runas da Oca. Leve-o ao portão e ative o selo antigo.'
              : 'O selo voltou a brilhar. A trilha além da Oca está desperta para a próxima etapa.';
      $('#quest-reward-copy').innerHTML = '40 ouro <b>·</b> Manto da Vigília';
    }

    const action = $('#quest-action');
    action.innerHTML = stage >= 4 ? 'MISSÃO CONCLUÍDA <span>✦</span>' : 'ACOMPANHAR MISSÃO <span>↗</span>';
    action.classList.toggle('completed', stage >= 4);
    $('#milestone-title').textContent = firstQuest ? 'Oca da Vigília' : stage >= 4 ? 'Trilha do Norte' : 'Selo da Oca';
    $('#milestone-copy').textContent = firstQuest
      ? 'Uma antiga torre guarda o caminho além da mata.'
      : stage >= 4
        ? 'Uma nova trilha se abriu além da fronteira.'
        : 'O núcleo de éter pode despertar as runas antigas.';
  }

  function updateTargetUI() {
    const panel = $('#target-panel');
    const enemy = getEnemy(state.target);
    const mobTarget = state.mobs.find((mob) => mob.id === state.target);
    const questRelevant = state.target === 'hound'
      ? state.currentDestinationId === 'aetherfall-city' && state.questStage > 0 && state.questStage < 4
      : state.target === 'wisp' && state.currentDestinationId === 'aetherfall-city' && state.questStage >= 4;
    const visible = Boolean(enemy && (mobTarget || questRelevant));
    panel.classList.toggle('hidden', !visible);
    panel.classList.toggle('wisp-target', visible && (state.target === 'wisp' || mobTarget?.kind === 'wisp'));
    if (!visible) {
      $('#combat-status-text').textContent = state.started ? 'SEM ALVO' : 'AGUARDANDO';
      $('.combat-status').classList.remove('has-target', 'wisp-target');
      return;
    }
    const wispTarget = state.target === 'wisp' || mobTarget?.kind === 'wisp';
    const range = distance(state.player, enemy);
    $('#target-name').textContent = mobTarget ? mobTarget.name : wispTarget ? 'Wisp de Éter' : 'Cão Selvagem';
    $('#target-level').textContent = `NV. ${mobTarget?.level || (wispTarget ? 4 : 2)}`;
    $('#target-kind').textContent = mobTarget ? mobTarget.kind.toUpperCase() : wispTarget ? 'ESPÍRITO' : 'CRIATURA';
    $('#target-hostility').textContent = 'HOSTIL';
    $('.combat-status').classList.toggle('wisp-target', wispTarget);
    const health = Math.max(0, enemy.hp);
    $('#enemy-hp-fill').style.width = `${(health / enemy.maxHp) * 100}%`;
    $('#enemy-hp-text').textContent = `${Math.ceil(health)} / ${enemy.maxHp}`;
    $('#target-hint').textContent = enemy.alive
      ? range > (wispTarget ? 7.5 : 2.15) ? 'Aproxime-se e use 1 ou 2 para atacar.' : 'Ao alcance — use 1 ou 2 para atacar.'
      : mobTarget ? `Renasce em ${Math.max(0, enemy.respawnAt - state.animTime).toFixed(1)}s.` : 'A criatura foi derrotada.';
    $('#combat-status-text').textContent = enemy.alive ? mobTarget ? `FARM · ${mobTarget.name.toUpperCase()}` : wispTarget ? 'WISP DE ÉTER' : 'CÃO SELVAGEM' : mobTarget ? 'MOB DERROTADO · RESPAWN 3s' : 'ALVO DERROTADO';
    $('.combat-status').classList.add('has-target');
  }

  function updateInteractionHint() {
    const hint = $('#interaction-hint');
    const label = $('#interaction-text');
    let text = '';
    if (!state.started || state.dead) {
      hint.classList.remove('visible');
      return;
    }
    if (state.currentDestinationId === 'aetherfall-city' && distance(state.player, questNpc) < 1.55) {
      text = state.questStage === 0
        ? 'Conversar com a Vigia Elara'
        : state.questStage === 3
          ? 'Entregar a missão à Elara'
          : state.questStage >= 4
            ? 'Falar com a Vigia Elara'
            : 'Falar com a Vigia Elara';
    } else if (state.drop && state.currentDestinationId === 'aetherfall-city' && distance(state.player, state.drop) < 1.55) {
      text = state.drop.kind === 'watch-core' ? 'Recolher o núcleo de vigília' : 'Recolher o fragmento de éter';
    } else if (state.itemDrops.some((drop) => distance(state.player, drop) < 1.55)) {
      const drop = state.itemDrops.find((item) => distance(state.player, item) < 1.55);
      text = `Recolher ${itemCatalog[drop.itemId]?.name || 'item'}`;
    } else if (state.currentDestinationId === 'aetherfall-city' && distance(state.player, hollowedWatch) < 1.45) {
      text = state.questStage < 4
        ? 'A Oca da Vigília está selada'
        : state.wispQuestStage === 0
          ? 'Investigar o selo da Vigília'
          : state.wispQuestStage === 3
            ? 'Ativar o selo com o núcleo'
            : 'Examinar a Oca da Vigília';
    }
    label.textContent = text;
    hint.classList.toggle('visible', Boolean(text));
  }

  function updateUi(time) {
    if (time - lastUiUpdate < 90) return;
    lastUiUpdate = time;
    const player = state.player;
    $('#hp-text').innerHTML = `${Math.ceil(player.hp)} <small>/ ${player.maxHp}</small>`;
    $('#mana-text').innerHTML = `${Math.floor(player.mana)} <small>/ ${player.maxMana}</small>`;
    $('#hp-fill').style.width = `${clamp(player.hp / player.maxHp, 0, 1) * 100}%`;
    $('#mana-fill').style.width = `${clamp(player.mana / player.maxMana, 0, 1) * 100}%`;
    $('#hp-fill').classList.toggle('low', player.hp < 32);
    $('#xp-fill').style.width = `${(player.xp / xpToNextLevel()) * 100}%`;
    $('#xp-text').textContent = `${Math.floor((player.xp / xpToNextLevel()) * 100)}%`;
    $('#gold-count').textContent = player.gold;
    $('#shard-count').textContent = player.shards;
    $('#potion-button small b').textContent = player.potions;
    $('#inventory-count').textContent = `${inventorySlotCount()} / 24`;
    $('#level-label').textContent = player.level;
    $('#level-small').textContent = player.level;
    $('#coordinate-label').textContent = `${state.worldLocation.xKm.toFixed(1)} km · ${state.worldLocation.yKm.toFixed(1)} km`;
    const currentDestination = destinationsById.get(state.currentDestinationId) || initialDestination;
    const inAetherfall = state.currentDestinationId === 'aetherfall-city';
    const nearWatch = inAetherfall && distance(player, hollowedWatch) < 3.5;
    const frontier = currentDestination.kind !== 'city';
    const activeMobCount = state.mobs.filter((mob) => mob.alive).length;
    $('#zone-title').textContent = inAetherfall
      ? nearWatch ? 'Oca da Vigília' : player.x + player.y > 17 ? 'Clareira da Fronteira' : player.x + player.y > 13 ? 'Estrada da Fronteira' : 'Praça de Aetherfall'
      : currentDestination.name;
    const unstableSeal = nearWatch && state.questStage >= 4 && state.wispQuestStage < 4;
    $('#zone-state').textContent = unstableSeal ? 'ÉTER INSTÁVEL' : currentDestination.safe ? 'CIDADE SEGURA' : `${activeMobCount}/10 MOBs · FARM`;
    $('.safe-zone').style.color = unstableSeal ? '#8bc8d1' : frontier ? '#dcab83' : '#8fc6af';
    $('.safe-zone i').style.background = unstableSeal ? '#72c8d6' : frontier ? '#d79a74' : '#7ac3a5';
    $('#distance-label').textContent = nearWatch ? 'OCA DA VIGÍLIA' : currentDestination.name.toUpperCase();
    const spotStatus = $('#spot-status');
    if (spotStatus) {
      spotStatus.classList.toggle('is-visible', Boolean(state.activeSpotId));
      spotStatus.innerHTML = state.activeSpotId ? `<span>${escapeHtml(currentDestination.name)}</span><b>${activeMobCount}/10 MOBs · RESPAWN 3s</b>` : '';
    }
    const mapPlayer = $('#minimap-player');
    mapPlayer.style.left = `${clamp(15 + (player.x / MAX_WORLD) * 72, 10, 88)}%`;
    mapPlayer.style.top = `${clamp(10 + (player.y / MAX_WORLD) * 76, 8, 88)}%`;
    const houndPin = $('#minimap-hound');
    houndPin.style.opacity = !inAetherfall ? '.08' : state.questStage === 1 ? '1' : state.questStage === 2 || state.questStage === 3 ? '.62' : '.18';
    houndPin.style.left = `${15 + (state.hound.x / MAX_WORLD) * 72}%`;
    houndPin.style.top = `${10 + (state.hound.y / MAX_WORLD) * 76}%`;
    const wispPin = $('#minimap-wisp');
    wispPin.style.opacity = !inAetherfall ? '.08' : state.questStage < 4 ? '.12' : state.wisp.alive ? '1' : state.wispQuestStage >= 2 ? '.62' : '.4';
    wispPin.style.left = `${15 + (state.wisp.x / MAX_WORLD) * 72}%`;
    wispPin.style.top = `${10 + (state.wisp.y / MAX_WORLD) * 76}%`;
    const watchPin = $('#minimap-watch');
    watchPin.style.left = `${15 + (hollowedWatch.x / MAX_WORLD) * 72}%`;
    watchPin.style.top = `${10 + (hollowedWatch.y / MAX_WORLD) * 76}%`;
    updateMiniMapRoute();
    updateWorldMapDialog();
    updateQuestUI();
    updateTargetUI();
    updateInteractionHint();
    updateHotbar(time / 1000);
  }

  function updateHotbar(now) {
    const keysBySkill = { basic: 'basic-cooldown', bolt: 'bolt-cooldown', heal: 'heal-cooldown' };
    Object.entries(keysBySkill).forEach(([skill, id]) => {
      const remaining = Math.max(0, state.cooldowns[skill] - now);
      const duration = skill === 'basic' ? .55 : skill === 'bolt' ? 1.4 : 6;
      const overlay = document.getElementById(id);
      if (overlay) overlay.style.transform = `scaleY(${remaining ? Math.min(1, remaining / duration) : 0})`;
    });
  }

  function interact() {
    if (!state.started || state.dead || state.paused) return;
    const player = state.player;
    if (state.currentDestinationId === 'aetherfall-city' && distance(player, questNpc) < 1.55) {
      state.elaraTalk.startedAt = performance.now() / 1000;
      state.elaraTalk.until = state.elaraTalk.startedAt + .9;
      if (state.questStage === 0) {
        state.questStage = 1;
        state.hound = { x: houndSpawn.x, y: houndSpawn.y, hp: 72, maxHp: 72, alive: true, aggro: false, nextAttack: 0, attackPoseUntil: 0, attackPoseStartedAt: 0 };
        state.target = 'hound';
        addJournal('Missão aceita: Uma promessa na fronteira', 'Elara marcou a trilha até a mata do norte.', '⌖');
        addToast('MISSÃO ACEITA · SIGA A ESTRADA AO NORTE');
        $('#world-tip-text').textContent = 'Siga a trilha de pedra. A criatura ronda a mata ao norte.';
        playTone(560, .11, 'triangle');
      } else if (state.questStage === 3) {
        state.questStage = 4;
        state.target = null;
        player.gold += 35;
        addInventoryItem('moonstone-ring');
        addJournal('Missão concluída', 'Elara entrega ouro e o Anel da Lua como recompensa.', '✦');
        addToast('MISSÃO CONCLUÍDA · +35 OURO · ANEL DA LUA', 'gold');
        $('#world-tip-text').textContent = 'A estrada está livre. Siga para a Oca da Vigília e investigue o selo azul.';
        playTone(740, .18, 'sine');
      } else if (state.questStage === 1) {
        addToast('ELARA: “A trilha segue para o norte. Cuidado com as presas.”');
      } else if (state.questStage === 2) {
        addToast('Elara espera o fragmento que a criatura deixou cair.');
      } else if (state.questStage >= 4 && state.wispQuestStage === 0) {
        addToast('ELARA: “A luz azul vem da Oca. As runas estão pedindo ajuda.”');
      } else if (state.wispQuestStage === 3) {
        addToast('ELARA: “O núcleo pertence à Oca. Devolva-o às runas do portão.”');
      } else {
        addToast('ELARA: “A fronteira está mais segura graças a você.”');
      }
      return;
    }
    if (state.drop && state.currentDestinationId === 'aetherfall-city' && distance(player, state.drop) < 1.55) {
      if (state.drop.kind === 'watch-core') {
        player.watchCores += 1;
        addInventoryItem('watch-core');
        state.drop = null;
        state.wispQuestStage = 3;
        addJournal('Núcleo de vigília recolhido', 'O núcleo se alinha às runas da torre.', '◆');
        addToast('NÚCLEO DE VIGÍLIA RECOLHIDO · +1');
        $('#world-tip-text').textContent = 'Leve o núcleo até a Oca e pressione E diante do portão.';
      } else {
        player.shards += 1;
        addInventoryItem('ether-shard');
        state.drop = null;
        state.questStage = Math.max(state.questStage, 3);
        addJournal('Fragmento de éter recolhido', 'A pedra vibra com uma luz azulada.', '◆');
        addToast('FRAGMENTO RECOLHIDO · +1');
        $('#world-tip-text').textContent = 'Volte à praça e entregue o fragmento à Vigia Elara.';
      }
      playTone(640, .13, 'sine');
      return;
    }
    const itemDropIndex = state.itemDrops.findIndex((drop) => distance(player, drop) < 1.55);
    if (itemDropIndex >= 0) {
      const [drop] = state.itemDrops.splice(itemDropIndex, 1);
      const item = itemCatalog[drop.itemId];
      addInventoryItem(drop.itemId, drop.quantity);
      addJournal(`${item.name} recolhido`, 'Um item encontrado em uma área de caça foi guardado no inventário.', item.icon);
      addToast(`${item.name.toUpperCase()} · +${drop.quantity} NO INVENTÁRIO`, 'gold');
      playTone(640, .13, 'sine');
      return;
    }
    if (state.currentDestinationId === 'aetherfall-city' && distance(player, hollowedWatch) < 1.45) {
      if (state.questStage < 4) {
        addToast('A entrada está selada. Ajude Elara antes de seguir adiante.', 'red');
      } else if (state.wispQuestStage === 0) {
        state.wispQuestStage = 1;
        state.wisp = { x: wispSpawn.x, y: wispSpawn.y, hp: 84, maxHp: 84, alive: true, aggro: false, nextAttack: performance.now() / 1000 + 1.1, attackPoseUntil: 0, attackPoseStartedAt: 0 };
        state.target = 'wisp';
        addJournal('Missão aceita: O sussurro da Vigília', 'As runas despertaram um Wisp de Éter nas ruínas.', '✧');
        addToast('SELO DESPERTADO · UM WISP SURGE NA MATA', 'gold');
        $('#world-tip-text').textContent = 'O Wisp protege a energia da torre. Use 1 e 2 para combatê-lo.';
        playTone(620, .2, 'sine');
      } else if (state.wispQuestStage === 3) {
        state.wispQuestStage = 4;
        state.target = null;
        player.watchCores = Math.max(0, player.watchCores - 1);
        removeInventoryItem('watch-core');
        player.gold += 40;
        addInventoryItem('watch-cloak');
        addJournal('Oca da Vigília ativada', 'O selo foi restaurado; uma peça do Manto da Vigília foi entregue.', '✦');
        addToast('SELO RESTAURADO · +40 OURO · MANTO DA VIGÍLIA', 'gold');
        $('#world-tip-text').textContent = 'A passagem despertou. Uma nova trilha aguarda além da Oca.';
        playTone(790, .22, 'sine');
      } else if (state.wispQuestStage === 1) {
        addToast('As runas tremem. O Wisp ainda protege o núcleo.');
      } else if (state.wispQuestStage === 2) {
        addToast('O selo continua apagado. Recolha o núcleo do Wisp.');
      } else {
        addToast('A Oca da Vigília pulsa com um brilho sereno.');
      }
      return;
    }
    addToast('Não há nada para interagir por aqui.');
  }

  function useBasicAttack() {
    const now = performance.now() / 1000;
    if (!state.started || state.dead || state.paused) return;
    const targetId = currentCombatTarget();
    const enemy = getEnemy(targetId);
    if (!enemy?.alive) {
      addToast(state.questStage === 0 ? 'Fale com Elara para iniciar a caçada.' : state.questStage >= 4 && state.wispQuestStage === 0 ? 'Investigue a Oca da Vigília.' : 'Não há inimigo ao alcance.');
      return;
    }
    state.target = targetId;
    const range = distance(state.player, enemy);
    if (range > 2.15) {
      state.moveTarget = { x: enemy.x - Math.sign(enemy.x - state.player.x) * 1.1, y: enemy.y - Math.sign(enemy.y - state.player.y) * 1.1 };
      addToast('APROXIMANDO-SE DO ALVO');
      return;
    }
    if (state.cooldowns.basic > now) return;
    state.cooldowns.basic = now + .56;
    state.player.attackPoseUntil = now + .35;
    state.player.attackPoseStartedAt = now;
    state.player.direction = enemy.x >= state.player.x ? 1 : -1;
    const damage = 14 + Math.floor(Math.random() * 7) + state.player.level * 2 + getEquippedStat('attack');
    state.spellVfx.push({ type: 'strike', start: { x: state.player.x, y: state.player.y }, target: { x: enemy.x, y: enemy.y }, born: now, duration: .34 });
    applyEnemyDamage(targetId, damage, 'Raio arcano');
    playTone(390, .075, 'triangle');
  }

  function useArcaneBolt() {
    const now = performance.now() / 1000;
    if (!state.started || state.dead || state.paused) return;
    if (state.cooldowns.bolt > now) return;
    if (state.player.mana < 15) { addToast('Éter insuficiente.'); return; }
    const targetId = currentCombatTarget();
    const enemy = getEnemy(targetId);
    if (!enemy?.alive) { addToast('Nenhum alvo para a Explosão de Éter.'); return; }
    state.target = targetId;
    if (distance(state.player, enemy) > 7.5) { addToast('O alvo está fora do alcance mágico.'); return; }
    state.player.mana -= 15;
    state.cooldowns.bolt = now + 1.4;
    state.player.attackPoseUntil = now + .4;
    state.player.attackPoseStartedAt = now;
    state.player.direction = enemy.x >= state.player.x ? 1 : -1;
    const damage = 27 + Math.floor(Math.random() * 9) + state.player.level * 3 + getEquippedStat('attack');
    state.spellVfx.push({ type: 'bolt', start: { x: state.player.x, y: state.player.y }, target: { x: enemy.x, y: enemy.y }, born: now, duration: .38 });
    applyEnemyDamage(targetId, damage, 'Explosão de éter');
    playTone(690, .13, 'sine');
  }

  function useHeal() {
    const now = performance.now() / 1000;
    if (!state.started || state.dead || state.paused) return;
    if (state.cooldowns.heal > now) return;
    if (state.player.mana < 20) { addToast('Éter insuficiente.'); return; }
    if (state.player.hp >= state.player.maxHp - 1) { addToast('Sua vida já está cheia.'); return; }
    state.player.mana -= 20;
    state.player.hp = Math.min(state.player.maxHp, state.player.hp + 36);
    state.cooldowns.heal = now + 6;
    state.player.attackPoseUntil = now + .72;
    state.player.attackPoseStartedAt = now;
    state.spellVfx.push({ type: 'heal', start: { x: state.player.x, y: state.player.y }, target: { x: state.player.x, y: state.player.y }, born: now, duration: 1.05 });
    const p = project(state.player.x, state.player.y, 62);
    addFloatingText(p.x, p.y, '+36', '#8ed8a7', 14);
    addToast('SOPRO VITAL · +36 VIDA');
    playTone(540, .18, 'sine');
  }

  function usePotion() {
    if (!state.started || state.dead || state.paused) return;
    if (!state.player.potions) { addToast('Você não tem poções de vida.'); return; }
    if (state.player.hp >= state.player.maxHp - 1) { addToast('Sua vida já está cheia.'); return; }
    removeInventoryItem('health-potion');
    state.player.hp = Math.min(state.player.maxHp, state.player.hp + 48);
    addFloatingText(width / 2, height / 2, '+48', '#e59a8e', 14);
    addToast('POÇÃO DE VIDA · +48 VIDA');
    playTone(470, .1, 'triangle');
  }

  function applyEnemyDamage(targetId, amount, source) {
    const enemy = getEnemy(targetId);
    if (!enemy?.alive) return;
    enemy.hp = Math.max(0, enemy.hp - amount);
    const p = project(enemy.x, enemy.y, 70 + Math.random() * 12);
    addFloatingText(p.x, p.y, `-${amount}`, targetId === 'wisp' ? '#a9eff0' : '#ffd7b1', 13);
    enemy.aggro = true;
    addToast(source.toUpperCase());
    if (enemy.hp > 0) return;

    enemy.alive = false;
    enemy.aggro = false;
    if (state.mobs.some((mob) => mob.id === targetId)) {
      enemy.respawnAt = state.animTime + enemy.respawnSeconds;
      enemy.nextAttack = 0;
      const xp = 18 + enemy.level * 3;
      const gold = 2 + Math.floor(Math.random() * 5);
      state.player.gold += gold;
      grantXp(xp);
      const loot = enemy.loot || [];
      if (loot.length && Math.random() < .78) {
        const itemId = loot[Math.floor(Math.random() * loot.length)];
        state.itemDrops.push({ id: `drop-${targetId}-${Math.floor(state.animTime * 1000)}`, itemId, quantity: 1, x: enemy.x, y: enemy.y, born: state.animTime });
        addToast(`MOB DERROTADO · +${xp} XP · +${gold} OURO · ITEM NO CHÃO`, 'gold');
      } else {
        addToast(`MOB DERROTADO · +${xp} XP · +${gold} OURO`, 'gold');
      }
      addJournal(`${enemy.name} derrotado`, `+${xp} XP · retorna em 3 segundos.`, '⚔');
      return;
    }
    if (targetId === 'hound') {
      state.questStage = Math.max(state.questStage, 2);
      state.drop = { x: enemy.x, y: enemy.y, kind: 'wild-fragment' };
      state.player.gold += 5;
      addJournal('Cão Selvagem derrotado', 'A criatura deixou um fragmento de éter; missões não concedem experiência.', '⚔');
      $('#world-tip-text').textContent = 'Recolha o fragmento brilhante no chão. Para evoluir, cace nas áreas de farm.';
      addToast('CRIATURA DERROTADA · +5 OURO · SEM XP DE MISSÃO', 'gold');
    } else {
      state.wispQuestStage = Math.max(state.wispQuestStage, 2);
      state.drop = { x: enemy.x, y: enemy.y, kind: 'watch-core' };
      state.player.gold += 8;
      addJournal('Wisp de Éter dispersado', 'Um núcleo de vigília caiu sobre a trilha; missões não concedem experiência.', '⚔');
      $('#world-tip-text').textContent = 'Recolha o núcleo luminoso e leve-o ao portão da Oca.';
      addToast('WISP DERROTADO · +8 OURO · SEM XP DE MISSÃO', 'gold');
    }
    playTone(260, .2, 'sawtooth');
  }

  function addFloatingText(x, y, text, color, size = 12) {
    state.floating.push({ x, y, text, color, size, born: performance.now() / 1000 });
  }

  function movePlayer(dt) {
    const player = state.player;
    let dx = 0;
    let dy = 0;
    if (keys.has('w') || keys.has('arrowup')) { dx += 0; dy -= 1; }
    if (keys.has('s') || keys.has('arrowdown')) { dx += 0; dy += 1; }
    if (keys.has('a') || keys.has('arrowleft')) { dx -= 1; dy += 0; }
    if (keys.has('d') || keys.has('arrowright')) { dx += 1; dy += 0; }
    if (dx || dy) state.moveTarget = null;
    let target = null;
    let speed = (keys.has('shift') ? 4.45 : 3.15) + getEquippedStat('speed');
    if (dx || dy) {
      const length = Math.hypot(dx, dy) || 1;
      target = { x: dx / length, y: dy / length };
    } else if (state.moveTarget) {
      const to = { x: state.moveTarget.x - player.x, y: state.moveTarget.y - player.y };
      const length = Math.hypot(to.x, to.y);
      if (length < .12) state.moveTarget = null;
      else target = { x: to.x / length, y: to.y / length };
    }
    player.moving = Boolean(target) && !state.dead;
    if (!target || state.dead) return;
    player.x = clamp(player.x + target.x * speed * dt, .45, MAX_WORLD - .45);
    player.y = clamp(player.y + target.y * speed * dt, .45, MAX_WORLD - .45);
    player.direction = target.x - target.y >= 0 ? 1 : -1;
  }

  function applyPlayerDamage(amount, source, tone = 145, toneType = 'square') {
    const player = state.player;
    player.hp = Math.max(0, player.hp - amount);
    const p = project(player.x, player.y, 55);
    addFloatingText(p.x, p.y, `-${amount}`, '#ff9992', 12);
    addToast(source, 'red');
    playTone(tone, .09, toneType);
    if (player.hp <= 0) {
      state.dead = true;
      $('#defeat-card').classList.remove('hidden');
      addJournal('Aelwyn caiu', 'Uma luz antiga a trouxe de volta à praça.', '✧');
    }
  }

  function updateHound(dt, now) {
    const hound = state.hound;
    const player = state.player;
    if (!hound.alive || state.dead || state.currentDestinationId !== 'aetherfall-city' || state.questStage === 0 || state.questStage >= 4) return;
    const dist = distance(player, hound);
    if (dist < 5.1 || hound.aggro) hound.aggro = true;
    if (!hound.aggro) return;
    if (dist > 1.45) {
      const dx = (player.x - hound.x) / (dist || 1);
      const dy = (player.y - hound.y) / (dist || 1);
      hound.x = clamp(hound.x + dx * 1.35 * dt, .45, MAX_WORLD - .45);
      hound.y = clamp(hound.y + dy * 1.35 * dt, .45, MAX_WORLD - .45);
    } else if (now > hound.nextAttack) {
      hound.nextAttack = now + 1.7;
      const damage = 6 + Math.floor(Math.random() * 5);
      hound.attackPoseStartedAt = now;
      hound.attackPoseUntil = now + .5;
      applyPlayerDamage(damage, 'O CÃO SELVAGEM ATACA');
    }
  }

  function updateWisp(dt, now) {
    const wisp = state.wisp;
    const player = state.player;
    if (!wisp.alive || state.dead || state.currentDestinationId !== 'aetherfall-city' || state.wispQuestStage !== 1) return;
    const dist = distance(player, wisp);
    if (dist < 7.5 || wisp.aggro) wisp.aggro = true;
    if (!wisp.aggro) return;

    if (dist > 5.1) {
      const dx = (player.x - wisp.x) / (dist || 1);
      const dy = (player.y - wisp.y) / (dist || 1);
      wisp.x = clamp(wisp.x + dx * .92 * dt, .45, MAX_WORLD - .45);
      wisp.y = clamp(wisp.y + dy * .92 * dt, .45, MAX_WORLD - .45);
    } else if (dist < 1.75) {
      const dx = (wisp.x - player.x) / (dist || 1);
      const dy = (wisp.y - player.y) / (dist || 1);
      wisp.x = clamp(wisp.x + dx * .62 * dt, .45, MAX_WORLD - .45);
      wisp.y = clamp(wisp.y + dy * .62 * dt, .45, MAX_WORLD - .45);
    }

    if (dist <= 7.8 && now > wisp.nextAttack) {
      wisp.nextAttack = now + 2.25;
      wisp.attackPoseStartedAt = now;
      wisp.attackPoseUntil = now + .55;
      state.projectiles.push({
        born: now,
        duration: .52,
        start: { x: wisp.x, y: wisp.y },
        target: { x: player.x, y: player.y },
        damage: 5 + Math.floor(Math.random() * 5),
      });
      playTone(515, .13, 'sine');
    }
  }

  function isCityLocation() {
    return destinationsById.get(state.currentDestinationId)?.kind === 'city';
  }

  function spawnMobSpot(destination, now = performance.now() / 1000) {
    state.activeSpotId = destination?.spotId || null;
    state.mobs = [];
    if (!destination?.spotId) return;
    const offsets = [
      [-2.35, -1.75], [-1.15, -2.05], [0.15, -2.35], [1.45, -1.8], [2.45, -.8],
      [-2.2, .45], [-1.0, .9], [.45, .75], [1.7, .8], [2.3, 1.8],
    ];
    for (let index = 0; index < (destination.mobCount || 10); index += 1) {
      const [dx, dy] = offsets[index];
      const level = destination.levelMin + (index % (destination.levelMax - destination.levelMin + 1));
      const x = clamp(destination.localX + dx, .65, MAX_WORLD - .65);
      const y = clamp(destination.localY + dy, .65, MAX_WORLD - .65);
      state.mobs.push({
        id: `${destination.spotId}-${index + 1}`, name: destination.mobName, kind: destination.mobKind,
        level, x, y, spawnX: x, spawnY: y, hp: 38 + level * 12, maxHp: 38 + level * 12,
        alive: true, aggro: false, nextAttack: now + .7 + (index % 4) * .23,
        attackPoseUntil: 0, attackPoseStartedAt: 0, respawnAt: 0, respawnSeconds: destination.respawnSeconds || 3, loot: destination.loot,
        seed: index * 7.31, color: ['#b8805d', '#83946c', '#7d71a8', '#a96659'][index % 4],
      });
    }
  }

  function updateFarmMobs(dt, now) {
    if (!state.activeSpotId || !state.mobs.length || state.dead) return;
    const aliveMobs = state.mobs.filter((mob) => mob.alive);
    const attackMobs = new Set(aliveMobs.slice().sort((a, b) => distance(state.player, a) - distance(state.player, b)).slice(0, 3));
    state.mobs.forEach((mob) => {
      if (!mob.alive) {
        if (now >= mob.respawnAt) {
          mob.alive = true;
          mob.hp = mob.maxHp;
          mob.x = mob.spawnX;
          mob.y = mob.spawnY;
          mob.aggro = false;
          mob.nextAttack = now + 1.1;
          mob.respawnAt = 0;
        }
        return;
      }
      const dist = distance(state.player, mob);
      if (dist < 5.25 || state.target === mob.id) mob.aggro = true;
      if (!mob.aggro) {
        const drift = Math.sin(now * .8 + mob.seed) * .18;
        mob.x = clamp(mob.spawnX + drift, .45, MAX_WORLD - .45);
        return;
      }
      if (dist > 1.55) {
        const dx = (state.player.x - mob.x) / (dist || 1);
        const dy = (state.player.y - mob.y) / (dist || 1);
        mob.x = clamp(mob.x + dx * (1.05 + Math.min(mob.level, 15) * .025) * dt, .45, MAX_WORLD - .45);
        mob.y = clamp(mob.y + dy * (1.05 + Math.min(mob.level, 15) * .025) * dt, .45, MAX_WORLD - .45);
      } else if (attackMobs.has(mob) && now >= mob.nextAttack) {
        mob.nextAttack = now + 2.15;
        mob.attackPoseStartedAt = now;
        mob.attackPoseUntil = now + .4;
        applyPlayerDamage(2 + Math.floor(mob.level / 3) + Math.floor(Math.random() * 3), `${mob.name.toUpperCase()} ATACA`, 170, 'square');
      }
    });
  }

  function updatePet(dt, now) {
    const petId = state.player.equipped.pet;
    if (!petId) return;
    const followX = state.player.x - (state.player.direction > 0 ? .85 : -.85);
    const followY = state.player.y + .62;
    const distanceToPlayer = distance(state.pet, { x: followX, y: followY });
    if (distanceToPlayer > .12) {
      const step = Math.min(distanceToPlayer, dt * 3.2);
      state.pet.x += (followX - state.pet.x) / distanceToPlayer * step;
      state.pet.y += (followY - state.pet.y) / distanceToPlayer * step;
    }
    const targetId = currentCombatTarget();
    const target = getEnemy(targetId);
    if (!target?.alive || now < state.pet.nextAttack || distance(state.pet, target) > 2.3) return;
    state.pet.nextAttack = now + 2.8;
    const damage = 2 + getEquippedStat('petDamage') + Math.floor(state.player.level / 2);
    const p = project(target.x, target.y, 70);
    addFloatingText(p.x, p.y, '✦', '#ffc979', 15);
    state.spellVfx.push({ type: 'pet', start: { x: state.pet.x, y: state.pet.y }, target: { x: target.x, y: target.y }, born: now, duration: .3 });
    applyEnemyDamage(targetId, damage, 'A RAPOSA DE BRASA ATACA');
  }

  function updateProjectiles(now) {
    state.projectiles = state.projectiles.filter((projectile) => {
      if (now - projectile.born < projectile.duration) return true;
      if (distance(state.player, projectile.target) < 1.15) {
        applyPlayerDamage(projectile.damage, 'O WISP DISPARA UM PULSO DE ÉTER', 260, 'sine');
      } else {
        const miss = project(projectile.target.x, projectile.target.y, 28);
        addFloatingText(miss.x, miss.y, 'ERROU', '#a8d8dd', 9);
      }
      return false;
    });
  }

  function update(dt, time) {
    const now = time / 1000;
    state.animTime = now;
    if (state.started && !state.paused && !state.dead) {
      movePlayer(dt);
      updateWorldLocationFromPlayer();
      updateHound(dt, now);
      updateWisp(dt, now);
      updateFarmMobs(dt, now);
      updatePet(dt, now);
      updateProjectiles(now);
      state.camera.x += (state.player.x - state.camera.x) * Math.min(1, dt * 5.2);
      state.camera.y += (state.player.y - state.camera.y) * Math.min(1, dt * 5.2);
      state.player.mana = Math.min(state.player.maxMana, state.player.mana + dt * 1.4);
    }
    state.floating = state.floating.filter((item) => now - item.born < 1.0);
    state.spellVfx = state.spellVfx.filter((effect) => now - effect.born < effect.duration + .52);
    updateUi(time);
  }

  function draw() {
    if (!width || !height) return;
    ctx.clearRect(0, 0, width, height);
    const sky = ctx.createLinearGradient(0, 0, 0, height);
    sky.addColorStop(0, '#26382d');
    sky.addColorStop(.6, '#344936');
    sky.addColorStop(1, '#1c2c27');
    ctx.fillStyle = sky;
    ctx.fillRect(0, 0, width, height);
    drawAtmosphere();
    drawTerrain();
    const entities = [];
    trees.forEach(([x, y, scale], index) => {
      if (!isCityLocation() || x < 2.4 || y < 2.4 || x > 15.2 || y > 15.2) entities.push({ depth: x + y, draw: () => drawTree(x, y, scale, index) });
    });
    if (!isCityLocation()) rocks.forEach(([x, y, scale], index) => entities.push({ depth: x + y, draw: () => drawRock(x, y, scale, index) }));
    if (isCityLocation()) {
      settlementBuildings.forEach(([x, y, scale], index) => entities.push({ depth: x + y, draw: () => drawHouse(x, y, scale, index) }));
      entities.push({ depth: 18.0, draw: () => drawGate(9, 9) });
      cityFolk.forEach(([x, y, color], index) => entities.push({ depth: x + y + .01, draw: () => drawCityFolk(x, y, color, index) }));
      entities.push({ depth: 7.8, draw: () => drawMarketStall(3.5, 6.4) });
      entities.push({ depth: 13.8, draw: () => drawCityFountain(6.6, 7.2) });
      entities.push({ depth: 7.5, draw: () => drawCityTower(1.4, 6.1, .76) });
      entities.push({ depth: 15.8, draw: () => drawCityTower(11.7, 4.1, .9) });
      entities.push({ depth: 6.2, draw: () => drawCitySign(3.2, 5.2, 'MERCADO') });
      entities.push({ depth: 12.8, draw: () => drawCitySign(8.9, 6.4, 'GUILDA') });
      if (state.currentDestinationId === 'aetherfall-city') entities.push({ depth: questNpc.x + questNpc.y + .02, draw: () => drawNpc(questNpc.x, questNpc.y) });
    }
    if (state.currentDestinationId === 'aetherfall-city' && (state.questStage >= 4 || state.wispQuestStage > 0)) {
      entities.push({ depth: hollowedWatch.x + hollowedWatch.y, draw: () => drawDungeonGate(hollowedWatch.x, hollowedWatch.y) });
    }
    if (state.currentDestinationId === 'aetherfall-city' && state.hound.alive) entities.push({ depth: state.hound.x + state.hound.y, draw: () => drawHound() });
    if (state.currentDestinationId === 'aetherfall-city' && state.wisp.alive) entities.push({ depth: state.wisp.x + state.wisp.y, draw: () => drawWisp() });
    state.mobs.filter((mob) => mob.alive).forEach((mob) => entities.push({ depth: mob.x + mob.y, draw: () => drawFarmMob(mob) }));
    if (state.drop && state.currentDestinationId === 'aetherfall-city') entities.push({ depth: state.drop.x + state.drop.y + .02, draw: () => drawDrop() });
    state.itemDrops.forEach((drop) => entities.push({ depth: drop.x + drop.y + .025, draw: () => drawItemDrop(drop) }));
    if (state.player.equipped.pet) entities.push({ depth: state.pet.x + state.pet.y + .01, draw: () => drawPet() });
    entities.push({ depth: state.player.x + state.player.y + .015, draw: () => drawPlayer() });
    entities.sort((a, b) => a.depth - b.depth);
    entities.forEach((item) => item.draw());
    drawProjectiles();
    drawSpellVfx();
    drawLevelUpEffect();
    drawQuestMarkers();
    drawWaypointMarker();
    drawFloatingText();
    drawAmbientParticles();
  }

  function drawAtmosphere() {
    const glow = ctx.createRadialGradient(width * .4, height * .18, 8, width * .4, height * .18, width * .72);
    glow.addColorStop(0, 'rgba(230,201,132,.13)');
    glow.addColorStop(.45, 'rgba(164,185,126,.045)');
    glow.addColorStop(1, 'rgba(9,17,17,0)');
    ctx.fillStyle = glow;
    ctx.fillRect(0, 0, width, height);
    const haze = ctx.createLinearGradient(0, 0, 0, height);
    haze.addColorStop(0, 'rgba(177,194,148,.08)');
    haze.addColorStop(.45, 'rgba(142,166,123,.015)');
    haze.addColorStop(1, 'rgba(9,15,18,.1)');
    ctx.fillStyle = haze;
    ctx.fillRect(0, 0, width, height);
  }

  function distanceToSegment(px, py, start, end) {
    const dx = end[0] - start[0];
    const dy = end[1] - start[1];
    const lengthSquared = dx * dx + dy * dy;
    const t = lengthSquared === 0 ? 0 : clamp(((px - start[0]) * dx + (py - start[1]) * dy) / lengthSquared, 0, 1);
    return Math.hypot(px - (start[0] + t * dx), py - (start[1] + t * dy));
  }

  function isRoad(x, y) {
    const px = x + .5;
    const py = y + .5;
    return regionMap.paths.some((path) => path.points.some((point, index) =>
      index < path.points.length - 1 && distanceToSegment(px, py, point, path.points[index + 1]) <= path.width * .5));
  }

  function isPlaza(x, y) {
    return isCityLocation() ? x >= 3 && x <= 9 && y >= 3 && y <= 9 : x >= 4 && x <= 7 && y >= 4 && y <= 7;
  }
  function isLake(x, y) {
    const biome = destinationsById.get(state.currentDestinationId)?.biome;
    if (biome === 'crystal-coast') return x <= 5.5 && y >= 7.5 && y <= 16;
    if (biome === 'mourning-marsh') return x <= 3 && y >= 10 && y <= 15;
    return x <= 2 && y >= 11 && y <= 15;
  }

  function drawTerrain() {
    const tiles = [];
    for (let x = 0; x < MAX_WORLD; x += 1) {
      for (let y = 0; y < MAX_WORLD; y += 1) tiles.push({ x, y, depth: x + y });
    }
    tiles.sort((a, b) => a.depth - b.depth);
    tiles.forEach(({ x, y }) => {
      const p = project(x + .5, y + .5);
      if (p.x < -HALF_W || p.x > width + HALF_W || p.y < -HALF_H || p.y > height + HALF_H) return;
      let color = tileColor(x, y);
      const tint = (Math.sin(x * 13.77 + y * 27.31) + Math.cos(y * 17.3 - x * 4.2)) * 2.1;
      color = tintColor(color, tint);
      ctx.beginPath();
      ctx.moveTo(p.x, p.y - HALF_H);
      ctx.lineTo(p.x + HALF_W, p.y);
      ctx.lineTo(p.x, p.y + HALF_H);
      ctx.lineTo(p.x - HALF_W, p.y);
      ctx.closePath();
      ctx.fillStyle = color;
      ctx.fill();
      ctx.strokeStyle = 'rgba(197,203,155,.075)';
      ctx.lineWidth = .65;
      ctx.stroke();
      if (isRoad(x, y) && !isPlaza(x, y)) {
        ctx.beginPath();
        ctx.ellipse(p.x + Math.sin(x * 4) * 7, p.y + Math.cos(y * 5) * 2, 10 + (x % 3) * 2, 2.2, -.15, 0, Math.PI * 2);
        ctx.fillStyle = 'rgba(167,143,104,.13)';
        ctx.fill();
      }
      if (isLake(x, y)) drawWaterTile(p, x, y);
    });
    grassTufts.forEach((tuft) => {
      if (Math.abs(tuft.x - state.camera.x) + Math.abs(tuft.y - state.camera.y) > 16) return;
      if (isRoad(Math.floor(tuft.x), Math.floor(tuft.y)) || isPlaza(Math.floor(tuft.x), Math.floor(tuft.y)) || isLake(Math.floor(tuft.x), Math.floor(tuft.y))) return;
      const p = project(tuft.x, tuft.y);
      if (p.x < 4 || p.x > width - 4 || p.y < 5 || p.y > height - 10) return;
      ctx.strokeStyle = `rgba(178,196,126,${.12 + (tuft.seed % 4) * .025})`;
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.moveTo(p.x, p.y);
      ctx.lineTo(p.x - 2, p.y - 4 * tuft.size);
      ctx.moveTo(p.x + 1, p.y);
      ctx.lineTo(p.x + 3, p.y - 3 * tuft.size);
      ctx.stroke();
    });
  }

  function tileColor(x, y) {
    if (isLake(x, y)) return '#31565a';
    if (isPlaza(x, y)) return (x + y) % 2 ? '#8a7957' : '#8e7d5a';
    if (isRoad(x, y)) return (x * 3 + y) % 2 ? '#79684c' : '#826f50';
    const biome = destinationsById.get(state.currentDestinationId)?.biome;
    const palettes = {
      moonwood: ['#334d37', '#3b583d', '#435e3f'],
      sunmeadow: ['#647044', '#707a47', '#7d844c'],
      'crystal-coast': ['#42605b', '#496c65', '#52756b'],
      'ember-highlands': ['#614c3c', '#6d523b', '#785a3d'],
      'mourning-marsh': ['#3c5140', '#465a42', '#4d6045'],
      'watch-ruins': ['#615c4b', '#6c6450', '#756b53'],
      glimmergrove: ['#3e5c57', '#49675e', '#526d62'],
      dragonkeep: ['#514840', '#5f4b3e', '#6b5140'],
    };
    const palette = palettes[biome];
    if (palette && !isCityLocation()) return palette[Math.abs(Math.floor(x * 3 + y * 7)) % palette.length];
    if (x + y > 20) return (x + y) % 2 ? '#3b5138' : '#40583b';
    if (x + y < 8) return (x + y) % 2 ? '#405a3a' : '#465d3c';
    return ((x * 7 + y * 11) % 5 === 0) ? '#4a613d' : ((x + y) % 2 ? '#435b3a' : '#47603c');
  }

  function tintColor(hex, amount) {
    const value = hex.replace('#', '');
    const r = clamp(parseInt(value.slice(0, 2), 16) + amount, 0, 255);
    const g = clamp(parseInt(value.slice(2, 4), 16) + amount, 0, 255);
    const b = clamp(parseInt(value.slice(4, 6), 16) + amount, 0, 255);
    return `rgb(${r|0},${g|0},${b|0})`;
  }

  function drawWaterTile(p, x, y) {
    ctx.save();
    ctx.beginPath();
    ctx.moveTo(p.x, p.y - HALF_H);
    ctx.lineTo(p.x + HALF_W, p.y);
    ctx.lineTo(p.x, p.y + HALF_H);
    ctx.lineTo(p.x - HALF_W, p.y);
    ctx.closePath();
    ctx.clip();
    const gradient = ctx.createLinearGradient(p.x - 35, p.y - 20, p.x + 35, p.y + 20);
    gradient.addColorStop(0, '#315458');
    gradient.addColorStop(.45, '#3d6b6b');
    gradient.addColorStop(1, '#26484d');
    ctx.fillStyle = gradient;
    ctx.fillRect(p.x - HALF_W, p.y - HALF_H, TILE_W, TILE_H);
    ctx.strokeStyle = 'rgba(190,222,190,.18)';
    ctx.lineWidth = 1;
    const offset = (state.animTime * 9 + x * 5 + y * 3) % 35;
    ctx.beginPath();
    ctx.moveTo(p.x - 18 + offset * .2, p.y - 3);
    ctx.quadraticCurveTo(p.x, p.y - 8, p.x + 16, p.y - 3);
    ctx.stroke();
    ctx.restore();
  }

  function drawHouse(x, y, scale, variation = 0) {
    const p = project(x, y);
    drawShadow(p.x, p.y - 2, 52 * scale, 14 * scale);
    if (images.house.complete && images.house.naturalWidth) {
      const w = 139 * scale;
      const h = 140 * scale;
      ctx.save();
      ctx.filter = ['none', 'hue-rotate(18deg)', 'hue-rotate(-16deg)', 'saturate(.78)'][variation % 4];
      ctx.drawImage(images.house, p.x - w * .46, p.y - h * .88, w, h);
      ctx.restore();
    } else {
      ctx.save();
      ctx.fillStyle = ['#ad956a', '#917d5e', '#a98163', '#858365'][variation % 4];
      drawFallbackHouse(p.x, p.y, scale);
      ctx.restore();
    }
    if (isCityLocation()) {
      ctx.fillStyle = 'rgba(238,211,135,.62)';
      ctx.beginPath(); ctx.arc(p.x + 17 * scale, p.y - 41 * scale + Math.sin(state.animTime * 2 + variation) * 1.2, 2.3 * scale, 0, Math.PI * 2); ctx.fill();
    }
  }

  function drawCitySign(x, y, label) {
    const p = project(x, y, 43);
    const width = label.length * 5.2 + 14;
    ctx.strokeStyle = '#63513b'; ctx.lineWidth = 2;
    ctx.beginPath(); ctx.moveTo(p.x, p.y + 11); ctx.lineTo(p.x, p.y + 24); ctx.stroke();
    ctx.fillStyle = 'rgba(21,31,29,.9)'; roundRect(ctx, p.x - width / 2, p.y - 1, width, 14, 3); ctx.fill();
    ctx.strokeStyle = 'rgba(215,185,121,.6)'; ctx.lineWidth = .8; ctx.stroke();
    ctx.fillStyle = '#e0cb91'; ctx.font = '800 6px system-ui'; ctx.textAlign = 'center'; ctx.fillText(label, p.x, p.y + 8);
  }

  function drawCityTower(x, y, scale) {
    const p = project(x, y);
    const half = 18 * scale;
    const bodyHeight = 59 * scale;
    drawShadow(p.x, p.y, half * 1.6, 6 * scale);
    ctx.fillStyle = '#887958'; ctx.fillRect(p.x - half, p.y - bodyHeight, half * 2, bodyHeight);
    ctx.fillStyle = '#b8a37a'; ctx.beginPath(); ctx.moveTo(p.x - half - 5 * scale, p.y - bodyHeight); ctx.lineTo(p.x, p.y - bodyHeight - 22 * scale); ctx.lineTo(p.x + half + 5 * scale, p.y - bodyHeight); ctx.closePath(); ctx.fill();
    ctx.strokeStyle = '#d7c08c'; ctx.lineWidth = 1.2 * scale; ctx.stroke();
    ctx.fillStyle = '#283d3d'; ctx.beginPath(); ctx.arc(p.x, p.y - bodyHeight + 17 * scale, 5 * scale, Math.PI, 0); ctx.lineTo(p.x + 5 * scale, p.y - bodyHeight + 29 * scale); ctx.lineTo(p.x - 5 * scale, p.y - bodyHeight + 29 * scale); ctx.closePath(); ctx.fill();
    ctx.fillStyle = '#dcb977'; ctx.fillRect(p.x + half * .4, p.y - bodyHeight - 26 * scale, 2 * scale, 25 * scale);
    ctx.fillStyle = '#a85c4f'; ctx.beginPath(); ctx.moveTo(p.x + half * .5, p.y - bodyHeight - 25 * scale); ctx.lineTo(p.x + half * .5 + 15 * scale, p.y - bodyHeight - 20 * scale); ctx.lineTo(p.x + half * .5, p.y - bodyHeight - 15 * scale); ctx.closePath(); ctx.fill();
  }

  function drawMarketStall(x, y) {
    const p = project(x, y);
    drawShadow(p.x, p.y, 25, 6);
    ctx.fillStyle = '#5f4b36'; ctx.fillRect(p.x - 22, p.y - 30, 3, 29); ctx.fillRect(p.x + 19, p.y - 30, 3, 29);
    ctx.fillStyle = '#8f704d'; ctx.fillRect(p.x - 24, p.y - 8, 48, 8);
    ctx.fillStyle = '#a64e44'; ctx.beginPath(); ctx.moveTo(p.x - 28, p.y - 31); ctx.lineTo(p.x, p.y - 43); ctx.lineTo(p.x + 28, p.y - 31); ctx.lineTo(p.x + 24, p.y - 22); ctx.lineTo(p.x - 24, p.y - 22); ctx.closePath(); ctx.fill();
    ctx.fillStyle = 'rgba(241,210,151,.85)';
    [-18, -6, 6, 18].forEach((offset) => { ctx.beginPath(); ctx.moveTo(p.x + offset, p.y - 22); ctx.lineTo(p.x + offset + 5, p.y - 22); ctx.lineTo(p.x + offset + 5, p.y - 29); ctx.lineTo(p.x + offset, p.y - 27); ctx.closePath(); ctx.fill(); });
    ctx.fillStyle = '#d7b96f'; ctx.beginPath(); ctx.arc(p.x - 10, p.y - 12, 3, 0, Math.PI * 2); ctx.arc(p.x, p.y - 14, 3.5, 0, Math.PI * 2); ctx.arc(p.x + 10, p.y - 12, 3, 0, Math.PI * 2); ctx.fill();
  }

  function drawCityFountain(x, y) {
    const p = project(x, y);
    const shimmer = Math.sin(state.animTime * 4) * 1.4;
    drawShadow(p.x, p.y, 35, 8);
    ctx.fillStyle = '#756b55'; ctx.beginPath(); ctx.ellipse(p.x, p.y - 3, 34, 12, 0, 0, Math.PI * 2); ctx.fill();
    ctx.strokeStyle = '#c3b188'; ctx.lineWidth = 3; ctx.beginPath(); ctx.ellipse(p.x, p.y - 7, 28, 10, 0, 0, Math.PI * 2); ctx.stroke();
    const water = ctx.createRadialGradient(p.x - 5, p.y - 15, 1, p.x, p.y - 12, 24);
    water.addColorStop(0, '#9bd7ca'); water.addColorStop(1, '#3c7d83');
    ctx.fillStyle = water; ctx.beginPath(); ctx.ellipse(p.x, p.y - 10, 24, 7, 0, 0, Math.PI * 2); ctx.fill();
    ctx.fillStyle = '#a9e7d4'; ctx.fillRect(p.x - 2, p.y - 27 + shimmer, 4, 14);
    ctx.beginPath(); ctx.arc(p.x - 9, p.y - 19 - shimmer, 1.8, 0, Math.PI * 2); ctx.arc(p.x + 8, p.y - 22 + shimmer, 1.5, 0, Math.PI * 2); ctx.fill();
  }

  function drawCityFolk(x, y, color, index) {
    const p = project(x, y);
    const bob = Math.sin(state.animTime * (2.3 + index * .2) + index) * 1.8;
    drawShadow(p.x, p.y, 11, 3.5);
    ctx.fillStyle = color; ctx.beginPath(); ctx.moveTo(p.x - 7, p.y - 2); ctx.lineTo(p.x - 6, p.y - 22 + bob); ctx.lineTo(p.x, p.y - 28 + bob); ctx.lineTo(p.x + 7, p.y - 21 + bob); ctx.lineTo(p.x + 8, p.y - 2); ctx.closePath(); ctx.fill();
    ctx.fillStyle = '#d8b591'; ctx.beginPath(); ctx.arc(p.x, p.y - 31 + bob, 5.3, 0, Math.PI * 2); ctx.fill();
    ctx.fillStyle = '#473f3d'; ctx.beginPath(); ctx.arc(p.x, p.y - 33 + bob, 5.6, Math.PI, Math.PI * 2); ctx.fill();
    ctx.strokeStyle = 'rgba(224,204,151,.45)'; ctx.lineWidth = 1; ctx.beginPath(); ctx.moveTo(p.x - 4, p.y - 1); ctx.lineTo(p.x - 5 + Math.sin(state.animTime * 4 + index), p.y + 3); ctx.moveTo(p.x + 4, p.y - 1); ctx.lineTo(p.x + 5 - Math.sin(state.animTime * 4 + index), p.y + 3); ctx.stroke();
  }

  function drawFallbackHouse(x, y, scale) {
    const w = 78 * scale;
    ctx.fillStyle = '#ad956a'; ctx.fillRect(x - w / 2, y - 52 * scale, w, 48 * scale);
    ctx.fillStyle = '#35444b'; ctx.beginPath(); ctx.moveTo(x - w / 2 - 7, y - 48 * scale); ctx.lineTo(x, y - 89 * scale); ctx.lineTo(x + w / 2 + 9, y - 48 * scale); ctx.closePath(); ctx.fill();
    ctx.fillStyle = '#55402f'; ctx.fillRect(x - 6 * scale, y - 31 * scale, 12 * scale, 27 * scale);
  }

  function drawTree(x, y, scale, seed) {
    const p = project(x, y);
    drawShadow(p.x, p.y - 1, 25 * scale, 8 * scale);
    const trunkH = 25 * scale;
    ctx.fillStyle = '#594331';
    ctx.beginPath();
    ctx.moveTo(p.x - 5 * scale, p.y - 4);
    ctx.lineTo(p.x - 2 * scale, p.y - trunkH);
    ctx.lineTo(p.x + 6 * scale, p.y - trunkH - 3 * scale);
    ctx.lineTo(p.x + 6 * scale, p.y - 4);
    ctx.closePath(); ctx.fill();
    const canopyY = p.y - trunkH - 17 * scale;
    const colors = ['#496d43', '#5c7b4a', '#587546', '#426344'];
    const base = colors[seed % colors.length];
    const gradient = ctx.createRadialGradient(p.x - 11 * scale, canopyY - 9 * scale, 2, p.x, canopyY, 36 * scale);
    gradient.addColorStop(0, '#a0a763'); gradient.addColorStop(.32, base); gradient.addColorStop(1, '#2b4b36');
    ctx.fillStyle = gradient;
    ctx.beginPath();
    ctx.ellipse(p.x, canopyY, 27 * scale, 31 * scale, .1, 0, Math.PI * 2);
    ctx.fill();
    ctx.fillStyle = 'rgba(179,183,111,.25)';
    ctx.beginPath(); ctx.ellipse(p.x - 9 * scale, canopyY - 10 * scale, 10 * scale, 9 * scale, -.4, 0, Math.PI * 2); ctx.fill();
    ctx.fillStyle = 'rgba(38,67,46,.55)';
    ctx.beginPath(); ctx.ellipse(p.x + 12 * scale, canopyY + 8 * scale, 13 * scale, 17 * scale, .5, 0, Math.PI * 2); ctx.fill();
  }

  function drawRock(x, y, scale, seed) {
    const p = project(x, y);
    drawShadow(p.x, p.y - 1, 16 * scale, 5 * scale);
    ctx.beginPath();
    ctx.moveTo(p.x - 15 * scale, p.y - 3 * scale);
    ctx.lineTo(p.x - 9 * scale, p.y - 16 * scale);
    ctx.lineTo(p.x + (seed % 2 ? 3 : 8) * scale, p.y - 21 * scale);
    ctx.lineTo(p.x + 15 * scale, p.y - 9 * scale);
    ctx.lineTo(p.x + 11 * scale, p.y - 2 * scale);
    ctx.closePath();
    const gradient = ctx.createLinearGradient(p.x - 13 * scale, p.y - 20 * scale, p.x + 13 * scale, p.y);
    gradient.addColorStop(0, '#b1a78b'); gradient.addColorStop(.4, '#767d70'); gradient.addColorStop(1, '#424e46');
    ctx.fillStyle = gradient; ctx.fill();
    ctx.strokeStyle = 'rgba(213,211,177,.25)'; ctx.lineWidth = .8; ctx.stroke();
  }

  function drawGate(x, y) {
    const p = project(x, y);
    drawShadow(p.x, p.y, 43, 9);
    const left = project(x - .55, y, 47);
    const right = project(x + .55, y, 47);
    const top = project(x, y, 101);
    ctx.lineCap = 'round';
    ctx.strokeStyle = '#564b39'; ctx.lineWidth = 11;
    ctx.beginPath(); ctx.moveTo(left.x, left.y); ctx.lineTo(project(x - .55, y, 5).x, p.y); ctx.moveTo(right.x, right.y); ctx.lineTo(project(x + .55, y, 5).x, p.y); ctx.stroke();
    ctx.strokeStyle = '#8e7950'; ctx.lineWidth = 4;
    ctx.beginPath(); ctx.moveTo(left.x, left.y); ctx.lineTo(right.x, right.y); ctx.stroke();
    ctx.strokeStyle = 'rgba(220,195,125,.42)'; ctx.lineWidth = 1.2;
    ctx.beginPath(); ctx.moveTo(left.x, left.y - 1); ctx.lineTo(right.x, right.y - 1); ctx.stroke();
    ctx.fillStyle = '#b09b68';
    [left, right].forEach((post) => { ctx.beginPath(); ctx.arc(post.x, post.y - 2, 2.5, 0, Math.PI * 2); ctx.fill(); });
    if (state.questStage >= 4) {
      ctx.fillStyle = 'rgba(229,199,128,.7)'; ctx.font = '8px sans-serif'; ctx.textAlign = 'center'; ctx.fillText('AETHERFALL', p.x, top.y - 6);
    }
  }

  function drawNpc(x, y) {
    const p = project(x, y);
    const bob = Math.sin(state.animTime * 2) * 1.2;
    const hasSprite = images.elaraSheet.complete && images.elaraSheet.naturalWidth;
    const spriteHeight = hasSprite ? 94 : 40;
    drawShadow(p.x, p.y - 1, 14, 4);
    if (hasSprite) {
      const spriteWidth = spriteHeight * spriteSheets.elara.frameWidth / spriteSheets.elara.frameHeight;
      const talking = state.animTime < state.elaraTalk.until;
      drawAtlasFrame(
        images.elaraSheet, spriteSheets.elara, talking ? 'talk' : 'idle', state.animTime,
        state.elaraTalk.startedAt, p.x - spriteWidth / 2, p.y - spriteHeight + bob,
        spriteWidth, spriteHeight,
      );
    } else {
      // Keep the original procedural silhouette as a resilient fallback while the local sheet loads.
      ctx.fillStyle = '#534b61'; ctx.beginPath(); ctx.moveTo(p.x - 13, p.y - 5); ctx.lineTo(p.x - 9, p.y - 28 + bob); ctx.lineTo(p.x, p.y - 39 + bob); ctx.lineTo(p.x + 10, p.y - 27 + bob); ctx.lineTo(p.x + 13, p.y - 5); ctx.quadraticCurveTo(p.x, p.y + 1, p.x - 13, p.y - 5); ctx.fill();
      ctx.fillStyle = '#c6aa70'; ctx.beginPath(); ctx.ellipse(p.x, p.y - 34 + bob, 7, 8, 0, 0, Math.PI * 2); ctx.fill();
      ctx.fillStyle = '#453c46'; ctx.beginPath(); ctx.arc(p.x, p.y - 37 + bob, 8, Math.PI, Math.PI * 2); ctx.lineTo(p.x + 8, p.y - 33 + bob); ctx.lineTo(p.x - 8, p.y - 33 + bob); ctx.closePath(); ctx.fill();
      ctx.strokeStyle = '#dfc384'; ctx.lineWidth = 1; ctx.beginPath(); ctx.moveTo(p.x, p.y - 21 + bob); ctx.lineTo(p.x, p.y - 12); ctx.stroke();
    }
    const questActive = state.questStage < 4;
    if (questActive) {
      const markerY = p.y - spriteHeight - 22 + bob + Math.sin(state.animTime * 3) * 2;
      ctx.fillStyle = 'rgba(10,17,20,.74)'; roundRect(ctx, p.x - 11, markerY - 10, 22, 18, 5); ctx.fill();
      ctx.strokeStyle = 'rgba(226,195,126,.6)'; ctx.lineWidth = .8; ctx.stroke();
      ctx.fillStyle = '#f0d38e'; ctx.font = 'bold 13px Georgia'; ctx.textAlign = 'center'; ctx.textBaseline = 'middle'; ctx.fillText(state.questStage === 0 ? '!' : '⋯', p.x, markerY - 1);
    }
    drawNameplate(p.x, p.y - spriteHeight - 3 + bob, 'VIGIA ELARA', '#e0ca93');
  }

  function drawDungeonGate(x, y) {
    const p = project(x, y);
    drawShadow(p.x, p.y, 33, 9);
    ctx.save();
    ctx.fillStyle = '#31383a';
    ctx.beginPath(); ctx.moveTo(p.x - 23, p.y); ctx.lineTo(p.x - 20, p.y - 34); ctx.quadraticCurveTo(p.x, p.y - 62, p.x + 20, p.y - 34); ctx.lineTo(p.x + 23, p.y); ctx.closePath(); ctx.fill();
    ctx.strokeStyle = '#78735e'; ctx.lineWidth = 4; ctx.stroke();
    ctx.fillStyle = '#111b1d'; ctx.beginPath(); ctx.moveTo(p.x - 13, p.y); ctx.lineTo(p.x - 12, p.y - 31); ctx.quadraticCurveTo(p.x, p.y - 49, p.x + 12, p.y - 31); ctx.lineTo(p.x + 13, p.y); ctx.closePath(); ctx.fill();
    const sealColor = state.wispQuestStage >= 4 ? 'rgba(117,193,177,.9)' : state.questStage >= 4 ? 'rgba(104,205,220,.82)' : 'rgba(215,185,121,.36)';
    ctx.strokeStyle = sealColor;
    ctx.lineWidth = state.questStage >= 4 ? 1.5 : 1;
    ctx.beginPath(); ctx.arc(p.x, p.y - 34, 15 + (state.questStage >= 4 ? Math.sin(state.animTime * 3) * 1.5 : 0), Math.PI, 0); ctx.stroke();
    ctx.restore();
    drawNameplate(p.x, p.y - 60, 'OCA DA VIGÍLIA', '#b9b8a1');
  }

  function drawHound() {
    const hound = state.hound;
    const p = project(hound.x, hound.y);
    const now = state.animTime;
    const bob = hound.aggro ? Math.sin(now * 12) * 2 : Math.sin(now * 4) * 1.3;
    drawShadow(p.x, p.y - 1, 32, 8);
    if (images.houndSheet.complete && images.houndSheet.naturalWidth) {
      const playerScreen = project(state.player.x, state.player.y);
      const animation = now < hound.attackPoseUntil ? 'attack' : hound.aggro && distance(state.player, hound) > 1.45 ? 'run' : 'idle';
      ctx.save();
      if (hound.aggro) ctx.filter = 'saturate(1.16)';
      drawAtlasFrame(images.houndSheet, spriteSheets.hound, animation, now, hound.attackPoseStartedAt,
        p.x - 72, p.y - 120 + bob, 144, 120, playerScreen.x > p.x);
      ctx.restore();
    } else if (images.hound.complete && images.hound.naturalWidth) {
      const scale = 105 / images.hound.height;
      const w = images.hound.width * scale;
      const h = images.hound.height * scale;
      ctx.save();
      if (hound.aggro) ctx.filter = 'saturate(1.16)';
      ctx.drawImage(images.hound, p.x - w * .5, p.y - h + bob, w, h);
      ctx.restore();
    } else {
      ctx.fillStyle = '#776054'; ctx.beginPath(); ctx.ellipse(p.x, p.y - 15 + bob, 27, 13, -.15, 0, Math.PI * 2); ctx.fill();
      ctx.fillStyle = '#544342'; ctx.beginPath(); ctx.arc(p.x - 19, p.y - 22 + bob, 10, 0, Math.PI * 2); ctx.fill();
    }
    if (state.target === 'hound') {
      ctx.strokeStyle = 'rgba(242,201,126,.86)'; ctx.lineWidth = 1.1; ctx.setLineDash([4, 3]);
      ctx.beginPath(); ctx.ellipse(p.x, p.y - 2, 35, 9, 0, 0, Math.PI * 2); ctx.stroke(); ctx.setLineDash([]);
      drawEntityHealth(p.x, p.y - 132, hound.hp, hound.maxHp);
      drawNameplate(p.x, p.y - 140, 'CÃO SELVAGEM · NV. 2', '#e6c4a5');
    } else if (hound.aggro) {
      ctx.fillStyle = '#deaa83'; ctx.font = 'bold 11px sans-serif'; ctx.textAlign = 'center'; ctx.fillText('!', p.x, p.y - 126);
    }
  }

  function drawWisp() {
    const wisp = state.wisp;
    const p = project(wisp.x, wisp.y);
    const now = state.animTime;
    const bob = Math.sin(now * (wisp.aggro ? 7 : 3.2)) * (wisp.aggro ? 3 : 2.2);
    const auraRadius = 48 + Math.sin(now * 3.5) * 5;
    drawShadow(p.x, p.y - 1, 24, 6);
    const aura = ctx.createRadialGradient(p.x, p.y - 56 + bob, 2, p.x, p.y - 56 + bob, auraRadius);
    aura.addColorStop(0, 'rgba(89,220,231,.2)');
    aura.addColorStop(.62, 'rgba(68,175,193,.08)');
    aura.addColorStop(1, 'rgba(48,140,165,0)');
    ctx.fillStyle = aura;
    ctx.beginPath(); ctx.arc(p.x, p.y - 56 + bob, auraRadius, 0, Math.PI * 2); ctx.fill();

    if (images.wispSheet.complete && images.wispSheet.naturalWidth) {
      const animation = now < wisp.attackPoseUntil ? 'pulse' : wisp.aggro && distance(state.player, wisp) > 5.1 ? 'glide' : 'idle';
      drawAtlasFrame(images.wispSheet, spriteSheets.wisp, animation, now, wisp.attackPoseStartedAt,
        p.x - 66, p.y - 112 + bob, 132, 110, state.player.x > wisp.x);
    } else {
      const orb = ctx.createRadialGradient(p.x - 9, p.y - 58 + bob, 2, p.x, p.y - 52 + bob, 33);
      orb.addColorStop(0, '#c9ffff'); orb.addColorStop(.32, '#63d7e2'); orb.addColorStop(1, '#17697b');
      ctx.fillStyle = orb; ctx.beginPath(); ctx.ellipse(p.x, p.y - 52 + bob, 24, 31, -.12, 0, Math.PI * 2); ctx.fill();
      ctx.strokeStyle = 'rgba(138,240,244,.7)'; ctx.lineWidth = 3;
      ctx.beginPath(); ctx.moveTo(p.x - 20, p.y - 50 + bob); ctx.quadraticCurveTo(p.x - 38, p.y - 22 + bob, p.x - 15, p.y - 14); ctx.moveTo(p.x + 19, p.y - 57 + bob); ctx.quadraticCurveTo(p.x + 38, p.y - 29 + bob, p.x + 21, p.y - 16); ctx.stroke();
      ctx.fillStyle = '#182d35'; ctx.beginPath(); ctx.ellipse(p.x, p.y - 63 + bob, 19, 13, 0, 0, Math.PI * 2); ctx.fill();
      ctx.fillStyle = '#e7b66e'; ctx.beginPath(); ctx.arc(p.x - 7, p.y - 63 + bob, 2.2, 0, Math.PI * 2); ctx.arc(p.x + 7, p.y - 63 + bob, 2.2, 0, Math.PI * 2); ctx.fill();
    }

    if (state.target === 'wisp') {
      ctx.strokeStyle = 'rgba(130,225,231,.9)'; ctx.lineWidth = 1.1; ctx.setLineDash([4, 3]);
      ctx.beginPath(); ctx.ellipse(p.x, p.y - 4, 32, 9, 0, 0, Math.PI * 2); ctx.stroke(); ctx.setLineDash([]);
      drawEntityHealth(p.x, p.y - 132, wisp.hp, wisp.maxHp);
      drawNameplate(p.x, p.y - 140, 'WISP DE ÉTER · NV. 4', '#a6e1e2');
    } else if (wisp.aggro) {
      ctx.fillStyle = '#9ce4e7'; ctx.font = 'bold 11px sans-serif'; ctx.textAlign = 'center'; ctx.fillText('!', p.x, p.y - 126);
    }
  }

  function drawProjectiles() {
    state.projectiles.forEach((projectile) => {
      const progress = clamp((state.animTime - projectile.born) / projectile.duration, 0, 1);
      const from = project(projectile.start.x, projectile.start.y, 54);
      const to = project(projectile.target.x, projectile.target.y, 28);
      const x = from.x + (to.x - from.x) * progress;
      const y = from.y + (to.y - from.y) * progress;
      ctx.save();
      ctx.globalAlpha = .9 - progress * .15;
      ctx.strokeStyle = 'rgba(114,224,236,.55)'; ctx.lineWidth = 2;
      ctx.beginPath(); ctx.moveTo(from.x, from.y); ctx.lineTo(x, y); ctx.stroke();
      const glow = ctx.createRadialGradient(x, y, 1, x, y, 14);
      glow.addColorStop(0, 'rgba(220,255,252,.95)'); glow.addColorStop(.34, 'rgba(96,221,234,.82)'); glow.addColorStop(1, 'rgba(40,145,188,0)');
      ctx.fillStyle = glow; ctx.beginPath(); ctx.arc(x, y, 14, 0, Math.PI * 2); ctx.fill();
      ctx.fillStyle = '#d5ffff'; ctx.beginPath(); ctx.arc(x, y, 3.3, 0, Math.PI * 2); ctx.fill();
      ctx.restore();
    });
  }

  function drawPlayer() {
    const player = state.player;
    const p = project(player.x, player.y);
    const now = state.animTime;
    const movingBob = player.moving ? Math.sin(now * 13) * 2 : Math.sin(now * 2.3) * 1.3;
    const attacking = now < player.attackPoseUntil;
    drawShadow(p.x, p.y, 20, 6);
    if (player.equipped.wings) drawWings(p, now, movingBob);
    if (attacking) {
      const flareX = p.x + player.direction * 38;
      const flareY = p.y - 66;
      const radius = 18 + Math.sin(now * 25) * 5;
      const glow = ctx.createRadialGradient(flareX, flareY, 1, flareX, flareY, radius);
      glow.addColorStop(0, 'rgba(255,224,153,.85)'); glow.addColorStop(1, 'rgba(169,207,225,0)');
      ctx.fillStyle = glow; ctx.beginPath(); ctx.arc(flareX, flareY, radius, 0, Math.PI * 2); ctx.fill();
      ctx.strokeStyle = 'rgba(232,201,132,.78)'; ctx.lineWidth = 2; ctx.beginPath(); ctx.arc(flareX, flareY, radius * .55, -1.5, .8); ctx.stroke();
    }
    if (images.mageSheet.complete && images.mageSheet.naturalWidth) {
      const animation = attacking ? 'cast' : player.moving ? 'walk' : 'idle';
      const spriteHeight = 128;
      const spriteWidth = spriteHeight * spriteSheets.mage.frameWidth / spriteSheets.mage.frameHeight;
      drawAtlasFrame(images.mageSheet, spriteSheets.mage, animation, now, player.attackPoseStartedAt,
        p.x - spriteWidth / 2, p.y - spriteHeight + movingBob, spriteWidth, spriteHeight, player.direction < 0);
    } else if (images.mage.complete && images.mage.naturalWidth) {
      const h = 116;
      const w = h * images.mage.width / images.mage.height;
      ctx.save();
      if (player.direction < 0) { ctx.translate(p.x, 0); ctx.scale(-1, 1); ctx.translate(-p.x, 0); }
      ctx.drawImage(images.mage, p.x - w / 2, p.y - h + movingBob, w, h);
      ctx.restore();
    } else {
      drawFallbackPlayer(p.x, p.y, movingBob);
    }
    const selectedEnemy = getEnemy();
    if (selectedEnemy?.alive && distance(player, selectedEnemy) < 2.15) {
      ctx.strokeStyle = 'rgba(132,208,177,.6)'; ctx.lineWidth = 1; ctx.beginPath(); ctx.ellipse(p.x, p.y - 1, 24, 8, 0, 0, Math.PI * 2); ctx.stroke();
    }
  }

  function drawWings(p, now, bob) {
    const flap = Math.sin(now * 6) * 4;
    const hue = state.player.equipped.wings === 'ember-wings' || state.player.equipped.wings === 'dragon-wings' ? 'rgba(255,169,93,' : 'rgba(139,223,229,';
    ctx.save(); ctx.globalAlpha = .84;
    ctx.shadowColor = state.player.equipped.wings === 'dragon-wings' ? '#ff9b62' : '#7ddbe6'; ctx.shadowBlur = 13;
    [-1, 1].forEach((side) => {
      ctx.fillStyle = `${hue}.28)`; ctx.strokeStyle = `${hue}.9)`; ctx.lineWidth = 1.6;
      ctx.beginPath();
      ctx.moveTo(p.x + side * 6, p.y - 60 + bob);
      ctx.quadraticCurveTo(p.x + side * (29 + flap), p.y - 100, p.x + side * 42, p.y - 84 + bob);
      ctx.quadraticCurveTo(p.x + side * 32, p.y - 66 + bob, p.x + side * 11, p.y - 43 + bob);
      ctx.closePath(); ctx.fill(); ctx.stroke();
      for (let feather = 0; feather < 3; feather += 1) {
        ctx.beginPath(); ctx.moveTo(p.x + side * (10 + feather * 4), p.y - (55 + feather * 7) + bob);
        ctx.lineTo(p.x + side * (29 + feather * 3 + flap), p.y - (88 - feather * 6) + bob); ctx.stroke();
      }
    });
    ctx.restore();
  }

  function drawFallbackPlayer(x, y, bob) {
    ctx.fillStyle = '#252b41'; ctx.beginPath(); ctx.moveTo(x - 15, y - 4); ctx.lineTo(x - 13, y - 47 + bob); ctx.lineTo(x, y - 70 + bob); ctx.lineTo(x + 15, y - 48 + bob); ctx.lineTo(x + 14, y - 3); ctx.closePath(); ctx.fill();
    ctx.fillStyle = '#d0a47c'; ctx.beginPath(); ctx.arc(x, y - 73 + bob, 9, 0, Math.PI * 2); ctx.fill();
    ctx.fillStyle = '#1d2637'; ctx.beginPath(); ctx.arc(x, y - 78 + bob, 10, Math.PI, Math.PI * 2); ctx.fill();
  }

  function drawFarmMob(mob) {
    const p = project(mob.x, mob.y);
    const now = state.animTime;
    const bob = Math.sin(now * (mob.aggro ? 8 : 3.2) + mob.seed) * (mob.aggro ? 2.2 : 1.2);
    drawShadow(p.x, p.y, 22, 6);
    if (mob.kind === 'wisp' && images.wispSheet.complete && images.wispSheet.naturalWidth) {
      drawAtlasFrame(images.wispSheet, spriteSheets.wisp, mob.aggro ? 'glide' : 'idle', now, mob.attackPoseStartedAt, p.x - 42, p.y - 74 + bob, 84, 70, state.player.x > mob.x);
    } else if (images.houndSheet.complete && images.houndSheet.naturalWidth) {
      ctx.save(); ctx.filter = mob.aggro ? `hue-rotate(${(mob.level * 11) % 360}deg) saturate(1.2)` : `hue-rotate(${(mob.level * 11) % 360}deg)`;
      drawAtlasFrame(images.houndSheet, spriteSheets.hound, now < mob.attackPoseUntil ? 'attack' : mob.aggro ? 'run' : 'idle', now, mob.attackPoseStartedAt, p.x - 48, p.y - 77 + bob, 96, 80, state.player.x > mob.x);
      ctx.restore();
    } else {
      ctx.fillStyle = mob.color; ctx.beginPath(); ctx.ellipse(p.x, p.y - 17 + bob, 20, 12, -.12, 0, Math.PI * 2); ctx.fill();
      ctx.fillStyle = '#332d35'; ctx.beginPath(); ctx.arc(p.x - 13, p.y - 24 + bob, 8, 0, Math.PI * 2); ctx.fill();
    }
    if (state.target === mob.id) {
      ctx.strokeStyle = 'rgba(242,201,126,.9)'; ctx.lineWidth = 1.2; ctx.setLineDash([4, 3]); ctx.beginPath(); ctx.ellipse(p.x, p.y - 2, 26, 7, 0, 0, Math.PI * 2); ctx.stroke(); ctx.setLineDash([]);
      drawEntityHealth(p.x, p.y - 84, mob.hp, mob.maxHp);
      drawNameplate(p.x, p.y - 94, `${mob.name.toUpperCase()} · NV. ${mob.level}`, '#f0d7a2');
    } else if (mob.aggro) {
      ctx.fillStyle = '#e9ad89'; ctx.font = 'bold 9px sans-serif'; ctx.textAlign = 'center'; ctx.fillText('!', p.x, p.y - 76);
    }
  }

  function drawPet() {
    const p = project(state.pet.x, state.pet.y);
    const bob = Math.sin(state.animTime * 7) * 2;
    drawShadow(p.x, p.y, 15, 4);
    ctx.save();
    ctx.translate(p.x, p.y + bob);
    ctx.fillStyle = '#e19a4e'; ctx.beginPath(); ctx.moveTo(-10, -12); ctx.quadraticCurveTo(-25, -24, -21, -36); ctx.quadraticCurveTo(-12, -29, -6, -19); ctx.fill();
    ctx.fillStyle = '#c66f3e'; ctx.beginPath(); ctx.ellipse(0, -17, 13, 9, -.08, 0, Math.PI * 2); ctx.fill();
    ctx.beginPath(); ctx.moveTo(7, -24); ctx.lineTo(10, -35); ctx.lineTo(15, -25); ctx.closePath(); ctx.fill();
    ctx.beginPath(); ctx.moveTo(-2, -25); ctx.lineTo(-1, -35); ctx.lineTo(5, -25); ctx.closePath(); ctx.fill();
    ctx.fillStyle = '#f4cb89'; ctx.beginPath(); ctx.ellipse(10, -22, 6, 4.5, 0, 0, Math.PI * 2); ctx.fill();
    ctx.fillStyle = '#2f2826'; ctx.beginPath(); ctx.arc(11, -24, 1.1, 0, Math.PI * 2); ctx.fill();
    ctx.fillStyle = '#ffc875'; ctx.shadowColor = '#ffb750'; ctx.shadowBlur = 8; ctx.beginPath(); ctx.arc(-14, -34, 2.3 + Math.sin(state.animTime * 6) * .4, 0, Math.PI * 2); ctx.fill();
    ctx.restore();
  }

  function drawItemDrop(drop) {
    const item = itemCatalog[drop.itemId];
    if (!item) return;
    const p = project(drop.x, drop.y, 1);
    const pulse = 1 + Math.sin(state.animTime * 4.2 + drop.born) * .12;
    ctx.save();
    const glow = ctx.createRadialGradient(p.x, p.y - 11, 1, p.x, p.y - 11, 21 * pulse);
    glow.addColorStop(0, 'rgba(250,208,119,.76)'); glow.addColorStop(1, 'rgba(226,159,77,0)');
    ctx.fillStyle = glow; ctx.beginPath(); ctx.arc(p.x, p.y - 11, 21 * pulse, 0, Math.PI * 2); ctx.fill();
    ctx.translate(p.x, p.y - 12); ctx.rotate(Math.PI / 4); ctx.fillStyle = '#f0c975'; ctx.fillRect(-5, -5, 10, 10); ctx.restore();
    drawNameplate(p.x, p.y - 29, `${item.name.toUpperCase()} · E`, '#f3dc9a');
  }

  function drawSpellVfx() {
    state.spellVfx.forEach((effect) => {
      const age = Math.max(0, state.animTime - effect.born);
      const progress = clamp(age / effect.duration, 0, 1);
      const start = project(effect.start.x, effect.start.y, effect.type === 'heal' ? 72 : 65);
      const target = project(effect.target.x, effect.target.y, effect.type === 'heal' ? 4 : 58);
      ctx.save();
      if (effect.type === 'heal') {
        const fade = Math.max(0, 1 - age / (effect.duration + .45));
        ctx.globalAlpha = fade;
        const radius = 19 + Math.sin(age * 17) * 4 + age * 18;
        const glow = ctx.createRadialGradient(target.x, target.y - 48, 2, target.x, target.y - 48, radius * 1.8);
        glow.addColorStop(0, 'rgba(151,246,192,.36)'); glow.addColorStop(1, 'rgba(99,208,155,0)');
        ctx.fillStyle = glow; ctx.beginPath(); ctx.arc(target.x, target.y - 48, radius * 1.8, 0, Math.PI * 2); ctx.fill();
        ctx.strokeStyle = `rgba(169,255,203,${fade * .85})`; ctx.lineWidth = 2;
        ctx.beginPath(); ctx.ellipse(target.x, target.y - 30, radius, radius * .36, age * 1.8, 0, Math.PI * 2); ctx.stroke();
        for (let index = 0; index < 7; index += 1) {
          const angle = index * Math.PI * 2 / 7 + age * 2.5;
          const x = target.x + Math.cos(angle) * radius * .68;
          const y = target.y - 42 + Math.sin(angle) * radius * .72 - age * 8;
          ctx.fillStyle = `rgba(200,255,216,${fade})`; ctx.beginPath(); ctx.arc(x, y, 1.6 + (index % 2), 0, Math.PI * 2); ctx.fill();
        }
      } else {
        const x = start.x + (target.x - start.x) * progress;
        const y = start.y + (target.y - start.y) * progress - Math.sin(progress * Math.PI) * 22;
        ctx.globalAlpha = Math.max(.15, 1 - age / (effect.duration + .45));
        ctx.strokeStyle = effect.type === 'pet' ? 'rgba(255,196,111,.75)' : effect.type === 'strike' ? 'rgba(240,215,147,.82)' : 'rgba(127,230,245,.78)';
        ctx.lineWidth = effect.type === 'bolt' ? 3 : 2;
        ctx.beginPath(); ctx.moveTo(start.x, start.y); ctx.quadraticCurveTo((start.x + x) / 2 + Math.sin(age * 25) * 9, (start.y + y) / 2 - 15, x, y); ctx.stroke();
        const glow = ctx.createRadialGradient(x, y, 1, x, y, effect.type === 'bolt' ? 19 : 11);
        glow.addColorStop(0, effect.type === 'pet' ? 'rgba(255,243,185,.95)' : 'rgba(225,255,253,.96)');
        glow.addColorStop(.35, effect.type === 'pet' ? 'rgba(255,170,81,.76)' : 'rgba(103,226,239,.8)');
        glow.addColorStop(1, 'rgba(75,152,186,0)');
        ctx.fillStyle = glow; ctx.beginPath(); ctx.arc(x, y, effect.type === 'bolt' ? 19 : 11, 0, Math.PI * 2); ctx.fill();
        if (age > effect.duration) {
          const impact = clamp((age - effect.duration) / .5, 0, 1);
          ctx.globalAlpha = 1 - impact;
          ctx.strokeStyle = 'rgba(176,246,241,.9)'; ctx.lineWidth = 2;
          ctx.beginPath(); ctx.arc(target.x, target.y - 34, 6 + impact * 26, 0, Math.PI * 2); ctx.stroke();
          for (let index = 0; index < 5; index += 1) {
            const angle = index * Math.PI * 2 / 5 + impact * 1.7;
            ctx.fillStyle = '#d9ffff'; ctx.beginPath(); ctx.arc(target.x + Math.cos(angle) * (8 + impact * 20), target.y - 34 + Math.sin(angle) * (8 + impact * 18), 1.6, 0, Math.PI * 2); ctx.fill();
          }
        }
      }
      ctx.restore();
    });
  }

  function drawLevelUpEffect() {
    const effect = state.levelUpEffect;
    if (!effect) return;
    const age = state.animTime - effect.born;
    if (age < 0 || age > effect.duration) return;
    const fade = Math.min(1, (effect.duration - age) * 1.4, age * 3);
    const p = project(state.player.x, state.player.y, 1);
    const radius = 32 + age * 48;
    ctx.save(); ctx.globalAlpha = clamp(fade, 0, 1);
    const glow = ctx.createRadialGradient(p.x, p.y - 63, 3, p.x, p.y - 63, radius * .9);
    glow.addColorStop(0, 'rgba(255,237,173,.6)'); glow.addColorStop(.4, 'rgba(232,192,94,.24)'); glow.addColorStop(1, 'rgba(235,180,67,0)');
    ctx.fillStyle = glow; ctx.beginPath(); ctx.arc(p.x, p.y - 63, radius * .9, 0, Math.PI * 2); ctx.fill();
    ctx.strokeStyle = 'rgba(255,224,139,.9)'; ctx.lineWidth = 2;
    ctx.beginPath(); ctx.ellipse(p.x, p.y - 17, 28 + age * 24, 10 + age * 6, age * 1.5, 0, Math.PI * 2); ctx.stroke();
    for (let index = 0; index < 12; index += 1) {
      const angle = index * Math.PI / 6 + age * 1.8;
      const r = 22 + (index % 3) * 7 + age * 20;
      const x = p.x + Math.cos(angle) * r;
      const y = p.y - 64 + Math.sin(angle) * r - age * (8 + index % 4 * 3);
      ctx.fillStyle = index % 3 ? '#f8e9ac' : '#fff8d7'; ctx.beginPath(); ctx.arc(x, y, 1.4 + (index % 3) * .5, 0, Math.PI * 2); ctx.fill();
    }
    ctx.fillStyle = '#fff0bd'; ctx.font = '800 12px Georgia'; ctx.textAlign = 'center'; ctx.shadowColor = 'rgba(247,195,91,.85)'; ctx.shadowBlur = 9;
    ctx.fillText(`NÍVEL ${effect.level}`, p.x, p.y - 138 - age * 12);
    ctx.restore();
  }

  function drawDrop() {
    const p = project(state.drop.x, state.drop.y, 1);
    const watchCore = state.drop.kind === 'watch-core';
    const pulse = 1 + Math.sin(state.animTime * 4) * .12;
    drawShadow(p.x, p.y, 12, 4);
    const glow = ctx.createRadialGradient(p.x, p.y - 11, 0, p.x, p.y - 11, 22 * pulse);
    glow.addColorStop(0, watchCore ? 'rgba(128,238,231,.78)' : 'rgba(102,210,230,.65)');
    glow.addColorStop(1, watchCore ? 'rgba(57,174,181,0)' : 'rgba(55,158,197,0)');
    ctx.fillStyle = glow; ctx.beginPath(); ctx.arc(p.x, p.y - 11, 22 * pulse, 0, Math.PI * 2); ctx.fill();
    ctx.save(); ctx.translate(p.x, p.y - 12); ctx.rotate(Math.PI / 4); ctx.fillStyle = watchCore ? '#c4fff0' : '#8de0df'; ctx.fillRect(-5, -5, 10, 10); ctx.strokeStyle = '#e3f5d2'; ctx.lineWidth = 1; ctx.strokeRect(-5, -5, 10, 10); ctx.restore();
    ctx.fillStyle = '#d2f0e4'; ctx.font = 'bold 8px sans-serif'; ctx.textAlign = 'center';
    ctx.fillText(watchCore ? 'NÚCLEO DE VIGÍLIA' : 'FRAGMENTO', p.x, p.y - 30);
  }

  function getQuestMarkerObjective() {
    if (!state.started || state.currentDestinationId !== 'aetherfall-city') return null;
    if (state.questStage < 4) {
      if (state.questStage === 0) {
        return { pointId: 'quest-hub', target: questNpc, description: 'Fale com a Vigia Elara na praça.', color: 'rgba(240,213,154,.68)', fill: 'rgba(240,213,154,.2)' };
      }
      if (state.questStage === 3) {
        return { pointId: 'quest-hub', target: questNpc, description: 'Entregue a missão à Vigia Elara.', color: 'rgba(240,213,154,.68)', fill: 'rgba(240,213,154,.2)' };
      }
      if (state.questStage === 2 && state.drop) {
        return { pointId: 'hound-hunt', target: state.drop, description: 'Recolha o fragmento deixado pela criatura.', color: 'rgba(240,213,154,.68)', fill: 'rgba(240,213,154,.2)' };
      }
      return {
        pointId: 'hound-hunt', target: houndSpawn,
        description: state.questStage === 2 ? 'Procure o fragmento na área da caçada.' : 'Siga a estrada e encontre o Cão Selvagem.',
        color: 'rgba(240,213,154,.68)', fill: 'rgba(240,213,154,.2)',
      };
    }
    if (state.wispQuestStage >= 4) return null;
    if (state.wispQuestStage === 0) {
      return { pointId: 'ruins-landmark', target: hollowedWatch, description: 'Investigue as runas da Oca da Vigília.', color: 'rgba(126,223,232,.78)', fill: 'rgba(126,223,232,.18)' };
    }
    if (state.wispQuestStage === 3) {
      return { pointId: 'ruins-landmark', target: hollowedWatch, description: 'Leve o núcleo até o selo antigo.', color: 'rgba(126,223,232,.78)', fill: 'rgba(126,223,232,.18)' };
    }
    if (state.wispQuestStage === 2 && state.drop) {
      return { pointId: 'wisp-nest', target: state.drop, description: 'Recolha o núcleo de vigília.', color: 'rgba(126,223,232,.78)', fill: 'rgba(126,223,232,.18)' };
    }
    return {
      pointId: 'wisp-nest', target: state.wisp,
      description: state.wispQuestStage === 2 ? 'Procure o Wisp na trilha.' : 'Derrote o Wisp que protege o selo.',
      color: 'rgba(126,223,232,.78)', fill: 'rgba(126,223,232,.18)',
    };
  }

  function drawQuestMarkers() {
    const objective = getQuestMarkerObjective();
    if (!objective?.target) return;
    const p = project(objective.target.x, objective.target.y, 100);
    const pulse = 1 + Math.sin(state.animTime * 3.2) * .12;
    ctx.save(); ctx.translate(p.x, p.y - 18); ctx.scale(pulse, pulse);
    ctx.strokeStyle = objective.color; ctx.lineWidth = 1;
    ctx.beginPath(); ctx.moveTo(0, -8); ctx.lineTo(5, -3); ctx.lineTo(0, 2); ctx.lineTo(-5, -3); ctx.closePath(); ctx.stroke();
    ctx.fillStyle = objective.fill; ctx.fill(); ctx.restore();
  }

  function drawWaypointMarker() {
    if (!state.waypoint || distance(state.player, state.waypoint) < .75) return;
    const p = project(state.waypoint.x, state.waypoint.y, 8);
    const pulse = 1 + Math.sin(state.animTime * 3.5) * .14;
    ctx.save();
    ctx.translate(p.x, p.y - 8);
    ctx.scale(pulse, pulse);
    ctx.strokeStyle = 'rgba(240,215,159,.92)';
    ctx.lineWidth = 1.6;
    ctx.beginPath(); ctx.moveTo(0, -10); ctx.lineTo(7, -3); ctx.lineTo(0, 4); ctx.lineTo(-7, -3); ctx.closePath(); ctx.stroke();
    ctx.fillStyle = 'rgba(240,215,159,.22)'; ctx.fill();
    ctx.restore();
    drawNameplate(p.x, p.y - 22, 'ROTA MARCADA', '#f0d79f');
  }

  function drawFloatingText() {
    const now = state.animTime;
    state.floating.forEach((item) => {
      const age = now - item.born;
      ctx.save(); ctx.globalAlpha = Math.max(0, 1 - age);
      ctx.fillStyle = item.color; ctx.font = `bold ${item.size}px ${item.size > 18 ? 'Georgia' : 'system-ui'}`; ctx.textAlign = 'center';
      ctx.shadowColor = 'rgba(0,0,0,.8)'; ctx.shadowBlur = 5;
      ctx.fillText(item.text, item.x, item.y - age * 34); ctx.restore();
    });
  }

  function drawAmbientParticles() {
    for (let i = 0; i < 13; i += 1) {
      const x = (i * 107 + Math.sin(state.animTime * .35 + i) * 31) % width;
      const y = (i * 71 + (state.animTime * (5 + i % 3) + i * 29) % height) % height;
      const alpha = .12 + ((Math.sin(state.animTime * 1.7 + i * 2.4) + 1) * .08);
      ctx.fillStyle = `rgba(238,220,162,${alpha})`;
      ctx.beginPath(); ctx.arc(x, y, i % 4 === 0 ? 1.2 : .7, 0, Math.PI * 2); ctx.fill();
    }
  }

  function drawEntityHealth(x, y, hp, maxHp) {
    const barW = 77; const barH = 4;
    ctx.fillStyle = 'rgba(5,10,12,.75)'; roundRect(ctx, x - barW / 2, y, barW, barH, 2); ctx.fill();
    ctx.fillStyle = '#c76b66'; roundRect(ctx, x - barW / 2, y, barW * (hp / maxHp), barH, 2); ctx.fill();
  }

  function drawNameplate(x, y, text, color) {
    ctx.font = '700 7px system-ui';
    ctx.textAlign = 'center';
    const w = ctx.measureText(text).width + 12;
    ctx.fillStyle = 'rgba(10,15,17,.64)'; roundRect(ctx, x - w / 2, y - 8, w, 13, 3); ctx.fill();
    ctx.fillStyle = color; ctx.fillText(text, x, y + 1);
  }

  function drawShadow(x, y, rx, ry) {
    ctx.fillStyle = 'rgba(12,17,15,.34)'; ctx.beginPath(); ctx.ellipse(x, y, rx, ry, 0, 0, Math.PI * 2); ctx.fill();
  }

  function roundRect(context, x, y, w, h, r) {
    const radius = Math.min(r, w / 2, h / 2);
    context.beginPath(); context.moveTo(x + radius, y); context.arcTo(x + w, y, x + w, y + h, radius); context.arcTo(x + w, y + h, x, y + h, radius); context.arcTo(x, y + h, x, y, radius); context.arcTo(x, y, x + w, y, radius); context.closePath();
  }

  function frame(time) {
    const dt = Math.min(.05, Math.max(0, (time - lastFrame) / 1000));
    lastFrame = time;
    fpsFrames += 1; fpsTimer += dt;
    if (fpsTimer > .6) { $('#fps-label').textContent = `${Math.round(fpsFrames / fpsTimer)} FPS`; fpsFrames = 0; fpsTimer = 0; }
    update(dt, time);
    draw();
    requestAnimationFrame(frame);
  }

  function handlePointerDown(event) {
    if (!state.started || state.dead || state.paused || event.target.closest('.intro-card') || event.target.closest('.defeat-card')) return;
    const rect = canvas.getBoundingClientRect();
    const sx = event.clientX - rect.left;
    const sy = event.clientY - rect.top;
    const clickableEnemies = [
      ...(state.currentDestinationId === 'aetherfall-city' && state.hound.alive ? [{ id: 'hound', enemy: state.hound, offsetY: 46, radius: 70 }] : []),
      ...(state.currentDestinationId === 'aetherfall-city' && state.wisp.alive ? [{ id: 'wisp', enemy: state.wisp, offsetY: 58, radius: 62 }] : []),
      ...state.mobs.filter((mob) => mob.alive).map((mob) => ({ id: mob.id, enemy: mob, offsetY: mob.kind === 'wisp' ? 42 : 36, radius: 50 })),
    ];
    for (const candidate of clickableEnemies) {
      const point = project(candidate.enemy.x, candidate.enemy.y, 0);
      if (Math.hypot(sx - point.x, sy - (point.y - candidate.offsetY)) >= candidate.radius) continue;
      state.target = candidate.id;
      if (distance(state.player, candidate.enemy) < 2.15) useBasicAttack();
      else {
        state.moveTarget = { x: candidate.enemy.x - Math.sign(candidate.enemy.x - state.player.x) * 1.2, y: candidate.enemy.y - Math.sign(candidate.enemy.y - state.player.y) * 1.2 };
        addToast('ALVO SELECIONADO · APROXIME-SE PARA ATACAR');
      }
      return;
    }
    const world = inverseProject(sx, sy);
    state.moveTarget = { x: clamp(world.x, .5, MAX_WORLD - .5), y: clamp(world.y, .5, MAX_WORLD - .5) };
    if (Math.hypot(state.moveTarget.x - state.player.x, state.moveTarget.y - state.player.y) < .18) state.moveTarget = null;
  }

  function startGame() {
    if (state.started) return;
    state.started = true;
    $('#intro-card').classList.add('hidden');
    $('#world-tip-text').textContent = 'Cace nos pontos de farm pelo mapa para ganhar XP. As histórias de Elara são opcionais e dão ouro/itens.';
    addToast('BEM-VINDA A AETHERFALL, AELWYN');
    playTone(520, .12, 'sine');
    canvas.focus();
  }

  function revive() {
    state.dead = false;
    state.player.hp = Math.max(55, Math.floor(state.player.maxHp * .72));
    state.player.mana = state.player.maxMana;
    state.itemDropsByDestination[state.currentDestinationId] = state.itemDrops;
    state.currentDestinationId = initialDestination.id;
    state.player.x = startPoint.x; state.player.y = startPoint.y;
    state.camera.x = state.player.x; state.camera.y = state.player.y;
    state.localOrigin = { x: startPoint.x, y: startPoint.y, xKm: initialDestination.xKm, yKm: initialDestination.yKm };
    state.worldLocation = { xKm: initialDestination.xKm, yKm: initialDestination.yKm };
    state.itemDrops = state.itemDropsByDestination[initialDestination.id] || [];
    state.target = null; state.moveTarget = null;
    state.pet.x = startPoint.x - .7; state.pet.y = startPoint.y + .7;
    state.hound.aggro = false;
    state.wisp.aggro = false;
    state.projectiles = [];
    spawnMobSpot(initialDestination);
    $('#defeat-card').classList.add('hidden');
    addToast('VOCÊ RETORNOU À PRAÇA');
  }

  function playTone(frequency, duration, type = 'sine') {
    if (!soundsEnabled) return;
    try {
      audioContext ??= new (window.AudioContext || window.webkitAudioContext)();
      if (audioContext.state === 'suspended') audioContext.resume();
      const oscillator = audioContext.createOscillator();
      const gain = audioContext.createGain();
      oscillator.type = type;
      oscillator.frequency.setValueAtTime(frequency, audioContext.currentTime);
      oscillator.frequency.exponentialRampToValueAtTime(Math.max(45, frequency * .74), audioContext.currentTime + duration);
      gain.gain.setValueAtTime(.028, audioContext.currentTime);
      gain.gain.exponentialRampToValueAtTime(.001, audioContext.currentTime + duration);
      oscillator.connect(gain); gain.connect(audioContext.destination);
      oscillator.start(); oscillator.stop(audioContext.currentTime + duration);
    } catch (_) { /* Audio is optional; gameplay remains fully local. */ }
  }

  function mapSvgPosition(x, y) {
    const scale = regionMap.coordinateScale;
    return {
      x: scale.offsetX + (x - regionMap.bounds.minX) * scale.x,
      y: scale.offsetY + (y - regionMap.bounds.minY) * scale.y,
    };
  }

  function mapSvgPolyline(points, close = false) {
    const mapped = points.map(([x, y]) => mapSvgPosition(x, y));
    if (!mapped.length) return '';
    const path = `M ${mapped[0].x.toFixed(1)} ${mapped[0].y.toFixed(1)} ${mapped.slice(1).map((point) => `L ${point.x.toFixed(1)} ${point.y.toFixed(1)}`).join(' ')}`;
    return close ? `${path} Z` : path;
  }

  function currentMapCell() {
    const width = (regionMap.bounds.maxX - regionMap.bounds.minX) / 3;
    const height = (regionMap.bounds.maxY - regionMap.bounds.minY) / 4;
    const col = clamp(Math.floor((state.player.x - regionMap.bounds.minX) / width), 0, 2);
    const row = clamp(Math.floor((state.player.y - regionMap.bounds.minY) / height), 0, 3);
    return regionMap.cells.find((cell) => cell.col === col && cell.row === row) || regionMap.cells[0];
  }

  function getMapPointSnapshot(point) {
    if (point.id === 'hound-hunt' && (state.drop?.kind === 'wild-fragment' || state.drop?.kind === 'shard')) {
      return { ...point, name: 'Fragmento de Éter', x: state.drop.x, y: state.drop.y, icon: '◆' };
    }
    if (point.id === 'wisp-nest' && state.drop?.kind === 'watch-core') {
      return { ...point, name: 'Núcleo de Vigília', x: state.drop.x, y: state.drop.y, icon: '✧' };
    }
    if (point.id === 'hound-hunt') return { ...point, x: houndSpawn.x, y: houndSpawn.y };
    if (point.id === 'wisp-nest') return { ...point, x: state.wisp.x, y: state.wisp.y };
    return point;
  }

  function getMapPointAvailability(point) {
    if (point.status === 'planned') return { available: false, label: 'PLANEJADO' };
    if (point.id === 'hound-hunt') {
      if (state.questStage === 0) return { available: false, label: 'REVELADO PELA MISSÃO' };
      if (state.questStage >= 3) return { available: true, label: 'ÁREA LIBERADA' };
      return { available: true, label: state.drop?.kind === 'wild-fragment' || state.drop?.kind === 'shard' ? 'FRAGMENTO NO CHÃO' : 'OBJETIVO' };
    }
    if (point.id === 'wisp-nest') {
      if (state.questStage < 4 || state.wispQuestStage === 0) return { available: false, label: 'SELO ADORMECIDO' };
      if (state.wispQuestStage >= 3) return { available: true, label: 'ÁREA LIBERADA' };
      return { available: true, label: state.drop?.kind === 'watch-core' ? 'NÚCLEO NO CHÃO' : 'OBJETIVO' };
    }
    if (point.id === 'dungeon-approach' || point.id === 'dungeon-entrance') return { available: false, label: 'CONTEÚDO FUTURO' };
    if (point.id === 'ruins-landmark' && state.questStage < 4) return { available: true, label: 'MARCO SELADO' };
    if (point.id === 'quest-hub' && state.questStage === 4 && state.wispQuestStage === 4) return { available: true, label: 'MISSÕES CONCLUÍDAS' };
    return { available: true, label: point.status === 'planned' ? 'PLANEJADO' : 'VISITÁVEL' };
  }

  function getCurrentMapObjective() {
    const markerObjective = getQuestMarkerObjective();
    if (!markerObjective) {
      if (!state.started) {
        return { ...pointsById.get('quest-hub'), description: 'Fale com a Vigia Elara na praça.' };
      }
      return null;
    }
    const point = pointsById.get(markerObjective.pointId);
    return {
      ...getMapPointSnapshot(point),
      x: markerObjective.target.x,
      y: markerObjective.target.y,
      description: markerObjective.description,
    };
  }

  function mapPointIsVisible(point) {
    if (point.id === 'hound-hunt') return state.questStage >= 1;
    if (point.id === 'wisp-nest') return state.questStage >= 4 && state.wispQuestStage >= 1;
    return true;
  }

  function buildMapMarker(point, objective) {
    if (!mapPointIsVisible(point)) return '';
    const snapshot = getMapPointSnapshot(point);
    const position = mapSvgPosition(snapshot.x, snapshot.y);
    const availability = getMapPointAvailability(point);
    const isObjective = objective && objective.id === point.id;
    const markerClasses = [
      'map-marker', `map-marker--${point.kind}`,
      point.status === 'planned' ? 'is-planned' : '',
      availability.available ? '' : 'is-locked',
      isObjective ? 'is-objective' : '',
    ].filter(Boolean).join(' ');
    const labelIds = ['settlement-plaza', 'quest-hub', 'settlement-gate', 'hound-hunt', 'ruins-landmark', 'dungeon-entrance'];
    const labelY = point.id === 'ruins-landmark' ? -13 : 23;
    const label = labelIds.includes(point.id)
      ? `<text class="map-marker-label" y="${labelY}">${escapeHtml(snapshot.name)}</text>`
      : '';
    return `<g class="${markerClasses}" data-map-point-id="${escapeHtml(point.id)}" transform="translate(${position.x.toFixed(1)} ${position.y.toFixed(1)})" tabindex="0" role="button" aria-label="${escapeHtml(snapshot.name)} — ${escapeHtml(availability.label)}"><title>${escapeHtml(snapshot.name)} · ${escapeHtml(availability.label)}</title><circle class="map-marker-halo" r="13"></circle><circle class="map-marker-disc" r="8.5"></circle><text class="map-marker-glyph" y="3.4">${escapeHtml(snapshot.icon)}</text>${label}</g>`;
  }

  function buildMapPointRow(point) {
    const availability = getMapPointAvailability(point);
    const snapshot = getMapPointSnapshot(point);
    const currentObjective = getCurrentMapObjective();
    const isObjective = currentObjective && currentObjective.id === point.id;
    const classes = ['world-map-point', point.status === 'planned' ? 'is-planned' : '', availability.available ? '' : 'is-locked', isObjective ? 'is-objective' : ''].filter(Boolean).join(' ');
    const detail = point.status === 'planned'
      ? 'Previsto no roadmap da região'
      : point.id === 'hound-hunt' || point.id === 'wisp-nest'
        ? availability.label.toLowerCase()
        : point.kind === 'service'
          ? 'Serviço futuro · sem interação nesta fatia'
          : availability.label.toLowerCase();
    return `<button class="${classes}" type="button" data-map-point-id="${escapeHtml(point.id)}"><span class="world-map-point-glyph">${escapeHtml(point.icon)}</span><span class="world-map-point-copy"><strong>${escapeHtml(snapshot.name)}</strong><small>${escapeHtml(detail)}</small></span><span class="world-map-point-state">${isObjective ? 'ROTA' : point.status === 'planned' ? 'FUTURO' : '⌖'}</span></button>`;
  }

  function globalMapPosition(xKm, yKm) {
    const border = 70;
    const scale = 8.6;
    return { x: border + xKm * scale, y: border + yKm * scale };
  }

  function buildWorldDestinationRow(destination) {
    const selected = state.selectedWorldDestinationId === destination.id;
    const isCity = destination.kind === 'city';
    const detail = isCity
      ? 'CIDADE · ZONA SEGURA'
      : `CAÇA · NV. ${destination.levelMin}–${destination.levelMax} · 10 MOBs`;
    const coords = `${destination.xKm}, ${destination.yKm} km`;
    return `<button class="world-map-point world-destination-point ${selected ? 'is-selected' : ''} ${isCity ? 'is-city' : 'is-hunt'}" type="button" data-world-destination-id="${escapeHtml(destination.id)}" aria-label="${escapeHtml(destination.name)} · ${coords} · ${detail}"><span class="world-map-point-glyph">${escapeHtml(destination.icon)}</span><span class="world-map-point-copy"><strong>${escapeHtml(destination.name)}</strong><small>${coords} · ${detail}</small></span><span class="world-map-point-state">${selected ? 'SELEC.' : isCity ? 'CIDADE' : 'TELEPORTAR'}</span></button>`;
  }

  function buildOverworldSvg() {
    const mapTerrain = overworldMap.terrain.map((terrain) => {
      const point = globalMapPosition(terrain.xKm, terrain.yKm);
      const rx = (terrain.rxKm * 8.6).toFixed(1);
      const ry = (terrain.ryKm * 8.6).toFixed(1);
      const labelY = (point.y + Number(ry) * .12).toFixed(1);
      return `<g class="overworld-terrain overworld-terrain--${escapeHtml(terrain.tone)}"><ellipse cx="${point.x.toFixed(1)}" cy="${point.y.toFixed(1)}" rx="${rx}" ry="${ry}"></ellipse><text x="${point.x.toFixed(1)}" y="${labelY}">${escapeHtml(terrain.name.toUpperCase())}</text></g>`;
    }).join('');
    const gridLines = [];
    for (let km = 0; km <= 100; km += overworldMap.gridStepKm) {
      const position = globalMapPosition(km, km);
      gridLines.push(`<path class="overworld-grid-line" d="M${position.x.toFixed(1)} 70V930 M70 ${position.y.toFixed(1)}H930"></path><text class="overworld-grid-label" x="${position.x.toFixed(1)}" y="954">${km}</text><text class="overworld-grid-label overworld-grid-label--side" x="49" y="${(position.y + 3).toFixed(1)}">${km}</text>`);
    }
    const roadIds = ['aetherfall-city', 'moonwood-hunt', 'sunmeadow-hunt', 'lunaria-city', 'ember-highlands-hunt', 'mourning-marsh-hunt', 'watch-ruins-hunt', 'glimmergrove-hunt', 'dragonkeep-hunt', 'crystal-coast-hunt'];
    const roadPoints = roadIds.map((id) => destinationsById.get(id)).filter(Boolean).map((destination) => globalMapPosition(destination.xKm, destination.yKm));
    const roads = `<path class="overworld-route-network" d="M${roadPoints.map((point) => `${point.x.toFixed(1)} ${point.y.toFixed(1)}`).join(' L ')}"></path>`;
    const markers = overworldMap.destinations.map((destination) => {
      const point = globalMapPosition(destination.xKm, destination.yKm);
      const city = destination.kind === 'city';
      const labelAnchor = destination.xKm > 70 ? 'end' : 'start';
      const labelX = (point.x + (labelAnchor === 'end' ? -13 : 13)).toFixed(1);
      const labelY = (point.y - (city ? 9 : 7)).toFixed(1);
      const detail = city ? 'CIDADE' : `NV. ${destination.levelMin}–${destination.levelMax}`;
      return `<g class="overworld-marker overworld-marker--${city ? 'city' : 'hunt'} ${state.selectedWorldDestinationId === destination.id ? 'is-selected' : ''}" data-world-destination-id="${escapeHtml(destination.id)}" transform="translate(${point.x.toFixed(1)} ${point.y.toFixed(1)})" tabindex="0" role="button" aria-label="${escapeHtml(destination.name)} · ${destination.xKm}, ${destination.yKm} km"><title>${escapeHtml(destination.name)} · ${destination.xKm}, ${destination.yKm} km · ${escapeHtml(detail)}</title><circle class="overworld-marker-halo" r="${city ? 22 : 17}"></circle><circle class="overworld-marker-disc" r="${city ? 13 : 10}"></circle><text class="overworld-marker-icon" y="${city ? 4.5 : 3.8}">${escapeHtml(destination.icon)}</text><text class="overworld-marker-label" x="${labelX}" y="${labelY}" text-anchor="${labelAnchor}">${escapeHtml(destination.name.toUpperCase())}</text><text class="overworld-marker-detail" x="${labelX}" y="${(Number(labelY) + 13).toFixed(1)}" text-anchor="${labelAnchor}">${escapeHtml(detail)} · ${destination.xKm},${destination.yKm} KM</text></g>`;
    }).join('');
    const player = globalMapPosition(state.worldLocation.xKm, state.worldLocation.yKm);
    return `<svg id="global-world-map-svg" viewBox="0 0 1000 1000" preserveAspectRatio="xMidYMid meet" role="application" aria-label="Mapa mundial de 100 por 100 quilômetros. Selecione uma cidade ou área de caça e use Teleportar."><defs><linearGradient id="overworld-paper-gradient" x1="0" y1="0" x2="1" y2="1"><stop offset="0" stop-color="#51624a"></stop><stop offset=".5" stop-color="#334c40"></stop><stop offset="1" stop-color="#203a3c"></stop></linearGradient><radialGradient id="overworld-vignette"><stop offset=".45" stop-color="#0b1719" stop-opacity="0"></stop><stop offset="1" stop-color="#091315" stop-opacity=".54"></stop></radialGradient><pattern id="overworld-grain" width="34" height="34" patternUnits="userSpaceOnUse"><circle cx="4" cy="9" r=".8" fill="#e5d9ad" fill-opacity=".1"></circle><circle cx="25" cy="22" r=".7" fill="#101913" fill-opacity=".2"></circle></pattern></defs><rect class="overworld-paper" x="35" y="35" width="930" height="930" rx="10"></rect><rect class="overworld-shore" x="53" y="53" width="894" height="894" rx="30"></rect><g class="overworld-terrain-layer">${mapTerrain}</g><g class="overworld-topography" aria-hidden="true"><path d="M130 160 C240 95 370 105 445 190 S615 271 761 157 S884 142 930 189"></path><path d="M118 175 C228 110 358 120 433 205 S603 286 749 172 S872 157 918 204"></path><path d="M203 332 C316 259 423 288 475 368 S613 458 742 359 S866 329 921 389"></path><path d="M192 348 C305 275 412 304 464 384 S602 474 731 375 S855 345 910 405"></path><path d="M103 692 C224 622 375 637 452 721 S631 808 771 698 S878 689 927 749"></path><path d="M91 710 C212 640 363 655 440 739 S619 826 759 716 S866 707 915 767"></path></g><g class="overworld-grid">${gridLines.join('')}</g><g class="overworld-road-layer" aria-hidden="true">${roads}</g><g id="overworld-destination-layer">${markers}</g><path id="global-world-route" class="global-world-route" d=""></path><g id="global-world-waypoint" class="global-world-waypoint"><circle r="18"></circle><path d="M0 -11 L8 -3 L0 5 L-8 -3 Z"></path><text y="26">MARCO</text></g><g id="global-world-player" class="global-world-player" transform="translate(${player.x.toFixed(1)} ${player.y.toFixed(1)})"><circle class="overworld-player-pulse" r="19"></circle><circle class="overworld-player-core" r="7"></circle><path d="M0 -10 L-4 -18 L4 -18 Z"></path><text x="0" y="31">AELWYN</text></g><text class="overworld-axis-label" x="500" y="987">LESTE · 0—100 KM</text><text class="overworld-axis-label" transform="translate(18 500) rotate(-90)">NORTE · 0—100 KM</text><text class="overworld-map-stamp" x="922" y="82" text-anchor="end">AOA · OVERWORLD 01</text></svg>`;
  }

  function buildWorldMapContent() {
    const scale = regionMap.coordinateScale;
    const objective = getCurrentMapObjective();
    const zones = regionMap.zones.map((zone) => {
      const label = mapSvgPosition(zone.label[0], zone.label[1]);
      const zoneClass = `map-zone map-zone--${zone.tone}${zone.planned ? ' is-planned' : ''}`;
      return `<g class="${zoneClass}"><polygon points="${zone.polygon.map(([x, y]) => { const p = mapSvgPosition(x, y); return `${p.x.toFixed(1)},${p.y.toFixed(1)}`; }).join(' ')}"></polygon><text class="map-zone-label" x="${label.x.toFixed(1)}" y="${label.y.toFixed(1)}">${escapeHtml(zone.shortName)}</text></g>`;
    }).join('');
    const paths = regionMap.paths.map((path) => `<path class="map-road map-road--${escapeHtml(path.kind)}" d="${mapSvgPolyline(path.points)}"></path>`).join('');
    const treesOnMap = regionMap.woodlands.map(({ x, y, scale: treeScale }, index) => {
      const p = mapSvgPosition(x, y);
      const r = (3.7 + treeScale * 1.8).toFixed(1);
      return `<g class="map-tree" transform="translate(${p.x.toFixed(1)} ${p.y.toFixed(1)}) scale(${(0.7 + treeScale * 0.22).toFixed(2)})"><circle r="${r}"></circle><path d="M0 -5.5 L-4.4 2 H-1.7 L-4 5.4 H4 L1.7 2 H4.4 Z"></path></g>`;
    }).join('');
    const rocksOnMap = regionMap.rocks.map(({ x, y, scale: rockScale }) => {
      const p = mapSvgPosition(x, y);
      return `<path class="map-rock" d="M${(p.x - 5 * rockScale).toFixed(1)} ${(p.y + 2).toFixed(1)} l${(2.8 * rockScale).toFixed(1)} ${( -5 * rockScale).toFixed(1)} l${(3.7 * rockScale).toFixed(1)} ${( -2.5 * rockScale).toFixed(1)} l${(4.2 * rockScale).toFixed(1)} ${(5.5 * rockScale).toFixed(1)} Z"></path>`;
    }).join('');
    const mapMarkers = regionMap.points.filter((point) => point.mapMarker).map((point) => buildMapMarker(point, objective)).join('');
    const mapPointList = regionMap.points.filter((point) => point.mapList).map(buildMapPointRow).join('');
    const worldDestinationList = overworldMap.destinations.map(buildWorldDestinationRow).join('');
    const globalWorldSvg = buildOverworldSvg();
    const selectedDestination = destinationsById.get(state.selectedWorldDestinationId);
    const cellWidth = (regionMap.bounds.maxX - regionMap.bounds.minX) / 3;
    const cellHeight = (regionMap.bounds.maxY - regionMap.bounds.minY) / 4;
    const cellLayer = regionMap.cells.map((cell, index) => {
      const a = mapSvgPosition(regionMap.bounds.minX + cell.col * cellWidth, regionMap.bounds.minY + cell.row * cellHeight);
      return `<g class="map-cell" data-map-cell-id="${escapeHtml(cell.id)}"><rect x="${a.x.toFixed(1)}" y="${a.y.toFixed(1)}" width="${(cellWidth * scale.x).toFixed(1)}" height="${(cellHeight * scale.y).toFixed(1)}"></rect><text class="map-cell-number" x="${(a.x + 9).toFixed(1)}" y="${(a.y + 15).toFixed(1)}">${String(index + 1).padStart(2, '0')}</text><text class="map-cell-label" x="${(a.x + 9).toFixed(1)}" y="${(a.y + 29).toFixed(1)}">${escapeHtml(cell.role)}</text></g>`;
    }).join('');
    const initial = mapSvgPosition(state.player.x, state.player.y);
    const objectiveCopy = objective ? objective.description : 'Todas as missões disponíveis foram concluídas.';
    const pointCount = regionMap.points.filter((point) => point.mapList).length;

    return `<div class="world-map-intro"><div><p class="eyebrow">${escapeHtml(overworldMap.mapId)} · ${overworldMap.boundsKm.maxX} × ${overworldMap.boundsKm.maxY} KM</p><p class="world-map-subtitle">Mundo aberto de 100 km × 100 km, com cidades extensas, áreas de caça e teletransporte funcional.</p></div><span class="map-runtime-badge"><i></i> CARTOGRAFIA JOGÁVEL · PRÉVIA WEB</span></div>
      <div class="world-map-toolbar"><div class="map-mode-switch" role="group" aria-label="Camadas do mapa"><button type="button" class="selected" data-map-mode="world">MUNDO · 100 KM</button><button type="button" data-map-mode="region">REGIÃO AETHERFALL</button><button type="button" data-map-mode="cells">CÉLULAS LÓGICAS <span>${regionMap.cells.length}</span></button></div><div class="map-toolbar-coords"><span>COORDENADAS</span><strong id="world-map-coordinates">${state.worldLocation.xKm.toFixed(1)}, ${state.worldLocation.yKm.toFixed(1)} km</strong><span class="map-toolbar-separator"></span><span>N ↑</span></div></div>
      <div class="world-map-layout"><div class="world-map-art-wrap"><div class="world-map-art" aria-label="Mapa mundial de 100 por 100 quilômetros">
        ${globalWorldSvg}<svg id="world-map-svg" viewBox="0 0 ${scale.viewWidth} ${scale.viewHeight}" preserveAspectRatio="none" role="application" aria-label="Mapa regional detalhado de Aetherfall.">
          <defs><linearGradient id="map-paper-gradient" x1="0" y1="0" x2="1" y2="1"><stop offset="0" stop-color="#586348"></stop><stop offset=".44" stop-color="#374c3d"></stop><stop offset="1" stop-color="#243c39"></stop></linearGradient><radialGradient id="map-vignette-gradient"><stop offset=".45" stop-color="#0b1719" stop-opacity="0"></stop><stop offset="1" stop-color="#091315" stop-opacity=".48"></stop></radialGradient><pattern id="map-grain-pattern" width="31" height="31" patternUnits="userSpaceOnUse"><circle cx="3" cy="8" r=".7" fill="#e1d8ad" fill-opacity=".12"></circle><circle cx="21" cy="18" r=".55" fill="#111a14" fill-opacity=".18"></circle><path d="M0 29L12 26" stroke="#d1c99c" stroke-opacity=".035"></path></pattern></defs>
          <rect class="map-paper" width="${scale.viewWidth}" height="${scale.viewHeight}" rx="8"></rect>
          <rect class="map-paper-grain" width="${scale.viewWidth}" height="${scale.viewHeight}" rx="8"></rect><rect class="map-paper-vignette" width="${scale.viewWidth}" height="${scale.viewHeight}" rx="8"></rect>
          <g class="map-topography" aria-hidden="true"><path d="M-10 119 C120 55 212 120 309 86 S502 57 603 102 S793 149 920 78"></path><path d="M-10 132 C120 68 212 133 309 99 S502 70 603 115 S793 162 920 91"></path><path d="M-10 145 C120 81 212 146 309 112 S502 83 603 128 S793 175 920 104"></path><path d="M10 390 C109 327 221 351 307 398 S477 462 564 413 S746 360 908 412"></path><path d="M10 405 C109 342 221 366 307 413 S477 477 564 428 S746 375 908 427"></path><path d="M10 420 C109 357 221 381 307 428 S477 492 564 443 S746 390 908 442"></path></g>
          <g class="map-zone-layer">${zones}</g>
          <path class="map-river-under" d="${mapSvgPolyline(regionMap.waterway)}"></path><path class="map-river" d="${mapSvgPolyline(regionMap.waterway)}"></path>
          <g class="map-woodland-layer" aria-hidden="true">${treesOnMap}${rocksOnMap}</g>
          <g class="map-road-layer" aria-hidden="true">${paths}</g>
          <g class="map-cell-layer">${cellLayer}</g>
          <path id="world-map-route" class="world-map-route" d=""></path>
          <g id="world-map-point-layer">${mapMarkers}</g>
          <g id="world-map-waypoint" class="world-map-waypoint" transform="translate(0 0)"><circle class="world-waypoint-pulse" r="18"></circle><path d="M0 -11 L8 -3 L0 5 L-8 -3 Z"></path><text id="world-map-waypoint-label" y="23">ROTA</text></g>
          <g id="world-map-player" class="world-map-player" transform="translate(${initial.x.toFixed(1)} ${initial.y.toFixed(1)})" role="img" aria-label="Posição atual de Aelwyn"><title>Aelwyn · posição atual</title><circle class="world-player-pulse" r="15"></circle><circle class="world-player-core" r="5.5"></circle><path class="world-player-heading" d="M0 -8 L-3 -14 L3 -14 Z"></path></g>
          <path class="map-frame-corner" d="M14 39V14H39 M861 14H886V39 M14 521V546H39 M861 546H886V521"></path>
        </svg>
        <div class="map-north-rose" aria-hidden="true"><span>N</span><i>▲</i><b>·</b></div><div class="map-art-scale"><span></span><small id="map-art-scale-label">10 KM</small></div><div class="map-art-stamp">AOA · 100×100 KM</div>
      </div><div class="map-art-caption"><span><i class="map-caption-dot"></i> CIDADES</span><span>ROTAS REGIONAIS</span><span><i class="map-caption-frontier"></i> CAÇA · 10 MOBs</span><span><i class="map-caption-ruins"></i> RESPAWN · 3s</span></div></div>
      <aside class="world-map-sidebar"><section class="map-sidebar-card map-location-card"><p class="eyebrow">VOCÊ ESTÁ EM</p><h3 id="map-current-place">${escapeHtml($('#zone-title').textContent)}</h3><div class="map-location-facts"><span>COORDENADAS <strong id="map-player-coordinates">${Math.floor(state.player.x)}, ${Math.floor(state.player.y)}</strong></span><span>CÉLULA LÓGICA <strong id="map-active-cell">${escapeHtml(currentMapCell().role)}</strong></span></div><div class="map-cell-caption" id="map-cell-description">${escapeHtml(currentMapCell().label)} · unidade de autoria, sem streaming simulado.</div><div class="map-zone-policy"><i></i><span id="map-zone-policy">${escapeHtml($('#zone-state').textContent)}</span></div></section>
        <section class="map-sidebar-card map-objective-card"><div class="map-card-heading"><p class="eyebrow">PRÓXIMO OBJETIVO · MISSÃO OPCIONAL</p><span class="map-objective-glyph">✧</span></div><h3 id="map-objective-name">${escapeHtml(objective ? objective.name : 'Jornada concluída')}</h3><p id="map-objective-description">${escapeHtml(objectiveCopy)}</p><button id="map-objective-button" class="map-action-button" type="button" ${objective ? '' : 'disabled'}><span>MARCAR OBJETIVO</span><b>↗</b></button></section>
        <section class="map-sidebar-card map-teleport-card"><div class="map-card-heading"><p class="eyebrow">DESTINO DE TELEPORTE</p><span class="map-objective-glyph">✦</span></div><h3 id="map-teleport-name">${escapeHtml(selectedDestination?.name || 'Selecione um destino')}</h3><p id="map-teleport-description">${escapeHtml(selectedDestination?.description || 'Escolha uma cidade ou área de caça no mapa. Cada área de farm contém dez mobs.')}</p><button id="map-teleport-button" class="map-action-button" type="button" ${selectedDestination ? '' : 'disabled'}><span>TELEPORTAR · ${escapeHtml(selectedDestination?.xKm ?? '—')}, ${escapeHtml(selectedDestination?.yKm ?? '—')} KM</span><b>✧</b></button></section>
        <section class="map-sidebar-card map-points-card"><div class="map-card-heading"><p id="map-points-heading" class="eyebrow">DESTINOS · TELEPORTE</p><span id="map-points-count" class="map-points-count">${overworldMap.destinations.length}</span></div><div id="world-destination-list" class="world-map-points-list">${worldDestinationList}</div><div id="map-points-list" class="world-map-points-list">${mapPointList}</div></section>
      </aside></div>
      <div class="world-map-footer"><div id="map-action-status" class="map-action-status" role="status" aria-live="polite">${selectedDestination ? `Destino pronto · ${escapeHtml(selectedDestination.name)} · teleporte ativo.` : 'Selecione uma cidade ou área de caça.'}</div><div class="world-map-footer-note"><span>100 × 100 KM</span><i></i><span>${overworldMap.destinations.length} DESTINOS</span><i></i><span>10 MOBs · RESPAWN 3s</span></div><button id="map-clear-waypoint" class="map-clear-button" type="button" ${state.worldWaypoint ? '' : 'disabled'}>LIMPAR ROTA</button></div>`;
  }

  function updateWorldMapDialog() {
    const root = $('#modal-root');
    const svg = root.querySelector('#world-map-svg');
    if (!svg) return;
    const playerPosition = mapSvgPosition(state.player.x, state.player.y);
    svg.querySelector('#world-map-player')?.setAttribute('transform', `translate(${playerPosition.x.toFixed(1)} ${playerPosition.y.toFixed(1)})`);
    const objective = getCurrentMapObjective();
    const cell = currentMapCell();
    const localCoordinates = `${Math.floor(state.player.x)}, ${Math.floor(state.player.y)}`;
    root.querySelector('#map-active-cell').textContent = cell.role;
    root.querySelector('#map-cell-description').textContent = `${cell.label} · unidade lógica de autoria; mapa regional de ${regionMap.bounds.maxX} × ${regionMap.bounds.maxY} unidades.`;
    root.querySelector('#map-objective-name').textContent = objective ? objective.name : 'Jornada opcional concluída';
    root.querySelector('#map-objective-description').textContent = objective ? objective.description : 'As missões são conteúdo de jornada e não concedem XP.';
    root.querySelector('#map-objective-button').disabled = !objective;
    svg.querySelectorAll('.map-cell').forEach((element) => element.classList.toggle('is-active', element.dataset.mapCellId === cell.id));
    const route = svg.querySelector('#world-map-route');
    const waypointGroup = svg.querySelector('#world-map-waypoint');
    if (state.waypoint) {
      const target = mapSvgPosition(state.waypoint.x, state.waypoint.y);
      route.setAttribute('d', `M ${playerPosition.x.toFixed(1)} ${playerPosition.y.toFixed(1)} L ${target.x.toFixed(1)} ${target.y.toFixed(1)}`);
      route.classList.add('is-visible');
      waypointGroup.setAttribute('transform', `translate(${target.x.toFixed(1)} ${target.y.toFixed(1)})`);
      waypointGroup.classList.add('is-visible');
    } else {
      route.classList.remove('is-visible');
      waypointGroup.classList.remove('is-visible');
    }
    root.querySelectorAll('[data-map-point-id]').forEach((element) => {
      const point = pointsById.get(element.dataset.mapPointId);
      if (!point) return;
      const availability = getMapPointAvailability(point);
      element.classList.toggle('is-objective', Boolean(objective && objective.id === point.id));
      if (element.classList.contains('map-marker')) element.setAttribute('aria-label', `${point.name} — ${availability.label}`);
    });
    updateOverworldMapDialog();
    const modal = root.querySelector('.world-map-modal');
    const status = root.querySelector('#map-action-status');
    if (status && modal?.dataset.mapMode !== 'world') {
      status.textContent = state.waypoint
        ? `Rota regional · ${state.waypoint.name} · ${distance(state.player, state.waypoint).toFixed(1)} unidades.`
        : `${currentMapCell().role} · mapa regional detalhado de Aetherfall.`;
    }
  }

  function updateOverworldMapDialog() {
    const root = $('#modal-root');
    const svg = root.querySelector('#global-world-map-svg');
    if (!svg) return;
    const location = state.worldLocation;
    const position = globalMapPosition(location.xKm, location.yKm);
    svg.querySelector('#global-world-player')?.setAttribute('transform', `translate(${position.x.toFixed(1)} ${position.y.toFixed(1)})`);
    const destination = destinationsById.get(state.currentDestinationId) || initialDestination;
    const selected = destinationsById.get(state.selectedWorldDestinationId);
    const coords = `${location.xKm.toFixed(1)}, ${location.yKm.toFixed(1)} km`;
    const place = root.querySelector('#map-current-place');
    if (place) place.textContent = destination.name;
    const coordinates = root.querySelector('#world-map-coordinates');
    if (coordinates) coordinates.textContent = coords;
    const playerCoordinates = root.querySelector('#map-player-coordinates');
    if (playerCoordinates) playerCoordinates.textContent = coords;
    const policy = root.querySelector('#map-zone-policy');
    if (policy) policy.textContent = destination.safe ? 'CIDADE · ZONA SEGURA' : `FARM · ${state.mobs.filter((mob) => mob.alive).length}/10 MOBs · RESPAWN 3s`;
    const teleportName = root.querySelector('#map-teleport-name');
    const teleportDescription = root.querySelector('#map-teleport-description');
    const teleportButton = root.querySelector('#map-teleport-button');
    if (teleportName) teleportName.textContent = selected?.name || 'Selecione um destino';
    if (teleportDescription) teleportDescription.textContent = selected
      ? `${selected.description} · Coordenadas ${selected.xKm}, ${selected.yKm} km.`
      : 'Escolha uma cidade ou área de caça no mapa.';
    if (teleportButton) {
      teleportButton.disabled = !selected;
      teleportButton.innerHTML = `<span>TELEPORTAR · ${selected ? `${selected.xKm}, ${selected.yKm} KM` : '—'}</span><b>✧</b>`;
    }
    root.querySelectorAll('[data-world-destination-id]').forEach((element) => {
      const isSelected = element.dataset.worldDestinationId === state.selectedWorldDestinationId;
      element.classList.toggle('is-selected', isSelected);
      if (element.classList.contains('overworld-marker')) element.setAttribute('aria-pressed', String(isSelected));
      if (element.classList.contains('world-destination-point')) {
        const stateLabel = element.querySelector('.world-map-point-state');
        if (stateLabel) stateLabel.textContent = isSelected ? 'SELEC.' : destinationsById.get(element.dataset.worldDestinationId)?.kind === 'city' ? 'CIDADE' : 'TELEPORTAR';
      }
    });
    const route = svg.querySelector('#global-world-route');
    const waypoint = svg.querySelector('#global-world-waypoint');
    if (state.worldWaypoint) {
      const target = globalMapPosition(state.worldWaypoint.xKm, state.worldWaypoint.yKm);
      route.setAttribute('d', `M ${position.x.toFixed(1)} ${position.y.toFixed(1)} L ${target.x.toFixed(1)} ${target.y.toFixed(1)}`);
      route.classList.add('is-visible');
      waypoint.setAttribute('transform', `translate(${target.x.toFixed(1)} ${target.y.toFixed(1)})`);
      waypoint.classList.add('is-visible');
    } else {
      route.classList.remove('is-visible');
      waypoint.classList.remove('is-visible');
    }
    const clearButton = root.querySelector('#map-clear-waypoint');
    if (clearButton) clearButton.disabled = !state.worldWaypoint && !state.waypoint;
    const modal = root.querySelector('.world-map-modal');
    const status = root.querySelector('#map-action-status');
    if (status && modal?.dataset.mapMode === 'world') {
      if (selected) {
        const distanceKm = Math.hypot(selected.xKm - location.xKm, selected.yKm - location.yKm).toFixed(1);
        status.textContent = state.worldWaypoint
          ? `Rota · ${state.worldWaypoint.name} · ${Math.hypot(state.worldWaypoint.xKm - location.xKm, state.worldWaypoint.yKm - location.yKm).toFixed(1)} km · teleporte disponível para destinos.`
          : `Destino pronto · ${selected.name} · ${distanceKm} km · clique em TELEPORTAR para viajar.`;
      } else status.textContent = 'Selecione uma cidade ou área de caça para ver o teleporte.';
    }
  }

  function setWorldWaypoint(xKm, yKm, name) {
    state.worldWaypoint = {
      xKm: clamp(xKm, overworldMap.boundsKm.minX, overworldMap.boundsKm.maxX),
      yKm: clamp(yKm, overworldMap.boundsKm.minY, overworldMap.boundsKm.maxY),
      name: name || 'Marco no mundo',
    };
    updateOverworldMapDialog();
  }

  function selectWorldDestination(destinationId) {
    const destination = destinationsById.get(destinationId);
    if (!destination) return;
    state.selectedWorldDestinationId = destination.id;
    state.worldWaypoint = { xKm: destination.xKm, yKm: destination.yKm, name: destination.name };
    updateOverworldMapDialog();
  }

  function teleportToSelectedDestination() {
    const destination = destinationsById.get(state.selectedWorldDestinationId);
    if (!destination) { addToast('Selecione um destino de teleporte.'); return; }
    if (state.player.gold < destination.teleportCost) { addToast('Ouro insuficiente para o teleporte.', 'red'); return; }
    const previousDestinationId = state.currentDestinationId;
    state.itemDropsByDestination[previousDestinationId] = state.itemDrops;
    state.player.gold -= destination.teleportCost;
    state.currentDestinationId = destination.id;
    state.player.x = destination.localX;
    state.player.y = destination.localY;
    state.camera.x = destination.localX;
    state.camera.y = destination.localY;
    state.localOrigin = { x: destination.localX, y: destination.localY, xKm: destination.xKm, yKm: destination.yKm };
    state.worldLocation = { xKm: destination.xKm, yKm: destination.yKm };
    state.worldWaypoint = null;
    state.pet.x = destination.localX - .75;
    state.pet.y = destination.localY + .65;
    state.moveTarget = null;
    state.target = null;
    state.waypoint = null;
    state.projectiles = [];
    state.itemDrops = state.itemDropsByDestination[destination.id] || [];
    spawnMobSpot(destination);
    updateMiniMapRoute();
    $('#world-tip-text').textContent = destination.spotId
      ? `Área de caça ${destination.name}: 10 mobs ativos, XP por abate e respawn individual em 3 segundos.`
      : `${destination.name}: cidade segura com distrito comercial, guilda e serviços.`;
    addJournal(`Teleporte · ${destination.name}`, `${destination.xKm}, ${destination.yKm} km · ${destination.spotId ? '10 mobs no local' : 'zona segura'}.`, '✧');
    closeModal();
    lastUiUpdate = 0;
    updateUi(performance.now() + 100);
    draw();
    addToast(`TELEPORTE · ${destination.name.toUpperCase()} · ${destination.xKm}, ${destination.yKm} KM`, 'gold');
    playTone(720, .16, 'sine');
  }

  function updateMiniMapRoute() {
    const marker = $('#minimap-waypoint');
    const line = $('#minimap-route-line');
    if (!marker || !line) return;
    if (!state.waypoint) {
      marker.classList.add('hidden');
      line.style.display = 'none';
      return;
    }
    const percent = (point) => ({ x: 15 + (point.x / MAX_WORLD) * 72, y: 10 + (point.y / MAX_WORLD) * 76 });
    const playerPoint = percent(state.player);
    const targetPoint = percent(state.waypoint);
    marker.classList.remove('hidden');
    marker.style.left = `${targetPoint.x}%`;
    marker.style.top = `${targetPoint.y}%`;
    marker.title = `Rota marcada: ${state.waypoint.name}`;
    line.setAttribute('x1', playerPoint.x.toFixed(2));
    line.setAttribute('y1', playerPoint.y.toFixed(2));
    line.setAttribute('x2', targetPoint.x.toFixed(2));
    line.setAttribute('y2', targetPoint.y.toFixed(2));
    line.style.display = 'block';
  }

  function setMapWaypoint(x, y, name) {
    state.waypoint = {
      x: clamp(x, regionMap.bounds.minX + .2, regionMap.bounds.maxX - .2),
      y: clamp(y, regionMap.bounds.minY + .2, regionMap.bounds.maxY - .2),
      name: name || 'Marco de orientação',
    };
    updateMiniMapRoute();
    updateWorldMapDialog();
    addToast(`ROTA MARCADA · ${state.waypoint.name}`);
  }

  function selectMapPoint(pointId) {
    const point = pointsById.get(pointId);
    if (!point) return;
    const availability = getMapPointAvailability(point);
    if (!availability.available) {
      const status = $('#map-action-status');
      if (status) status.textContent = `${point.name}: ${availability.label.toLowerCase()}.`;
      return;
    }
    const snapshot = getMapPointSnapshot(point);
    setMapWaypoint(snapshot.x, snapshot.y, snapshot.name);
  }

  function setWorldMapMode(mode) {
    const root = $('#modal-root');
    const modal = root.querySelector('.world-map-modal');
    const layer = root.querySelector('.map-cell-layer');
    if (!modal || !layer) return;
    const safeMode = ['world', 'region', 'cells'].includes(mode) ? mode : 'world';
    modal.dataset.mapMode = safeMode;
    layer.classList.toggle('is-visible', safeMode === 'cells');
    root.querySelectorAll('button[data-map-mode]').forEach((button) => button.classList.toggle('selected', button.dataset.mapMode === safeMode));
    const heading = root.querySelector('#map-points-heading');
    const count = root.querySelector('#map-points-count');
    if (heading) heading.textContent = safeMode === 'world' ? 'DESTINOS · TELEPORTE' : 'PONTOS DE INTERESSE';
    if (count) count.textContent = safeMode === 'world' ? overworldMap.destinations.length : regionMap.points.filter((point) => point.mapList).length;
    const scaleLabel = root.querySelector('#map-art-scale-label');
    if (scaleLabel) scaleLabel.textContent = safeMode === 'world' ? '10 KM' : '4 UNIDADES';
    const status = root.querySelector('#map-action-status');
    if (status) status.textContent = safeMode === 'world'
      ? `Mundo aberto · ${overworldMap.boundsKm.maxX} × ${overworldMap.boundsKm.maxY} km · selecione um destino e teleporte.`
      : `${currentMapCell().role} · detalhe regional${safeMode === 'cells' ? ' · células lógicas de autoria' : ''}.`;
  }

  function closeModal() {
    const root = $('#modal-root');
    if (worldMapRootHandlers) {
      root.removeEventListener('click', worldMapRootHandlers.click);
      root.removeEventListener('keydown', worldMapRootHandlers.keydown);
      worldMapRootHandlers = null;
    }
    root.innerHTML = '';
    const onClose = modalCloseCallback;
    modalCloseCallback = null;
    if (onClose) onClose();
  }

  function openModal(title, content, options = {}) {
    const root = $('#modal-root');
    if (root.firstElementChild) closeModal();
    const dialogClass = options.dialogClass || '';
    const backdropClass = options.backdropClass || '';
    const eyebrow = options.eyebrow || 'AETHERFALL · AELWYN';
    modalCloseCallback = options.onClose || null;
    root.innerHTML = `<div class="modal-backdrop ${backdropClass}" role="presentation"><section class="modal-card ${dialogClass}" role="dialog" aria-modal="true" aria-label="${escapeHtml(title)}"><button class="modal-close" type="button" aria-label="Fechar">×</button><p class="eyebrow">${escapeHtml(eyebrow)}</p><h2>${escapeHtml(title)}</h2><div class="modal-content">${content}</div></section></div>`;
    root.querySelector('.modal-close').addEventListener('click', closeModal);
    root.querySelector('.modal-backdrop').addEventListener('click', (event) => { if (event.target.classList.contains('modal-backdrop')) closeModal(); });
  }

  function openWorldMap() {
    const root = $('#modal-root');
    if (root.querySelector('.world-map-modal')) { closeModal(); return; }
    const wasPaused = state.paused;
    state.paused = true;
    keys.clear();
    openModal('Mundo de Aetherfall · 100 × 100 km', buildWorldMapContent(), {
      dialogClass: 'world-map-modal',
      backdropClass: 'world-map-backdrop',
      eyebrow: 'OVERWORLD · MUNDO ABERTO',
      onClose: () => { state.paused = wasPaused; keys.clear(); },
    });
    const mapModal = root.querySelector('.world-map-modal');
    mapModal.dataset.mapMode = 'world';
    const svg = root.querySelector('#world-map-svg');
    svg.addEventListener('pointerdown', (event) => {
      if (event.button !== 0 || event.target.closest('[data-map-point-id]')) return;
      const rect = svg.getBoundingClientRect();
      const view = regionMap.coordinateScale;
      const x = ((event.clientX - rect.left) / rect.width) * view.viewWidth;
      const y = ((event.clientY - rect.top) / rect.height) * view.viewHeight;
      const worldX = regionMap.bounds.minX + (x - view.offsetX) / view.x;
      const worldY = regionMap.bounds.minY + (y - view.offsetY) / view.y;
      if (worldX < regionMap.bounds.minX || worldX > regionMap.bounds.maxX || worldY < regionMap.bounds.minY || worldY > regionMap.bounds.maxY) return;
      setMapWaypoint(worldX, worldY, 'Marco no terreno');
    });
    const globalSvg = root.querySelector('#global-world-map-svg');
    globalSvg.addEventListener('pointerdown', (event) => {
      if (event.button !== 0 || event.target.closest('[data-world-destination-id]')) return;
      const rect = globalSvg.getBoundingClientRect();
      const scale = Math.min(rect.width / 1000, rect.height / 1000);
      if (!scale) return;
      const offsetX = (rect.width - 1000 * scale) / 2;
      const offsetY = (rect.height - 1000 * scale) / 2;
      const x = (event.clientX - rect.left - offsetX) / scale;
      const y = (event.clientY - rect.top - offsetY) / scale;
      const xKm = (x - 70) / 8.6;
      const yKm = (y - 70) / 8.6;
      if (xKm < 0 || xKm > 100 || yKm < 0 || yKm > 100) return;
      setWorldWaypoint(xKm, yKm, 'Marco livre');
    });
    const handleMapClick = (event) => {
      const mode = event.target.closest('button[data-map-mode]');
      if (mode) { setWorldMapMode(mode.dataset.mapMode); return; }
      const worldDestination = event.target.closest('[data-world-destination-id]');
      if (worldDestination) { selectWorldDestination(worldDestination.dataset.worldDestinationId); return; }
      if (event.target.closest('#map-teleport-button')) { teleportToSelectedDestination(); return; }
      if (event.target.closest('#map-clear-waypoint')) {
        state.worldWaypoint = null;
        state.waypoint = null;
        updateMiniMapRoute();
        updateWorldMapDialog();
        return;
      }
      const point = event.target.closest('[data-map-point-id]');
      if (point) { selectMapPoint(point.dataset.mapPointId); return; }
      if (event.target.closest('#map-objective-button')) {
        const objective = getCurrentMapObjective();
        if (objective) setMapWaypoint(objective.x, objective.y, objective.name);
        return;
      }
    };
    const handleMapKeydown = (event) => {
      if ((event.key === 'Enter' || event.key === ' ') && event.target.matches('.overworld-marker[data-world-destination-id]')) {
        event.preventDefault(); event.stopPropagation(); selectWorldDestination(event.target.dataset.worldDestinationId); return;
      }
      if ((event.key === 'Enter' || event.key === ' ') && event.target.matches('.map-marker[data-map-point-id]')) {
        event.preventDefault(); event.stopPropagation(); selectMapPoint(event.target.dataset.mapPointId);
      }
    };
    root.addEventListener('click', handleMapClick);
    root.addEventListener('keydown', handleMapKeydown);
    worldMapRootHandlers = { click: handleMapClick, keydown: handleMapKeydown };
    updateWorldMapDialog();
  }

  function itemSlotName(slot) {
    return ({
      weapon: 'Arma', armor: 'Armadura', boots: 'Botas', cloak: 'Manto', ring: 'Anel',
      pet: 'Mascote', wings: 'Asas', consumable: 'Consumível', material: 'Material', quest: 'Missão',
    })[slot] || slot;
  }

  function itemStatsLabel(item) {
    const stats = item.stats || {};
    const labels = [];
    if (stats.attack) labels.push(`+${stats.attack} ATQ`);
    if (stats.hp) labels.push(`+${stats.hp} VIDA`);
    if (stats.mana) labels.push(`+${stats.mana} ÉTER`);
    if (stats.speed) labels.push(`+${stats.speed.toFixed(2)} VEL.`);
    if (stats.petDamage) labels.push(`+${stats.petDamage} DANO DO PET`);
    return labels.length ? labels.join(' · ') : item.description;
  }

  function toggleEquippedItem(itemId) {
    const item = itemCatalog[itemId];
    if (!item || !['weapon', 'armor', 'boots', 'cloak', 'ring', 'pet', 'wings'].includes(item.slot)) return;
    const current = state.player.equipped[item.slot];
    state.player.equipped[item.slot] = current === itemId ? null : itemId;
    recalculatePlayerStats(false);
    addToast(state.player.equipped[item.slot] ? `${item.name.toUpperCase()} EQUIPADO` : `${item.name.toUpperCase()} REMOVIDO`);
    lastUiUpdate = 0;
    updateUi(performance.now() + 100);
    showInventory();
  }

  function showInventory() {
    const items = state.player.inventory.map((entry) => {
      const item = itemCatalog[entry.id];
      if (!item) return '';
      const equipped = state.player.equipped[item.slot] === item.id;
      const canEquip = ['weapon', 'armor', 'boots', 'cloak', 'ring', 'pet', 'wings'].includes(item.slot);
      const canUse = item.id === 'health-potion';
      const action = canEquip
        ? `<button type="button" data-item-action="equip" data-item-id="${escapeHtml(item.id)}">${equipped ? 'REMOVER' : 'EQUIPAR'}</button>`
        : canUse ? `<button type="button" data-item-action="use" data-item-id="${escapeHtml(item.id)}">USAR</button>` : '';
      const statLabel = itemStatsLabel(item);
      return `<article class="inventory-item ${equipped ? 'is-equipped' : ''}"><span class="inventory-glyph ${escapeHtml(item.slot)}">${escapeHtml(item.icon)}</span><div><strong>${escapeHtml(item.name)}</strong><small>${escapeHtml(itemSlotName(item.slot))} · ${escapeHtml(item.rarity)} · ${escapeHtml(statLabel)}</small></div><div class="inventory-actions"><span class="inventory-slot-label">${equipped ? 'EQUIPADO' : `${entry.quantity} un.`}</span>${action}</div></article>`;
    }).join('');
    const empty = items ? '' : '<div class="inventory-empty">Os itens encontrados em áreas de caça aparecerão aqui.</div>';
    openModal('Inventário e equipamento', `<div class="inventory-grid">${items}${empty}</div><div class="inventory-foot"><span>OURO</span><strong>◉ ${state.player.gold}</strong><span>ESPAÇOS</span><strong>${inventorySlotCount()} / 24</strong></div>`);
    $('#modal-root').querySelectorAll('[data-item-action]').forEach((button) => {
      button.addEventListener('click', () => {
        if (button.dataset.itemAction === 'equip') toggleEquippedItem(button.dataset.itemId);
        if (button.dataset.itemAction === 'use') { usePotion(); showInventory(); }
      });
    });
  }

  function showCharacter() {
    const player = state.player;
    const equippedLine = (slot) => {
      const item = itemCatalog[player.equipped[slot]];
      return item ? item.name : '—';
    };
    openModal('Aventureira', `<div class="sheet-header"><img src="assets/player-mage.png" alt=""><div><strong>Aelwyn</strong><span>Maga Errante · Nível ${player.level}</span></div></div><div class="sheet-stats"><div><small>VIDA</small><strong>${Math.ceil(player.hp)} / ${player.maxHp}</strong></div><div><small>ÉTER</small><strong>${Math.floor(player.mana)} / ${player.maxMana}</strong></div><div><small>EXPERIÊNCIA DE FARM</small><strong>${player.xp} / ${xpToNextLevel()} XP</strong></div><div><small>ATAQUE</small><strong>${14 + player.level * 2 + getEquippedStat('attack')}</strong></div><div><small>VELOCIDADE</small><strong>${(3.15 + getEquippedStat('speed')).toFixed(2)}</strong></div><div><small>CAMINHO</small><strong>O Caminho do Arcano</strong></div></div><div class="character-equipment"><div><small>ARMA</small><strong>${escapeHtml(equippedLine('weapon'))}</strong></div><div><small>ARMADURA</small><strong>${escapeHtml(equippedLine('armor'))}</strong></div><div><small>BOTAS / MANTO</small><strong>${escapeHtml(equippedLine('boots'))} · ${escapeHtml(equippedLine('cloak'))}</strong></div><div><small>ANEL</small><strong>${escapeHtml(equippedLine('ring'))}</strong></div><div><small>MASCOTE</small><strong>${escapeHtml(equippedLine('pet'))}</strong></div><div><small>ASAS</small><strong>${escapeHtml(equippedLine('wings'))}</strong></div></div><p class="sheet-note">“O éter não escolhe quem o carrega. Só revela o que essa pessoa já era.”</p>`);
  }

  function initEvents() {
    $('#start-button').addEventListener('click', startGame);
    $('#revive-button').addEventListener('click', revive);
    $('#quest-action').addEventListener('click', () => {
      if (state.questStage < 4 || state.wispQuestStage < 4) addToast('MISSÃO ACOMPANHADA · O MARCADOR MOSTRA O PRÓXIMO PASSO');
      else addToast('O SELO DA VIGÍLIA FOI RESTAURADO');
    });
    $('#inventory-button').addEventListener('click', showInventory);
    $('#character-menu').addEventListener('click', showCharacter);
    $('#world-map-button').addEventListener('click', openWorldMap);
    $('#map-panel-open').addEventListener('click', openWorldMap);
    $('#potion-button').addEventListener('click', usePotion);
    $('#sound-toggle').addEventListener('click', (event) => {
      soundsEnabled = !soundsEnabled;
      event.currentTarget.classList.toggle('sound-on', soundsEnabled);
      event.currentTarget.setAttribute('aria-label', soundsEnabled ? 'Desativar sons' : 'Ativar sons');
      addToast(soundsEnabled ? 'EFEITOS DE ÁUDIO ATIVADOS' : 'EFEITOS DE ÁUDIO DESATIVADOS');
      if (soundsEnabled) playTone(600, .1, 'sine');
    });
    document.querySelectorAll('.skill-slot').forEach((button) => {
      button.addEventListener('click', () => {
        document.querySelectorAll('.skill-slot').forEach((slot) => slot.classList.remove('selected'));
        button.classList.add('selected');
        const skill = button.dataset.skill;
        if (skill === 'attack') useBasicAttack();
        if (skill === 'bolt') useArcaneBolt();
        if (skill === 'heal') useHeal();
      });
    });
    canvas.addEventListener('pointerdown', handlePointerDown);
    canvas.addEventListener('contextmenu', (event) => event.preventDefault());
    window.addEventListener('keydown', (event) => {
      const key = event.key.toLowerCase();
      if (key === 'm' && !event.repeat) { event.preventDefault(); openWorldMap(); return; }
      if (key === 'escape') { event.preventDefault(); closeModal(); return; }
      if (['w','a','s','d','arrowup','arrowleft','arrowdown','arrowright',' '].includes(key)) event.preventDefault();
      if (event.repeat) { keys.add(key); return; }
      keys.add(key);
      if (key === 'e') interact();
      if (key === '1') useBasicAttack();
      if (key === '2') useArcaneBolt();
      if (key === '3') useHeal();
      if (key === 'h') usePotion();
      if (key === 'p' && state.started && !state.dead && !$('#modal-root').querySelector('.world-map-modal')) {
        state.paused = !state.paused;
        keys.clear();
        addToast(state.paused ? 'JOGO PAUSADO · PRESSIONE P PARA CONTINUAR' : 'JORNADA RETOMADA');
      }
      if (key === 'enter' && !state.started && !$('#modal-root').firstElementChild) startGame();
    });
    window.addEventListener('keyup', (event) => keys.delete(event.key.toLowerCase()));
    window.addEventListener('blur', () => keys.clear());
    window.addEventListener('resize', resizeCanvas);
    document.addEventListener('visibilitychange', () => { if (document.hidden) keys.clear(); });
  }

  initEvents();
  updateQuestUI();
  updateJournal();
  resizeCanvas();
  requestAnimationFrame(frame);
})();
