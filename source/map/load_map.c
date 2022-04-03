#include "source/configuration/system_constants.h"
#include "source/neslib_asm/neslib.h"
#include "source/library/bank_helpers.h"
#include "source/map/map.h"
#include "source/globals.h"
#include "source/map/map.h"
#include "source/sprites/player.h"

unsigned char palette[16];

ZEROPAGE_DEF(unsigned char, lastCounterSprite);

// TODO: If we really never change this, hardcode it.
const unsigned char groundWaterLevels[64] = {
    3, 4, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 6, 2, 3, 3, 5,
    2, 3, 10, 4, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10
};

// Replace any "sprite-ish" things, so that when we move them they don't duplicate
void update_map_replace_spriteish(void) {
    if (tempChar1 == TILE_COLLISION_COLLECTABLE) {
        currentMapOrig[j] = 0;
        ++totalCollectableCount;
    } else if (tempChar1 == TILE_COLLISION_CRATE || tempChar1 == TILE_COLLISION_LOCK || tempChar1 == TILE_COLLISION_KEY) {
        currentMapOrig[j] = 0;
    } else if (tempChar1 == TILE_COLLISION_GAP) {
        currentMapOrig[j] = 0;
        ++totalCrateCount;
    } else {
        currentMapOrig[j] = currentMap[j];
    }

    if (currentMap[j] == GRATE_TILE) {
        oam_spr(36 + ((j % 12)<<4), 58 + ((j  / 12) << 4), (0x60 + '0' + maxWaterLevel), 0, lastCounterSprite<<2);
        ++lastCounterSprite;
    }
}

// Loads the map at the player's current position into the ram variable for the map. 
void load_map() {
    totalCollectableCount = 0;
    totalCrateCount = 0;
    lastCounterSprite = 8; // Skip sprite0 and player

    // Each map is 64 bytes in total, so find the index to start looking at
    tempInt1 = currentLevelId << 6;

    // Pull data out of the data we have available (see patchable_data.asm for where this comes from)
    currentMapBorderTile = gameLevelData[60 + tempInt1];
    currentMapBorderAsset = tilePalettes[currentMapBorderTile];
    currentMapBorderAsset += (currentMapBorderAsset << 2);
    currentMapBorderAsset += (currentMapBorderAsset << 4);
    if (currentMapBorderTile < 8) {
        currentMapBorderTile <<= 1;
    } else {
        currentMapBorderTile -= 8;
        currentMapBorderTile <<= 1;
        currentMapBorderTile += 32;
    }
    maxWaterLevel = groundWaterLevels[currentLevelId];


    // Iterate over the map data and expand it into a full map. Each byte in the data we store actually holds
    // 2 tiles - each nybble (half-byte) is a tile id, left then right. Thus each row is 6 bytes. If you're editing
    // in assembly, the left tile is 0xN_ and the right is 0x_N.
    for (i = 0, j = 0; i != 60; ++i) {
        j = i<<1;
        currentMap[j] = (gameLevelData[i + tempInt1] & 0xf0) >> 4;
        // Optimization: 1 so we can subtract the extra 1 for crates and not overflow (This doesn't apply anymore but changing it would change a lot...)
        floodMap[j] = 1;

        tempChar1 = tileCollisionTypes[currentMap[j]];
        update_map_replace_spriteish();

        ++j;
        currentMap[j] = (gameLevelData[i + tempInt1] & 0x0f);
        floodMap[j] = 1;

        tempChar1 = tileCollisionTypes[currentMap[j]];
        update_map_replace_spriteish();
    }
    playerGridPositionX = gameLevelData[tempInt1 + 62] & 0x0f;
    playerGridPositionY = gameLevelData[tempInt1 + 62] >> 4;
    currentGameStyle = gameLevelData[tempInt1 + 61];
}