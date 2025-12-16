#ifndef GUARD_PLAYER_SPRITE_H
#define GUARD_PLAYER_SPRITE_H

extern const u8 gPlayerFrontSpriteIds[GENDER_COUNT];
extern const u8 gPlayerBackSpriteIds[GENDER_COUNT];
extern const u8 gPlayerOverworldSpriteIds[][GENDER_COUNT];

u16 GetPlayerFrontSpriteId();
u16 GetPlayerFrontSpriteId_Compat(enum Gender gender);
u32 GetPlayerBackSpriteId();
u16 GetPlayerOverworldSpriteId(enum PlayerAvatarState state);

#endif //GUARD_PLAYER_SPRITE_H
