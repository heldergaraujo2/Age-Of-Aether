const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const vm = require('node:vm');

const dataFile = path.resolve(__dirname, '../../Preview/world-map-data.js');
const context = { window: {} };
vm.runInNewContext(fs.readFileSync(dataFile, 'utf8'), context, { filename: dataFile });
const map = context.window.AETHER_FIRST_REGION_MAP;
const overworld = context.window.AETHER_OVERWORLD_MAP;
const items = context.window.AETHER_ITEMS;
assert.ok(map, 'map presentation data is exported');
assert.ok(overworld, '100 km overworld travel data is exported');
assert.ok(items, 'inventory item catalog is exported');
assert.equal(map.mapId, 'AOA.Region.FirstPermanent');
assert.equal(map.unrealMaterialized, false, 'browser data must not claim to be an Unreal map');

const expectedZones = [
  'AOA.Region.FirstPermanent.Settlement',
  'AOA.Region.FirstPermanent.Outskirts',
  'AOA.Region.FirstPermanent.Countryside',
  'AOA.Region.FirstPermanent.ForestEdge',
  'AOA.Region.FirstPermanent.Wilderness',
  'AOA.Region.FirstPermanent.CombatFrontier',
  'AOA.Region.FirstPermanent.Ruins',
  'AOA.Region.FirstPermanent.DungeonApproach',
  'AOA.Region.FirstPermanent.DungeonEntrance',
];
assert.deepEqual(Array.from(map.zones, (zone) => zone.id).sort(), expectedZones.sort());
assert.equal(new Set(Array.from(map.zones, (zone) => zone.id)).size, 9, 'zone IDs are unique');

const expectedCellRoles = [
  'Arrival', 'Settlement-West', 'Settlement-Center', 'Settlement-East',
  'Outskirts', 'Main-Road', 'Countryside', 'Forest-Edge',
  'Wilderness-North', 'Wilderness-South', 'Combat-Frontier', 'Ruins-Dungeon-Approach',
];
assert.equal(map.cells.length, 12, 'the logical streaming-cell grammar has twelve cells');
assert.deepEqual(Array.from(map.cells, (cell) => cell.role).sort(), expectedCellRoles.sort());
assert.equal(new Set(map.cells.map((cell) => `${cell.col}:${cell.row}`)).size, 12, 'each logical cell occupies a unique preview grid slot');
assert.ok(map.cells.every((cell) => cell.col >= 0 && cell.col < 3 && cell.row >= 0 && cell.row < 4));

const zoneIds = new Set(map.zones.map((zone) => zone.id));
const southernWildernessTrail = map.points.find((point) => point.id === 'wilderness-south');
assert.equal(southernWildernessTrail.zoneId, 'AOA.Region.FirstPermanent.Wilderness', 'wilderness-south remains a point, not a noncanonical zone');
function pointInPolygon(x, y, polygon) {
  let inside = false;
  for (let i = 0, j = polygon.length - 1; i < polygon.length; j = i, i += 1) {
    const [xi, yi] = polygon[i];
    const [xj, yj] = polygon[j];
    const crossesRay = (yi > y) !== (yj > y);
    if (crossesRay && x < ((xj - xi) * (y - yi)) / (yj - yi) + xi) inside = !inside;
  }
  return inside;
}

const pointIds = new Set();
for (const point of map.points) {
  assert.ok(!pointIds.has(point.id), `duplicate point ID: ${point.id}`);
  pointIds.add(point.id);
  assert.ok(point.x >= map.bounds.minX && point.x <= map.bounds.maxX, `${point.id} x is in map bounds`);
  assert.ok(point.y >= map.bounds.minY && point.y <= map.bounds.maxY, `${point.id} y is in map bounds`);
  assert.ok(zoneIds.has(point.zoneId), `${point.id} references a known zone`);
  const zone = map.zones.find((candidate) => candidate.id === point.zoneId);
  assert.ok(pointInPolygon(point.x, point.y, zone.polygon), `${point.id} lies inside ${point.zoneId}`);
}

for (const zone of map.zones) {
  assert.ok(zone.polygon.length >= 3, `${zone.id} has a drawable boundary`);
  assert.ok(zone.label[0] >= map.bounds.minX && zone.label[0] <= map.bounds.maxX, `${zone.id} label x is bounded`);
  assert.ok(zone.label[1] >= map.bounds.minY && zone.label[1] <= map.bounds.maxY, `${zone.id} label y is bounded`);
  assert.ok(pointInPolygon(zone.label[0], zone.label[1], zone.polygon), `${zone.id} label lies inside its zone`);
  for (const [x, y] of zone.polygon) {
    assert.ok(x >= map.bounds.minX && x <= map.bounds.maxX, `${zone.id} polygon x is bounded`);
    assert.ok(y >= map.bounds.minY && y <= map.bounds.maxY, `${zone.id} polygon y is bounded`);
  }
}
for (const [x, y] of map.waterway) {
  assert.ok(x >= map.bounds.minX && x <= map.bounds.maxX, 'waterway x is bounded');
  assert.ok(y >= map.bounds.minY && y <= map.bounds.maxY, 'waterway y is bounded');
}
for (const scenery of [...map.woodlands, ...map.rocks]) {
  assert.ok(scenery.x >= map.bounds.minX && scenery.x <= map.bounds.maxX, 'scenery x is bounded');
  assert.ok(scenery.y >= map.bounds.minY && scenery.y <= map.bounds.maxY, 'scenery y is bounded');
}
for (const road of map.paths) {
  assert.ok(road.points.length >= 2, `${road.id} has a route`);
  for (const [x, y] of road.points) {
    assert.ok(x >= map.bounds.minX && x <= map.bounds.maxX, `${road.id} x is bounded`);
    assert.ok(y >= map.bounds.minY && y <= map.bounds.maxY, `${road.id} y is bounded`);
  }
}

const mainRoad = map.paths.find((road) => road.id === 'main-road');
assert.ok(mainRoad, 'the region has a primary route');
assert.ok(mainRoad.points.some(([x, y]) => x === 9 && y === 9), 'the main road passes through the settlement gate');
assert.ok(mainRoad.points.some(([x, y]) => x === 16 && y === 15.7), 'the main road reaches the Vigília landmark');
assert.ok(map.points.some((point) => point.id === 'ruins-landmark' && point.status === 'playable'));
assert.ok(map.points.some((point) => point.id === 'dungeon-entrance' && point.status === 'planned'));
for (const id of ['elite-ground', 'boss-arena', 'pvp-event-ground', 'expansion-trailhead']) {
  assert.ok(map.points.some((point) => point.id === id && point.status === 'planned'), `${id} is represented as planned roadmap content`);
}

assert.equal(overworld.mapId, 'AOA.Overworld.Aetherfall100');
assert.deepEqual({ ...overworld.boundsKm }, { minX: 0, minY: 0, maxX: 100, maxY: 100 }, 'global preview map spans exactly 100 km by 100 km');
assert.equal(overworld.gridStepKm, 10, 'world map has a ten-kilometer scale grid');
assert.equal(overworld.destinations.filter((destination) => destination.kind === 'city').length, 2, 'the overworld includes two city destinations');
const huntingSpots = overworld.destinations.filter((destination) => destination.spotId);
assert.equal(huntingSpots.length, 8, 'eight distinct farming spots are teleport destinations');
const destinationIds = new Set();
for (const destination of overworld.destinations) {
  assert.ok(!destinationIds.has(destination.id), `duplicate overworld destination: ${destination.id}`);
  destinationIds.add(destination.id);
  assert.ok(destination.xKm >= 0 && destination.xKm <= 100, `${destination.id} longitude is within the 100 km world`);
  assert.ok(destination.yKm >= 0 && destination.yKm <= 100, `${destination.id} latitude is within the 100 km world`);
  assert.ok(destination.localX >= 0 && destination.localX <= map.bounds.maxX, `${destination.id} local teleport x is bounded`);
  assert.ok(destination.localY >= 0 && destination.localY <= map.bounds.maxY, `${destination.id} local teleport y is bounded`);
  assert.equal(destination.teleportCost, 0, `${destination.id} can be reached in this preview`);
  if (destination.spotId) {
    assert.ok(destination.levelMin <= destination.levelMax, `${destination.spotId} has an ordered level range`);
    assert.ok(Array.isArray(destination.loot) && destination.loot.length > 0, `${destination.spotId} has a loot table`);
    for (const itemId of destination.loot) assert.ok(items[itemId], `${destination.spotId} loot item ${itemId} exists in the inventory catalog`);
    assert.equal(destination.mobCount, 10, `${destination.spotId} maintains ten active mobs`);
    assert.equal(destination.respawnSeconds, 3, `${destination.spotId} respawns mobs individually after three seconds`);
  }
}
for (const itemId of ['apprentice-focus', 'ranger-robe', 'ember-fox', 'wind-wings', 'health-potion', 'moonstone-ring']) {
  assert.ok(items[itemId], `inventory item ${itemId} exists`);
}
assert.equal(items['ember-fox'].slot, 'pet', 'the pet is equipable');
assert.equal(items['wind-wings'].slot, 'wings', 'wings are equipable');
assert.equal(items['apprentice-focus'].slot, 'weapon', 'weapons are equipment slots');

console.log('Map contracts passed: Aetherfall region (9 zones, 12 cells) + 100 × 100 km overworld (2 cities, 8 ten-mob farms).');
