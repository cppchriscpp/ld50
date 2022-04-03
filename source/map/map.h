#include "source/library/bank_helpers.h"

// How many tiles are in the map before we start getting into sprite data.
#define MAP_DATA_TILE_LENGTH 192

// The current map; usable for collisions/etc
extern unsigned char currentMap[120];
extern unsigned char currentMapOrig[120];

extern unsigned char assetTable[64];

ZEROPAGE_EXTERN(unsigned char, currentGameStyle);


// Set some default variables and hardware settings to prepare to draw the map after showing menus/etc.
void init_map();

// Assuming the map is available in currentMap, draw it to a nametable. Assumes ppu is already turned off.
void draw_current_map_to_a_inline();


// Handles collisions of all sorts in this engine.
#define TILE_COLLISION_WALKABLE 0
#define TILE_COLLISION_SOLID 1
#define TILE_COLLISION_HOLE 2
#define TILE_COLLISION_GAP 3
#define TILE_COLLISION_CRATE 4
#define TILE_COLLISION_COLLECTABLE 5
#define TILE_COLLISION_UNUSED 6
#define TILE_COLLISION_LEVEL_END 7
#define TILE_COLLISION_LOCK 8
#define TILE_COLLISION_KEY 9
#define TILE_COLLISION_ICE 0xa

extern const unsigned char gameName[];
extern const unsigned char tileCollisionTypes[];
extern const unsigned char tilePalettes[];

extern const unsigned char gameLevelData[];
extern const unsigned char totalGameLevels;
extern const unsigned char singleLevelOverride;

// Simplest style - just get to the end of each level.
#define GAME_STYLE_MAZE 0
#define GAME_STYLE_CRATES 1
#define GAME_STYLE_COIN 2
#define GAME_STYLE_COUNT 3

// Track the water height throughout the level
extern unsigned char floodMap[120];
ZEROPAGE_EXTERN(unsigned char, waterLevel);
ZEROPAGE_EXTERN(unsigned char, maxWaterLevel);

// New unique tiles for the water mechanic
#define WATER_TILE 15
#define GRATE_TILE 9
#define CAT_TILE 8

void flood_map(void);
void update_flooded_tiles(void);

ZEROPAGE_EXTERN(unsigned char, lastCounterSprite);

void update_water_levels(void);