#include "global.h"
#include "event_data.h"
#include "field_message_box.h"
#include "pokedex.h"
#include "strings.h"

bool16 ScriptGetPokedexInfo(void)
{
    gSpecialVar_0x8005 = GetNationalPokedexCount(FLAG_GET_SEEN);
    gSpecialVar_0x8006 = GetNationalPokedexCount(FLAG_GET_CAUGHT);

    return IsNationalPokedexEnabled();
}

// TODO
const u8 *GetPokedexRatingText(u32 count)
{
    return gBirchDexRatingText_DexCompleted;
}

void ShowPokedexRatingMessage(void)
{
    ShowFieldMessage(GetPokedexRatingText(gSpecialVar_0x8004));
}
