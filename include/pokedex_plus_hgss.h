#ifndef GUARD_POKEDEX_PLUS_HGSS_H
#define GUARD_POKEDEX_PLUS_HGSS_H
extern void (*gPokedexVBlankCB)(void);

void CB2_OpenPokedexPlusHGSS(void);
void Task_DisplayCaughtMonDexPageHGSS(u8);

void ResetPokedex(void);
u16 GetDexCount(u8 caseID);
u8 DisplayCaughtMonDexPage(u16 species);
u32 Pokedex_CreateCaughtMonSprite(u32 species, s32 x, s32 y);
s8 GetSetPokedexFlag(enum DexOrder dexNum, u8 caseID);
void DrawFootprint(u8 windowId, u16 species);
void ResetPokedexScrollPositions(void);
bool16 HasAllMons(void);
u16 CreateMonSpriteFromDexNumberHGSS(u16 num, s16 x, s16 y, u16 paletteSlot);
void PrintMonMeasurements(u16 species, u32 owned);
u8* ConvertMonHeightToString(u32 height);
u8* ConvertMonWeightToString(u32 weight);

#endif // GUARD_POKEDEX_PLUS_HGSS_H
