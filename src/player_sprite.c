#include "global.h"
#include "player_sprite.h"
#include "constants/event_objects.h"
#include "constants/trainers.h"
#include "battle.h"
#include "battle_controllers.h"
#include "link.h"

const u8 sPlayerFrontSpriteIds[GENDER_COUNT] = {
    [MALE] = TRAINER_PIC_BRENDAN,
    [FEMALE] = TRAINER_PIC_MAY,
};
const u8 sPlayerBackSpriteIds[GENDER_COUNT] = {
    [MALE] = TRAINER_BACK_PIC_BRENDAN,
    [FEMALE] = TRAINER_BACK_PIC_MAY,
};
const u8 sPlayerOverworldSpriteIds[][GENDER_COUNT] = {
    [PLAYER_AVATAR_STATE_NORMAL] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_NORMAL,
        [FEMALE] = OBJ_EVENT_GFX_MAY_NORMAL,
    },
    [PLAYER_AVATAR_STATE_MACH_BIKE] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_MACH_BIKE,
        [FEMALE] = OBJ_EVENT_GFX_MAY_MACH_BIKE,
    },
    [PLAYER_AVATAR_STATE_ACRO_BIKE] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_ACRO_BIKE,
        [FEMALE] = OBJ_EVENT_GFX_MAY_ACRO_BIKE,
    },
    [PLAYER_AVATAR_STATE_SURFING] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_SURFING,
        [FEMALE] = OBJ_EVENT_GFX_MAY_SURFING,
    },
    [PLAYER_AVATAR_STATE_UNDERWATER] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_UNDERWATER,
        [FEMALE] = OBJ_EVENT_GFX_MAY_UNDERWATER,
    },
    [PLAYER_AVATAR_STATE_FIELD_MOVE] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_FIELD_MOVE,
        [FEMALE] = OBJ_EVENT_GFX_MAY_FIELD_MOVE,
    },
    [PLAYER_AVATAR_STATE_FISHING] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_FISHING,
        [FEMALE] = OBJ_EVENT_GFX_MAY_FISHING,
    },
    [PLAYER_AVATAR_STATE_WATERING] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_WATERING,
        [FEMALE] = OBJ_EVENT_GFX_MAY_WATERING,
    },
    [PLAYER_AVATAR_STATE_VSSEEKER] = {
        [MALE] = OBJ_EVENT_GFX_BRENDAN_FIELD_MOVE,
        [FEMALE] = OBJ_EVENT_GFX_MAY_FIELD_MOVE,
    },
};

// Returns the trainer front sprite ID for the player.
u16 GetPlayerFrontSpriteId()
{
    return sPlayerFrontSpriteIds[gSaveBlock2Ptr->playerGender];
}

// This function exists solely for Link Battles, which are planned to be removed
// in the future.
u16 GetPlayerFrontSpriteId_Compat(enum Gender gender)
{
    return sPlayerFrontSpriteIds[gender];
}

// Returns the trainer back sprite ID for the player.
u32 GetPlayerBackSpriteId()
{
    return !(gBattleTypeFlags & BATTLE_TYPE_LINK) ? sPlayerBackSpriteIds[gSaveBlock2Ptr->playerGender] : LinkPlayerGetTrainerPicId(GetMultiplayerId());
}

// Returns the appropriate overworld sprite ID for the player.
u16 GetPlayerOverworldSpriteId(enum PlayerAvatarState state)
{
    return sPlayerOverworldSpriteIds[state][gSaveBlock2Ptr->playerGender];
}
