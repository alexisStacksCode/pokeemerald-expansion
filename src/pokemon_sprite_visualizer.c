//Credits: Gamer2020, AsparagusEduardo, TheXaman, ShinyDragonHunter
#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_gfx_sfx_util.h"
#include "battle_environment.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_object_movement.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_icon.h"
#include "list_menu.h"
#include "m4a.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "overworld.h"
#include "palette.h"
#include "palette_util.h"
#include "pokedex_plus_hgss.h"
#include "pokemon.h"
#include "pokemon_sprite_visualizer.h"
#include "pokemon_icon.h"
#include "reset_rtc_screen.h"
#include "scanline_effect.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "trainer_pokemon_sprites.h"

#include "constants/global.h"
#include "constants/items.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"
#include "constants/songs.h"

extern const struct BattleEnvironment gBattleEnvironmentInfo[BATTLE_ENVIRONMENT_COUNT];
extern const struct CompressedSpriteSheet gSpriteSheet_EnemyShadow;
extern const struct CompressedSpriteSheet gSpriteSheet_EnemyShadowsSized;
extern const struct SpriteTemplate gSpriteTemplate_EnemyShadow;
extern const struct SpritePalette sSpritePalettes_HealthBoxHealthBar[2];
extern const struct UCoords8 sBattlerCoords[][MAX_BATTLERS_COUNT] ;
static const u16 sBgColor[] = {RGB_WHITE};

static struct PokemonSpriteVisualizer *GetStructPtr(u8 taskId)
{
    u8 *taskDataPtr = (u8 *)(&gTasks[taskId].data[0]);

    return (struct PokemonSpriteVisualizer*)(T1_READ_PTR(taskDataPtr));
}

static const union AnimCmd sAnim_Follower_1[] =
{
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_FRAME(1, 30),
    ANIMCMD_FRAME(0, 30),
    ANIMCMD_FRAME(1, 30),
    ANIMCMD_FRAME(0, 10),
    ANIMCMD_FRAME(2, 30),
    ANIMCMD_FRAME(3, 30),
    ANIMCMD_FRAME(2, 30),
    ANIMCMD_FRAME(3, 30),
    ANIMCMD_FRAME(2, 10),
    ANIMCMD_FRAME(4, 30),
    ANIMCMD_FRAME(5, 30),
    ANIMCMD_FRAME(4, 30),
    ANIMCMD_FRAME(5, 30),
    ANIMCMD_FRAME(4, 10),
    ANIMCMD_FRAME(4, 30, .hFlip = TRUE),
    ANIMCMD_FRAME(5, 30, .hFlip = TRUE),
    ANIMCMD_FRAME(4, 30, .hFlip = TRUE),
    ANIMCMD_FRAME(5, 30, .hFlip = TRUE),
    ANIMCMD_FRAME(4, 10, .hFlip = TRUE),
    ANIMCMD_END,
};

static const union AnimCmd *const sAnims_Follower[] =
{
    sAnim_GeneralFrame0,
    sAnim_Follower_1,
};

//BgTemplates
static const struct BgTemplate sBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 24,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
   {
        .bg = 3,
        .charBaseIndex = 2,
        .mapBaseIndex = 26,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

//WindowTemplates
static const struct WindowTemplate sPokemonSpriteVisualizerWindowTemplate[] =
{
    [WIN_NAME_NUMBERS] = {
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 12,
        .width = 15,
        .height = 2,
        .paletteNum = 0xF,
        .baseBlock = 1
    },
    [WIN_INSTRUCTIONS] = {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 15,
        .height = 4,
        .paletteNum = 0xF,
        .baseBlock = 1 + 30
    },
    [WIN_BOTTOM_LEFT] = {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 14,
        .width = 6,
        .height = 6,
        .paletteNum = 0xF,
        .baseBlock = 1 + 30 + 60
    },
    [WIN_BOTTOM_RIGHT] = {
        .bg = 0,
        .tilemapLeft = 7,
        .tilemapTop = 14,
        .width = 24,
        .height = 6,
        .paletteNum = 0xF,
        .baseBlock = 1 + 30 + 60 + 36
    },
    [WIN_FOOTPRINT] =
    {
        .bg = 0,
        .tilemapLeft = 27,
        .tilemapTop = 14,
        .width = 2,
        .height = 2,
        .paletteNum = 0xF,
        .baseBlock = 1 + 30 + 60 + 36 + 144,
    },
    DUMMY_WIN_TEMPLATE,
};

const u8 gBattleBackgroundNames[][30] =
{
    [MAP_BATTLE_SCENE_NORMAL]   = _("NORMAL                  "),
    [MAP_BATTLE_SCENE_GYM]      = _("GYM                     "),
    [MAP_BATTLE_SCENE_MAGMA]    = _("MAGMA                   "),
    [MAP_BATTLE_SCENE_AQUA]     = _("AQUA                    "),
    [MAP_BATTLE_SCENE_SIDNEY]   = _("SIDNEY                  "),
    [MAP_BATTLE_SCENE_PHOEBE]   = _("PHOEBE                  "),
    [MAP_BATTLE_SCENE_GLACIA]   = _("GLACIA                  "),
    [MAP_BATTLE_SCENE_DRAKE]    = _("DRAKE                   "),
    [MAP_BATTLE_SCENE_FRONTIER] = _("FRONTIER                "),
    [MAP_BATTLE_SCENE_LEADER]   = _("LEADER                  "),
    [MAP_BATTLE_SCENE_WALLACE]  = _("WALLACE                 "),
    [MAP_BATTLE_SCENE_GROUDON]  = _("GROUDON                 "),
    [MAP_BATTLE_SCENE_KYOGRE]   = _("KYOGRE                  "),
    [MAP_BATTLE_SCENE_RAYQUAZA] = _("RAYQUAZA                "),
};

const u8 gBattleBackgroundTerrainNames[][26] =
{
    [BATTLE_ENVIRONMENT_GRASS]      = _("NORMAL - GRASS           "),
    [BATTLE_ENVIRONMENT_LONG_GRASS] = _("NORMAL - LONG GRASS      "),
    [BATTLE_ENVIRONMENT_SAND]       = _("NORMAL - SAND            "),
    [BATTLE_ENVIRONMENT_UNDERWATER] = _("NORMAL - UNDERWATER      "),
    [BATTLE_ENVIRONMENT_WATER]      = _("NORMAL - WATER           "),
    [BATTLE_ENVIRONMENT_POND]       = _("NORMAL - POND            "),
    [BATTLE_ENVIRONMENT_MOUNTAIN]   = _("NORMAL - MOUNTAIN        "),
    [BATTLE_ENVIRONMENT_CAVE]       = _("NORMAL - CAVE            "),
    [BATTLE_ENVIRONMENT_BUILDING]   = _("NORMAL - BUILDING        "),
    [BATTLE_ENVIRONMENT_PLAIN]      = _("NORMAL - PLAIN           "),
};

const u8 sShadowSizeLabels[][4] =
{
    [SHADOW_SIZE_S]                 = _(" S"),
    [SHADOW_SIZE_M]                 = _(" M"),
    [SHADOW_SIZE_L]                 = _(" L"),
    [SHADOW_SIZE_XL_BATTLE_ONLY]    = _(" XL"),
};

//Function declarations
static void PrintDigitChars(struct PokemonSpriteVisualizer *data);
static void SetUpModifyArrows(struct PokemonSpriteVisualizer *data);
static void UpdateBattlerValue(struct PokemonSpriteVisualizer *data);
static void ValueToCharDigits(u8 *charDigits, u32 newValue, u8 maxDigits);
static bool32 TryMoveDigit(struct PokemonSpriteVisualizerModifyArrows *modArrows, bool32 moveUp);
static void CB2_PokemonSpriteVisualizerRunner(void);
static void ResetBGs_PokemonSpriteVisualizer(u16);
static void HandleInput_PokemonSpriteVisualizer(u8);
static void ReloadPokemonSprites(struct PokemonSpriteVisualizer *data);
static void Exit_PokemonSpriteVisualizer(u8);

//Text handling functions
static void UNUSED PadString(const u8 *src, u8 *dst)
{
    u32 i;

    for (i = 0; i < 17 && src[i] != EOS; i++)
        dst[i] = src[i];

    for (; i < 17; i++)
        dst[i] = CHAR_SPACE;

    dst[i] = EOS;
}

static void PrintInstructionsOnWindow(struct PokemonSpriteVisualizer *data)
{
    u8 fontId = 0;
    u8 x = 2;
    u8 textInstructions[] = _("{START_BUTTON} Shiny\n{B_BUTTON} Exit  {A_BUTTON} Anims and BG$");
    u8 textInstructionsGender[] = _("{START_BUTTON} Shiny {SELECT_BUTTON} Gender\n{B_BUTTON} Exit  {A_BUTTON} Anims and BG$");
    u8 textInstructionsSubmenuOne[] = _("{START_BUTTON} Shiny\n{B_BUTTON} Back  {A_BUTTON} Sprite Coords$");
    u8 textInstructionsSubmenuOneGender[] = _("{START_BUTTON} Shiny {SELECT_BUTTON} Gender\n{B_BUTTON} Back  {A_BUTTON} Sprite Coords$");
#if B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE
    u8 textInstructionsSubmenuTwo[] = _("{START_BUTTON} Shiny\n{B_BUTTON} Back  {A_BUTTON} Shadow Coords$");
    u8 textInstructionsSubmenuTwoGender[] = _("{START_BUTTON} Shiny {SELECT_BUTTON} Gender\n{B_BUTTON} Back  {A_BUTTON} Shadow Coords$");
    u8 textInstructionsSubmenuThree[] = _("{START_BUTTON} Shiny\n{B_BUTTON} Back");
    u8 textInstructionsSubmenuThreeGender[] = _("{START_BUTTON} Shiny {SELECT_BUTTON} Gender\n{B_BUTTON} Back$");
#else
    u8 textInstructionsSubmenuTwo[] = _("{START_BUTTON} Shiny\n{B_BUTTON} Back$");
    u8 textInstructionsSubmenuTwoGender[] = _("{START_BUTTON} Shiny {SELECT_BUTTON} Gender\n{B_BUTTON} Back$");
    u8 textInstructionsSubmenuThree[] = _("$");
    u8 textInstructionsSubmenuThreeGender[] = _("$");
#endif


    u8 textBottom[] = _("BACK:\nFRONT:\nBG:$");
    u8 textBottomForms[] = _("BACK:\nFRONT:\nBG:\nFORMS:$");
    u8 textBottomSubmenuTwo[] = _("B coords:\nF coords:\nF elev:");
    u8 textBottomSubmenuThree[] = _("X coords:\nY coords:\nSize:");
    u16 species = data->modifyArrows.currValue;

    u8 textL[] = _("{L_BUTTON}");
    u8 textR[] = _("{R_BUTTON}");

    //Instruction window
    FillWindowPixelBuffer(WIN_INSTRUCTIONS, 0x11);
    if (data->currentSubmenu == 0)
    {
        if (SpeciesHasGenderDifferences(species))
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructionsGender, x, 0, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructions, x, 0, 0, NULL);
    }
    else if (data->currentSubmenu == 1)
    {
        if (SpeciesHasGenderDifferences(species))
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructionsSubmenuOneGender, x, 0, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructionsSubmenuOne, x, 0, 0, NULL);
    }
    else if (data->currentSubmenu == 2)
    {
        if (SpeciesHasGenderDifferences(species))
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructionsSubmenuTwoGender, x, 0, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructionsSubmenuTwo, x, 0, 0, NULL);
    }
    else if (data->currentSubmenu == 3)
    {
        if (SpeciesHasGenderDifferences(species))
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructionsSubmenuThreeGender, x, 0, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_INSTRUCTIONS, fontId, textInstructionsSubmenuThree, x, 0, 0, NULL);
    }
    CopyWindowToVram(WIN_INSTRUCTIONS, COPYWIN_FULL);

    //Bottom left text
    FillWindowPixelBuffer(WIN_BOTTOM_LEFT, PIXEL_FILL(0));
    if (data->currentSubmenu < 2)
    {
        AddTextPrinterParameterized(WIN_BOTTOM_LEFT, fontId, textL, 30, 0, 0, NULL);
        AddTextPrinterParameterized(WIN_BOTTOM_LEFT, fontId, textR, 30, 12, 0, NULL);
        if (GetSpeciesFormTable(data->currentmonId) != NULL)
            AddTextPrinterParameterized(WIN_BOTTOM_LEFT, fontId, textBottomForms, 0, 0, 0, NULL);
        else
            AddTextPrinterParameterized(WIN_BOTTOM_LEFT, fontId, textBottom, 0, 0, 0, NULL);
    }
    else if (data->currentSubmenu == 2)
        AddTextPrinterParameterized(WIN_BOTTOM_LEFT, fontId, textBottomSubmenuTwo, 0, 0, 0, NULL);
    else if (data->currentSubmenu == 3)
        AddTextPrinterParameterized(WIN_BOTTOM_LEFT, fontId, textBottomSubmenuThree, 0, 0, 0, NULL);
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void SetStructPtr(u8 taskId, void *ptr)
{
    u32 structPtr = (u32)(ptr);
    u8 *taskDataPtr = (u8 *)(&gTasks[taskId].data[0]);

    taskDataPtr[0] = structPtr >> 0;
    taskDataPtr[1] = structPtr >> 8;
    taskDataPtr[2] = structPtr >> 16;
    taskDataPtr[3] = structPtr >> 24;
}

//Digit and arrow functions
#define VAL_U16     0
static void PrintDigitChars(struct PokemonSpriteVisualizer *data)
{
    s32 i;
    u16 species = data->modifyArrows.currValue;
    u8 text[MODIFY_DIGITS_MAX + POKEMON_NAME_LENGTH + 8];

    for (i = 0; i < data->modifyArrows.maxDigits; i++)
        text[i] = data->modifyArrows.charDigits[i];

    text[i++] = CHAR_SPACE;
    text[i++] = CHAR_HYPHEN;

    if (SpeciesHasGenderDifferences(species))
    {
        if (data->isFemale)
            text[i++] = CHAR_FEMALE;
        else
            text[i++] = CHAR_MALE;
        text[i++] = CHAR_HYPHEN;
    }

    text[i++] = CHAR_SPACE;
    StringCopy(&text[i], GetSpeciesName(species));

    FillWindowPixelBuffer(WIN_NAME_NUMBERS, 0x11);
    AddTextPrinterParameterized(WIN_NAME_NUMBERS, FONT_NORMAL, text, 6, 0, 0, NULL);
}

static u32 CharDigitsToValue(u8 *charDigits, u8 maxDigits)
{
    s32 i;
    u8 id = 0;
    u32 newValue = 0;
    u8 valueDigits[MODIFY_DIGITS_MAX];

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        valueDigits[i] = charDigits[i] - CHAR_0;

    if (maxDigits >= MODIFY_DIGITS_MAX)
        newValue += valueDigits[id++] * 1000;
    if (maxDigits >= MODIFY_DIGITS_MAX - 1)
        newValue += valueDigits[id++] * 100;
    if (maxDigits >= MODIFY_DIGITS_MAX - 2)
        newValue += valueDigits[id++] * 10;
    if (maxDigits >= MODIFY_DIGITS_MAX - 3)
        newValue += valueDigits[id++];

    return newValue;
}

static void ValueToCharDigits(u8 *charDigits, u32 newValue, u8 maxDigits)
{
    s32 i;
    u8 valueDigits[MODIFY_DIGITS_MAX];
    u8 id = 0;

    if (maxDigits >= MODIFY_DIGITS_MAX)
        valueDigits[id++] = newValue / 1000;
    if (maxDigits >= MODIFY_DIGITS_MAX - 1)
        valueDigits[id++] = (newValue % 1000) / 100;
    if (maxDigits >= MODIFY_DIGITS_MAX - 2)
        valueDigits[id++] = (newValue % 100) / 10;
    if (maxDigits >= MODIFY_DIGITS_MAX - 3)
        valueDigits[id++] = newValue % 10;

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        charDigits[i] = valueDigits[i] + CHAR_0;
}

static void SetArrowInvisibility(struct PokemonSpriteVisualizer *data)
{
    switch (data->currentSubmenu)
    {
    case 0:
        gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = FALSE;
        gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = FALSE;
        gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
        gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = TRUE;
        break;
    case 1:
        gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = TRUE;
        gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = TRUE;
        gSprites[data->optionArrows.arrowSpriteId[0]].invisible = FALSE;
        gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = TRUE;
        break;
    case 2:
    case 3:
        gSprites[data->modifyArrows.arrowSpriteId[0]].invisible = TRUE;
        gSprites[data->modifyArrows.arrowSpriteId[1]].invisible = TRUE;
        gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
        gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = FALSE;
        break;
    }
}

static void SetUpModifyArrows(struct PokemonSpriteVisualizer *data)
{
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->modifyArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X, MODIFY_DIGITS_ARROW1_Y, 0);
    data->modifyArrows.arrowSpriteId[1] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X, MODIFY_DIGITS_ARROW2_Y, 0);
    gSprites[data->modifyArrows.arrowSpriteId[1]].animNum = 1;

    data->modifyArrows.minValue = 1;
    data->modifyArrows.maxValue = NUM_SPECIES - 1;
    data->modifyArrows.maxDigits = 4;
    data->modifyArrows.modifiedValPtr = &data->currentmonId;
    data->modifyArrows.typeOfVal = VAL_U16;
    data->modifyArrows.currValue = data->currentmonId;

    data->modifyArrows.currentDigit = 0;
    ValueToCharDigits(data->modifyArrows.charDigits, data->modifyArrows.currValue, data->modifyArrows.maxDigits);
}

static void SetUpOptionArrows(struct PokemonSpriteVisualizer *data)
{
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->optionArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, OPTIONS_ARROW_Y, 0);
    gSprites[data->optionArrows.arrowSpriteId[0]].animNum = 2;

    data->optionArrows.currentDigit = 0;

    gSprites[data->optionArrows.arrowSpriteId[0]].invisible = TRUE;
}

static void SetUpYPosModifyArrows(struct PokemonSpriteVisualizer *data)
{
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->yPosModifyArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, OPTIONS_ARROW_Y, 0);
    gSprites[data->yPosModifyArrows.arrowSpriteId[0]].animNum = 2;

    data->yPosModifyArrows.currentDigit = 0;

    gSprites[data->yPosModifyArrows.arrowSpriteId[0]].invisible = TRUE;
}

static bool32 TryMoveDigit(struct PokemonSpriteVisualizerModifyArrows *modArrows, bool32 moveUp)
{
    s32 i;
    u8 charDigits[MODIFY_DIGITS_MAX];
    u32 newValue;

    for (i = 0; i < MODIFY_DIGITS_MAX; i++)
        charDigits[i] = modArrows->charDigits[i];

    if (moveUp)
    {
        if (charDigits[modArrows->currentDigit] == CHAR_9)
        {
            charDigits[modArrows->currentDigit] = CHAR_0;
            for (i = modArrows->currentDigit - 1; i >= 0; i--)
            {
                if (charDigits[i] == CHAR_9)
                    charDigits[i] = CHAR_0;
                else
                {
                    charDigits[i]++;
                    break;
                }
            }
        }
        else
            charDigits[modArrows->currentDigit]++;
    }
    else
    {
        if (charDigits[modArrows->currentDigit] == CHAR_0)
        {
            charDigits[modArrows->currentDigit] = CHAR_9;

            for (i = modArrows->currentDigit - 1; i >= 0; i--)
            {
                if (charDigits[i] == CHAR_0)
                    charDigits[i] = CHAR_9;
                else
                {
                    charDigits[i]--;
                    break;
                }
            }
        }

        else
            charDigits[modArrows->currentDigit]--;
    }

    newValue = CharDigitsToValue(charDigits, modArrows->maxDigits);
    if (newValue > modArrows->maxValue || newValue < modArrows->minValue)
    {
        return FALSE;
    }
    else
    {
        modArrows->currValue = newValue;
        for (i = 0; i < MODIFY_DIGITS_MAX; i++)
             modArrows->charDigits[i] = charDigits[i];
        return TRUE;
    }
}

static void UpdateBattlerValue(struct PokemonSpriteVisualizer *data)
{
    switch (data->modifyArrows.typeOfVal)
    {
    case VAL_U16:
        *(u16*)(data->modifyArrows.modifiedValPtr) = data->modifyArrows.currValue;
        break;
    }
}

static void BattleLoadOpponentMonSpriteGfxCustom(u16 species, bool8 isFemale, bool8 isShiny, u8 battlerId)
{
    const u16 *palette = GetMonSpritePalFromSpecies(species, isShiny, isFemale);
    u16 paletteOffset = OBJ_PLTT_ID(battlerId);

    LoadPalette(palette, paletteOffset, PLTT_SIZE_4BPP);
    LoadPalette(palette, BG_PLTT_ID(8) + BG_PLTT_ID(battlerId), PLTT_SIZE_4BPP);
}

static void SetConstSpriteValues(struct PokemonSpriteVisualizer *data)
{
    u16 species = data->currentmonId;
    data->constSpriteValues.frontPicCoords = gSpeciesInfo[species].frontPicYOffset;
    data->constSpriteValues.frontElevation = gSpeciesInfo[species].enemyMonElevation;
    data->constSpriteValues.backPicCoords = gSpeciesInfo[species].backPicYOffset;
}

static void ResetOffsetSpriteValues(struct PokemonSpriteVisualizer *data)
{
    data->offsetsSpriteValues.offset_back_picCoords = 0;
    data->offsetsSpriteValues.offset_front_picCoords = 0;
    data->offsetsSpriteValues.offset_front_elevation = 0;
}

static void ResetShadowSettings(struct PokemonSpriteVisualizer *data, u16 species)
{
    if (B_ENEMY_MON_SHADOW_STYLE <= GEN_3 || P_GBA_STYLE_SPECIES_GFX == TRUE)
        return;

    data->shadowSettings.definedX = gSpeciesInfo[species].enemyShadowXOffset;
    data->shadowSettings.definedY = gSpeciesInfo[species].enemyShadowYOffset;
    data->shadowSettings.definedSize = gSpeciesInfo[species].enemyShadowSize;

    data->shadowSettings.overrideX = data->shadowSettings.definedX;
    data->shadowSettings.overrideY = data->shadowSettings.definedY;
    data->shadowSettings.overrideSize = data->shadowSettings.definedSize;
}

static u8 GetBattlerSpriteFinal_YCustom(u16 species, s8 offset_picCoords, s8 offset_elevation)
{
    u16 offset;
    u8 y;
    species = SanitizeSpeciesId(species);

    //FrontPicCoords
    offset = gSpeciesInfo[species].frontPicYOffset + offset_picCoords;

    //Elevation
    offset -= gSpeciesInfo[species].enemyMonElevation + offset_elevation;

    //Main position
    y = offset + sBattlerCoords[0][1].y;

    if (y > DISPLAY_HEIGHT - MON_PIC_HEIGHT + 8)
        y = DISPLAY_HEIGHT - MON_PIC_HEIGHT + 8;

    return y;
}

static void UpdateShadowSpriteInvisible(struct PokemonSpriteVisualizer *data)
{
    if (B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE)
        return;

    if (data->constSpriteValues.frontElevation + data->offsetsSpriteValues.offset_front_elevation == 0)
        gSprites[data->frontShadowSpriteIdPrimary].invisible = TRUE;
    else
        gSprites[data->frontShadowSpriteIdPrimary].invisible = FALSE;
}

#define tFrontSpriteId  data[0]
#define tSpriteSide     data[1]
#define tShadowXOffset  data[2]
#define tShadowYOffset  data[3]

#define SPRITE_SIDE_LEFT    0
#define SPRITE_SIDE_RIGHT   1


static void SpriteCB_EnemyShadowCustom(struct Sprite *shadowSprite)
{
    u8 frontSpriteId = shadowSprite->tFrontSpriteId;
    struct Sprite *battlerSprite = &gSprites[frontSpriteId];

    s8 xOffset = 0, yOffset = 0;
    if (B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE)
    {
        xOffset = shadowSprite->tShadowXOffset + (shadowSprite->tSpriteSide == SPRITE_SIDE_LEFT ? -16 : 16);
        yOffset = shadowSprite->tShadowYOffset + 16;

        shadowSprite->y = battlerSprite->y + yOffset;
    }

    shadowSprite->x = battlerSprite->x + xOffset;
    shadowSprite->x2 = battlerSprite->x2;
}

static void SpriteCB_Follower(struct Sprite *sprite)
{
    if (sprite->animDelayCounter == 0)
    {
        sprite->animDelayCounter = 60;
        switch (sprite->animNum)
        {
            default:
            case 0:
            case 1:
                StartSpriteAnim(sprite, GetFaceDirectionAnimNum(DIR_NORTH));
                break;
            case 2:
                StartSpriteAnim(sprite, GetFaceDirectionAnimNum(DIR_WEST));
                break;
            case 3:
                StartSpriteAnim(sprite, GetFaceDirectionAnimNum(DIR_EAST));
                break;
            case 4:
                StartSpriteAnim(sprite, GetFaceDirectionAnimNum(DIR_SOUTH));
                break;
        }
    }
    else
    {
        sprite->animDelayCounter--;
    }
}
static void LoadAndCreateEnemyShadowSpriteCustom(struct PokemonSpriteVisualizer *data, u16 species)
{
    bool8 invisible = FALSE;
    species = SanitizeSpeciesId(species);

    if (B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE)
    {
        invisible = gSpeciesInfo[species].suppressEnemyShadow;

        LoadCompressedSpriteSheet(&gSpriteSheet_EnemyShadowsSized);
        LoadSpritePalette(&sSpritePalettes_HealthBoxHealthBar[0]);
        u8 x = sBattlerCoords[0][1].x;
        u8 y = sBattlerCoords[0][1].y;
        s8 xOffset = data->shadowSettings.overrideX;
        s8 yOffset = data->shadowSettings.overrideY;
        u8 size = data->shadowSettings.overrideSize;

        data->frontShadowSpriteIdPrimary = CreateSprite(&gSpriteTemplate_EnemyShadow, x, y, 0xC8);
        gSprites[data->frontShadowSpriteIdPrimary].tFrontSpriteId = data->frontspriteId;
        gSprites[data->frontShadowSpriteIdPrimary].tSpriteSide = SPRITE_SIDE_LEFT;
        gSprites[data->frontShadowSpriteIdPrimary].tShadowXOffset = (u8)xOffset;
        gSprites[data->frontShadowSpriteIdPrimary].tShadowYOffset = (u8)yOffset;
        gSprites[data->frontShadowSpriteIdPrimary].callback = SpriteCB_EnemyShadowCustom;
        gSprites[data->frontShadowSpriteIdPrimary].oam.priority = 0;
        gSprites[data->frontShadowSpriteIdPrimary].oam.tileNum += 8 * size;
        gSprites[data->frontShadowSpriteIdPrimary].invisible = invisible;

        data->frontShadowSpriteIdSecondary = CreateSprite(&gSpriteTemplate_EnemyShadow, x, y, 0xC8);
        gSprites[data->frontShadowSpriteIdSecondary].tFrontSpriteId = data->frontspriteId;
        gSprites[data->frontShadowSpriteIdSecondary].tSpriteSide = SPRITE_SIDE_RIGHT;
        gSprites[data->frontShadowSpriteIdSecondary].tShadowXOffset = (u8)xOffset;
        gSprites[data->frontShadowSpriteIdSecondary].tShadowYOffset = (u8)yOffset;
        gSprites[data->frontShadowSpriteIdSecondary].callback = SpriteCB_EnemyShadowCustom;
        gSprites[data->frontShadowSpriteIdSecondary].oam.priority = 0;
        gSprites[data->frontShadowSpriteIdSecondary].oam.tileNum += (8 * size) + 4;
        gSprites[data->frontShadowSpriteIdSecondary].invisible = invisible;
    }
    else
    {
        if (gSpeciesInfo[species].enemyMonElevation == 0)
            invisible = TRUE;

        LoadCompressedSpriteSheet(&gSpriteSheet_EnemyShadow);
        LoadSpritePalette(&sSpritePalettes_HealthBoxHealthBar[0]);
        u8 x = sBattlerCoords[0][1].x;
        u8 y = sBattlerCoords[0][1].y;

        data->frontShadowSpriteIdPrimary = CreateSprite(&gSpriteTemplate_EnemyShadow, x, y + 29, 0xC8);
        gSprites[data->frontShadowSpriteIdPrimary].data[0] = data->frontspriteId;

        gSprites[data->frontShadowSpriteIdPrimary].callback = SpriteCB_EnemyShadowCustom;
        gSprites[data->frontShadowSpriteIdPrimary].oam.priority = 0;
        gSprites[data->frontShadowSpriteIdPrimary].invisible = invisible;
    }
}

//Battle background functions
static void LoadBattleBg(u8 battleBgType, enum BattleEnvironments battleEnvironment)
{
    switch (battleBgType)
    {
    default:
    case MAP_BATTLE_SCENE_NORMAL:
        DecompressDataWithHeaderVram(gBattleEnvironmentInfo[battleEnvironment].background.tileset, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentInfo[battleEnvironment].background.tilemap, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentInfo[battleEnvironment].background.palette, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_GYM:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Building, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Building, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_BuildingGym, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_MAGMA:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Stadium, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Stadium, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_StadiumMagma, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_AQUA:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Stadium, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Stadium, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_StadiumAqua, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_SIDNEY:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Stadium, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Stadium, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_StadiumSidney, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_PHOEBE:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Stadium, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Stadium, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_StadiumPhoebe, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_GLACIA:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Stadium, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Stadium, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_StadiumGlacia, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_DRAKE:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Stadium, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Stadium, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_StadiumDrake, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_FRONTIER:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Building, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Building, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_Frontier, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_LEADER:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Building, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Building, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_BuildingLeader, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_WALLACE:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Stadium, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Stadium, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_StadiumWallace, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_GROUDON:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Cave, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Cave, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_Groudon, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_KYOGRE:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Water, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Water, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_Kyogre, 0x20, 0x60);
        break;
    case MAP_BATTLE_SCENE_RAYQUAZA:
        DecompressDataWithHeaderVram(gBattleEnvironmentTiles_Rayquaza, (void*)(BG_CHAR_ADDR(2)));
        DecompressDataWithHeaderVram(gBattleEnvironmentTilemap_Rayquaza, (void*)(BG_SCREEN_ADDR(26)));
        LoadPalette(gBattleEnvironmentPalette_Rayquaza, 0x20, 0x60);
        break;
    }
}

static void PrintBattleBgName(u8 taskId)
{
    struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
    u8 fontId = 0;
    u8 text[30+1];

    if (data->battleBgType == 0)
        StringCopy(text, gBattleBackgroundTerrainNames[data->battleEnvironment]);
    else
        StringCopy(text, gBattleBackgroundNames[data->battleBgType]);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, 0, 24, 0, NULL);
}

static void UpdateBattleBg(u8 taskId, bool8 increment)
{
    struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);

    if (data->battleBgType == MAP_BATTLE_SCENE_NORMAL)
    {
        if (increment)
        {
            if (data->battleEnvironment == BATTLE_ENVIRONMENT_PLAIN)
                data->battleBgType += 1;
            else
                data->battleEnvironment += 1;
        }
        else
        {
            if (data->battleEnvironment == BATTLE_ENVIRONMENT_GRASS)
                data->battleBgType = MAP_BATTLE_SCENE_RAYQUAZA;
            else
                data->battleEnvironment -= 1;
        }
    }
    else if (data->battleBgType == MAP_BATTLE_SCENE_GYM)
    {
        if (increment)
            data->battleBgType += 1;
        else
        {
            data->battleBgType = MAP_BATTLE_SCENE_NORMAL;
            data->battleEnvironment = BATTLE_ENVIRONMENT_PLAIN;
        }
    }
    else if (data->battleBgType == MAP_BATTLE_SCENE_RAYQUAZA)
    {
        if (increment)
        {
            data->battleBgType = MAP_BATTLE_SCENE_NORMAL;
            data->battleEnvironment = BATTLE_ENVIRONMENT_GRASS;
        }
        else
            data->battleBgType -= 1;
    }
    else
    {
        if (increment)
            data->battleBgType += 1;
        else
            data->battleBgType -= 1;
    }

    PrintBattleBgName(taskId);

    LoadBattleBg(data->battleBgType, data->battleEnvironment);
}

// *******************************
// Main functions
static void UpdateYPosOffsetText(struct PokemonSpriteVisualizer *data)
{
    u8 text[34];
    u8 fontId = 0;
    u8 textConst[] = _("const val:");
    u8 textNew[] = _("new val:");
    u8 x_const_val = 50;
    u8 x_new_text = 70;
    u8 x_new_val = 110;
    u8 y = 0;

    u8 backPicCoords    = data->constSpriteValues.backPicCoords;
    u8 frontPicCoords   = data->constSpriteValues.frontPicCoords;
    u8 frontElevation   = data->constSpriteValues.frontElevation;

    s8 offset_back_picCoords    = data->offsetsSpriteValues.offset_back_picCoords;
    s8 offset_front_picCoords   = data->offsetsSpriteValues.offset_front_picCoords;
    s8 offset_front_elevation   = data->offsetsSpriteValues.offset_front_elevation;

    u8 newBackPicCoords    = backPicCoords   +  offset_back_picCoords;
    u8 newFrontPicCoords   = frontPicCoords  +  offset_front_picCoords;
    u8 newFrontElevation   = frontElevation  +  offset_front_elevation;

    FillWindowPixelBuffer(WIN_BOTTOM_RIGHT, PIXEL_FILL(0));

    //Back
    y = 0;
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textConst, 0, y, 0, NULL);
    ConvertIntToDecimalStringN(text, backPicCoords , STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_const_val, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textNew, x_new_text, y, 0, NULL);
    ConvertIntToDecimalStringN(text, newBackPicCoords , STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_new_val, y, 0, NULL);
    //Front picCoords
    y = 12;
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textConst, 0, y, 0, NULL);
    ConvertIntToDecimalStringN(text, frontPicCoords , STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_const_val, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textNew, x_new_text, y, 0, NULL);
    ConvertIntToDecimalStringN(text, newFrontPicCoords , STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_new_val, y, 0, NULL);
    //Front elevation
    y = 24;
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textConst, 0, y, 0, NULL);
    ConvertIntToDecimalStringN(text, frontElevation , STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_const_val, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textNew, x_new_text, y, 0, NULL);
    ConvertIntToDecimalStringN(text, newFrontElevation , STR_CONV_MODE_LEFT_ALIGN, 2);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_new_val, y, 0, NULL);
}

#define ABS(val)    (val < 0 ? val * -1 : val)
#define ITOA_SIGNED(buf, val)                                                       \
{                                                                                   \
    buf[0] = val < 0 ? CHAR_HYPHEN : CHAR_SPACER;                                   \
    ConvertIntToDecimalStringN(&text[1], ABS(val), STR_CONV_MODE_LEFT_ALIGN, 2);    \
}

static void UpdateShadowSettingsText(struct PokemonSpriteVisualizer *data)
{
    if (B_ENEMY_MON_SHADOW_STYLE <= GEN_3 || P_GBA_STYLE_SPECIES_GFX == TRUE)
        return;

    u8 text[16];
    u8 fontId = 0;
    u8 textConst[] = _("const val:");
    u8 textNew[] = _("new val:");
    u8 x_const_val = 50;
    u8 x_new_text = 70;
    u8 x_new_val = 110;
    u8 y = 0;

    FillWindowPixelBuffer(WIN_BOTTOM_RIGHT, PIXEL_FILL(0));

    // X offset
    y = 0;
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textConst, 0, y, 0, NULL);
    ITOA_SIGNED(text, data->shadowSettings.definedX);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_const_val, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textNew, x_new_text, y, 0, NULL);
    ITOA_SIGNED(text, data->shadowSettings.overrideX);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_new_val, y, 0, NULL);

    // Y offset
    y = 12;
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textConst, 0, y, 0, NULL);
    ITOA_SIGNED(text, data->shadowSettings.definedY);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_const_val, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textNew, x_new_text, y, 0, NULL);
    ITOA_SIGNED(text, data->shadowSettings.overrideY);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, text, x_new_val, y, 0, NULL);

    // Shadow Size
    y = 24;
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textConst, 0, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, sShadowSizeLabels[data->shadowSettings.definedSize], x_const_val, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, textNew, x_new_text, y, 0, NULL);
    AddTextPrinterParameterized(WIN_BOTTOM_RIGHT, fontId, sShadowSizeLabels[data->shadowSettings.overrideSize], x_new_val, y, 0, NULL);
}

static void ResetPokemonSpriteVisualizerWindows(void)
{
    u8 i;

    FreeAllWindowBuffers();
    InitWindows(sPokemonSpriteVisualizerWindowTemplate);

    for (i = 0; i < WIN_END + 1; i++)
    {
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
        PutWindowTilemap(i);
        CopyWindowToVram(i, COPYWIN_FULL);
    }
}

#define MALE_PERSONALITY 0xFE
#define FEMALE_PERSONALITY 0X0

void CB2_Pokemon_Sprite_Visualizer(void)
{
    u8 taskId;
    const u16 *palette;
    struct PokemonSpriteVisualizer *data;
    u16 species;
    s16 offset_y;
    u8 front_x = sBattlerCoords[0][1].x;
    u8 front_y;

    switch (gMain.state)
    {
        case 0:
        default:
            SetVBlankCallback(NULL);
            FreeMonSpritesGfx();
            ResetBGs_PokemonSpriteVisualizer(0);
            DmaFillLarge16(3, 0, (u8 *)VRAM, VRAM_SIZE, 0x1000)
            DmaClear32(3, OAM, OAM_SIZE);
            DmaClear16(3, PLTT, PLTT_SIZE);
            gMain.state = 1;
            break;
        case 1:
            ScanlineEffect_Stop();
            ResetTasks();
            ResetSpriteData();
            ResetPaletteFade();
            FreeAllSpritePalettes();
            gReservedSpritePaletteCount = 8;
            ResetAllPicSprites();
            BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
            LoadPalette(GetTextWindowPalette(0), 15*16, 0x40);

            FillBgTilemapBufferRect(0, 0, 0, 0, 32, 20, 15);
            InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
            LoadBattleBg(0, BATTLE_ENVIRONMENT_GRASS);

            gMain.state++;
            break;
        case 2:
            ResetPokemonSpriteVisualizerWindows();
            gMain.state++;
            break;
        case 3:
            AllocateMonSpritesGfx();

            LoadPalette(sBgColor, 0, 2);
            LoadMonIconPalette(SPECIES_BULBASAUR);

            SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
            ShowBg(0);
            ShowBg(1);
            ShowBg(2);
            ShowBg(3);

            //input task handler
            taskId = CreateTask(HandleInput_PokemonSpriteVisualizer, 0);

            data = AllocZeroed(sizeof(struct PokemonSpriteVisualizer));
            SetStructPtr(taskId, data);

            data->currentmonId = SPECIES_BULBASAUR;
            species = data->currentmonId;

            //Print instructions
            PrintInstructionsOnWindow(data);

            //Palettes
            palette = GetMonSpritePalFromSpecies(species, data->isShiny, data->isFemale);
            LoadSpritePaletteWithTag(palette, species);
            //Front
            HandleLoadSpecialPokePic(TRUE, gMonSpritesGfxPtr->spritesGfx[1], species, (data->isFemale ? FEMALE_PERSONALITY : MALE_PERSONALITY));
            data->isShiny = FALSE;
            data->isFemale = FALSE;
            BattleLoadOpponentMonSpriteGfxCustom(species, data->isFemale, data->isShiny, 1);
            SetMultiuseSpriteTemplateToPokemon(species, 1);
            gMultiuseSpriteTemplate.paletteTag = species;
            front_y = GetBattlerSpriteFinal_YCustom(species, 0, 0);
            data->frontspriteId = CreateSprite(&gMultiuseSpriteTemplate, front_x, front_y, 0);
            gSprites[data->frontspriteId].oam.paletteNum = 1;
            gSprites[data->frontspriteId].callback = SpriteCallbackDummy;
            gSprites[data->frontspriteId].oam.priority = 0;
            //Front Shadow
            LoadAndCreateEnemyShadowSpriteCustom(data, species);

            //Back
            HandleLoadSpecialPokePic(FALSE, gMonSpritesGfxPtr->spritesGfx[2], species, (data->isFemale ? FEMALE_PERSONALITY : MALE_PERSONALITY));
            BattleLoadOpponentMonSpriteGfxCustom(species, data->isFemale, data->isShiny, 4);
            SetMultiuseSpriteTemplateToPokemon(species, 2);
            offset_y = gSpeciesInfo[species].backPicYOffset;
            data->backspriteId = CreateSprite(&gMultiuseSpriteTemplate, VISUALIZER_MON_BACK_X, VISUALIZER_MON_BACK_Y + offset_y, 0);
            gSprites[data->backspriteId].oam.paletteNum = 4;
            gSprites[data->backspriteId].callback = SpriteCallbackDummy;
            gSprites[data->backspriteId].oam.priority = 0;

            //Icon Sprite
            data->iconspriteId = CreateMonIcon(species, SpriteCB_MonIcon, VISUALIZER_ICON_X, VISUALIZER_ICON_Y, 4, (data->isFemale ? FEMALE_PERSONALITY : MALE_PERSONALITY));
            gSprites[data->iconspriteId].oam.priority = 0;

            //Follower Sprite
            data->followerspriteId = CreateObjectGraphicsSprite(OBJ_EVENT_MON + species, SpriteCB_Follower, VISUALIZER_FOLLOWER_X, VISUALIZER_FOLLOWER_Y, 0);
            gSprites[data->followerspriteId].oam.priority = 0;
            gSprites[data->followerspriteId].anims = sAnims_Follower;

            //Modify Arrows
            SetUpModifyArrows(data);
            PrintDigitChars(data);

            //Option Arrow
            SetUpOptionArrows(data);

            //Modify Y Pos Arrow
            SetUpYPosModifyArrows(data);

            //BattleNg Name
            PrintBattleBgName(taskId);

            //Footprint
            DrawFootprint(WIN_FOOTPRINT, species);
            CopyWindowToVram(WIN_FOOTPRINT, COPYWIN_GFX);

            gMain.state++;
            break;
        case 4:
            EnableInterrupts(1);
            SetVBlankCallback(VBlankCB);
            SetMainCallback2(CB2_PokemonSpriteVisualizerRunner);
            m4aMPlayVolumeControl(&gMPlayInfo_BGM, 0xFFFF, 0x80);
            break;
    }
}

static void CB2_PokemonSpriteVisualizerRunner(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void ResetBGs_PokemonSpriteVisualizer(u16 a)
{
    if (!(a & DISPCNT_BG0_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG0_ON);
        SetGpuReg(REG_OFFSET_BG0CNT, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    }
    if (!(a & DISPCNT_BG1_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG1_ON);
        SetGpuReg(REG_OFFSET_BG1CNT, 0);
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    }
    if (!(a & DISPCNT_BG2_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG2_ON);
        SetGpuReg(REG_OFFSET_BG2CNT, 0);
        SetGpuReg(REG_OFFSET_BG2HOFS, 0);
        SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    }
    if (!(a & DISPCNT_BG3_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG3_ON);
        SetGpuReg(REG_OFFSET_BG3CNT, 0);
        SetGpuReg(REG_OFFSET_BG3HOFS, 0);
        SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    }
    if (!(a & DISPCNT_OBJ_ON))
    {
        ClearGpuRegBits(0, DISPCNT_OBJ_ON);
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
    }
}

static void ApplyOffsetSpriteValues(struct PokemonSpriteVisualizer *data)
{
    u16 species = data->currentmonId;
    //Back
    gSprites[data->backspriteId].y = VISUALIZER_MON_BACK_Y + gSpeciesInfo[species].backPicYOffset + data->offsetsSpriteValues.offset_back_picCoords;
    //Front
    gSprites[data->frontspriteId].y = GetBattlerSpriteFinal_YCustom(species, data->offsetsSpriteValues.offset_front_picCoords, data->offsetsSpriteValues.offset_front_elevation);

    if (data->currentSubmenu == 2)
        UpdateShadowSpriteInvisible(data);
}

static void UpdateSubmenuOneOptionValue(u8 taskId, bool8 increment)
{
    struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
    u8 option = data->submenuYpos[1];

    switch (option)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        UpdateBattleBg(taskId, increment);
        break;
    case 3:
        if (GetSpeciesFormTable(data->currentmonId) != NULL)
        {
            struct PokemonSpriteVisualizerModifyArrows *modArrows = &data->modifyArrows;
            u8 formId = GetFormIdFromFormSpeciesId(data->currentmonId);
            const u16 *formTable = GetSpeciesFormTable(data->currentmonId);
            if (increment)
            {
                if (formTable[formId + 1] != FORM_SPECIES_END)
                    modArrows->currValue = GetFormSpeciesId(data->currentmonId, formId + 1);
                else
                    modArrows->currValue = formTable[0];
            }
            else
            {
                if (formTable[formId] == formTable[0])
                {
                    u8 lastForm;
                    for (lastForm = 0; formTable[lastForm] != FORM_SPECIES_END; lastForm++)
                    {
                        if (formTable[lastForm + 1] == FORM_SPECIES_END)
                            break;
                    }
                    modArrows->currValue = formTable[lastForm];
                }
                else
                    modArrows->currValue = GetFormSpeciesId(data->currentmonId, formId - 1);
            }
            ResetOffsetSpriteValues(data);
            ResetShadowSettings(data, modArrows->currValue);

            UpdateBattlerValue(data);
            ReloadPokemonSprites(data);
            VBlankIntrWait();
            PlaySE(SE_DEX_SCROLL);
        }
        break;
    default:
        break;
    }
}

static void UpdateSubmenuTwoOptionValue(u8 taskId, bool8 increment)
{
    struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
    u16 species = data->currentmonId;
    u8 option = data->submenuYpos[2];
    s8 offset;
    u8 y;

    switch (option)
    {
    case 0: //Back picCoords
        offset = data->offsetsSpriteValues.offset_back_picCoords;
        if (increment)
        {
            if (offset == MAX_Y_OFFSET)
                offset = -data->constSpriteValues.backPicCoords;
            else
                offset += 1;
        }
        else
        {
            if (offset == -data->constSpriteValues.backPicCoords)
                offset = MAX_Y_OFFSET;
            else
                offset -= 1;
        }
        data->offsetsSpriteValues.offset_back_picCoords = offset;
        gSprites[data->backspriteId].y = VISUALIZER_MON_BACK_Y + gSpeciesInfo[species].backPicYOffset + offset;
        break;
    case 1: //Front picCoords
        offset = data->offsetsSpriteValues.offset_front_picCoords;
        if (increment)
        {
            if (offset == MAX_Y_OFFSET)
                offset = -data->constSpriteValues.frontPicCoords;
            else
                offset += 1;
        }
        else
        {
            if (offset == -data->constSpriteValues.frontPicCoords)
                offset = MAX_Y_OFFSET;
            else
                offset -= 1;
        }
        data->offsetsSpriteValues.offset_front_picCoords = offset;
        y = GetBattlerSpriteFinal_YCustom(species, offset, data->offsetsSpriteValues.offset_front_elevation);
        gSprites[data->frontspriteId].y = y;
        break;
    case 2: //Front elevation
        offset = data->offsetsSpriteValues.offset_front_elevation;
        if (increment)
        {
            if (offset == MAX_Y_OFFSET)
                offset = -data->constSpriteValues.frontElevation;
            else
                offset += 1;
        }
        else
        {
            if (offset == -data->constSpriteValues.frontElevation)
                offset = MAX_Y_OFFSET;
            else
                offset -= 1;
        }
        data->offsetsSpriteValues.offset_front_elevation = offset;
        y = GetBattlerSpriteFinal_YCustom(species, data->offsetsSpriteValues.offset_front_picCoords, offset);
        gSprites[data->frontspriteId].y = y;
        UpdateShadowSpriteInvisible(data);
        break;
    }

    UpdateYPosOffsetText(data);
}

static void UpdateShadowSettingsValue(u8 taskId, bool8 increment)
{
    if (B_ENEMY_MON_SHADOW_STYLE <= GEN_3 || P_GBA_STYLE_SPECIES_GFX == TRUE)
        return;

    struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
    u8 option = data->submenuYpos[2];
    s8 *offset;
    s16 *leftTarget, *rightTarget;
    if (option == 0)
    {
        offset = &data->shadowSettings.overrideX;
        leftTarget = &gSprites[data->frontShadowSpriteIdPrimary].tShadowXOffset;
        rightTarget = &gSprites[data->frontShadowSpriteIdSecondary].tShadowXOffset;
    }
    else
    {
        offset = &data->shadowSettings.overrideY;
        leftTarget = &gSprites[data->frontShadowSpriteIdPrimary].tShadowYOffset;
        rightTarget = &gSprites[data->frontShadowSpriteIdSecondary].tShadowYOffset;
    }

    *offset = *offset + (increment ? 1 : -1);
    if (*offset > 20)
        *offset = -20;
    else if (*offset < -20)
        *offset = 20;
    UpdateShadowSettingsText(data);

    *leftTarget = (s16)*offset;
    *rightTarget = (s16)*offset;
}

static void UpdateShadowSizeValue(u8 taskId, bool8 increment)
{
    if (B_ENEMY_MON_SHADOW_STYLE <= GEN_3 || P_GBA_STYLE_SPECIES_GFX == TRUE)
        return;

    struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
    s8 update;

    if (increment)
    {
        if (data->shadowSettings.overrideSize == SHADOW_SIZE_XL_BATTLE_ONLY)
        {
            update = -data->shadowSettings.overrideSize;
            data->shadowSettings.overrideSize = SHADOW_SIZE_S;
        }
        else
        {
            update = 1;
            data->shadowSettings.overrideSize += 1;
        }
    }
    else
    {
        if (data->shadowSettings.overrideSize == SHADOW_SIZE_S)
        {
            update = SHADOW_SIZE_XL_BATTLE_ONLY;
            data->shadowSettings.overrideSize = update;
        }
        else
        {
            update = -1;
            data->shadowSettings.overrideSize -= 1;
        }
    }

    UpdateShadowSettingsText(data);
    gSprites[data->frontShadowSpriteIdPrimary].oam.tileNum += (8 * update);
    gSprites[data->frontShadowSpriteIdSecondary].oam.tileNum += (8 * update);
}

#define READ_PTR_FROM_TASK(taskId, dataId)                      \
    (void *)(                                                   \
    ((u16)(gTasks[taskId].data[dataId]) |                       \
    ((u16)(gTasks[taskId].data[dataId + 1]) << 16)))

#define STORE_PTR_IN_TASK(ptr, taskId, dataId)                 \
{                                                              \
    gTasks[taskId].data[dataId] = (u32)(ptr);                  \
    gTasks[taskId].data[dataId + 1] = (u32)(ptr) >> 16;        \
}

static void HandleInput_PokemonSpriteVisualizer(u8 taskId)
{
    struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
    struct Sprite *Frontsprite = &gSprites[data->frontspriteId];
    struct Sprite *Backsprite = &gSprites[data->backspriteId];

    if (JOY_NEW(L_BUTTON)  && (Backsprite->callback == SpriteCallbackDummy))
    {
        PlayCryInternal(data->currentmonId, 0, 120, 10, CRY_MODE_NORMAL);
    }
    if (JOY_NEW(R_BUTTON) && (Frontsprite->callback == SpriteCallbackDummy))
    {
        PlayCryInternal(data->currentmonId, 0, 120, 10, CRY_MODE_NORMAL);
    }

    if (JOY_NEW(START_BUTTON))
    {
        data->isShiny = !data->isShiny;

        if(data->isShiny)
            PlaySE(SE_SHINY);

        ReloadPokemonSprites(data);
        ApplyOffsetSpriteValues(data);
    }
    if (JOY_NEW(SELECT_BUTTON) && SpeciesHasGenderDifferences(data->currentmonId))
    {
        data->isFemale = !data->isFemale;
        PrintDigitChars(data);
        UpdateBattlerValue(data);
        ReloadPokemonSprites(data);
        VBlankIntrWait();
        PlaySE(SE_DEX_SCROLL);
    }

    if (data->currentSubmenu == 0)
    {
        if (JOY_NEW(A_BUTTON))
        {
            data->currentSubmenu = 1;
            SetArrowInvisibility(data);
            PrintInstructionsOnWindow(data);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Exit_PokemonSpriteVisualizer;
            PlaySE(SE_PC_OFF);
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            if (TryMoveDigit(&data->modifyArrows, FALSE))
            {
                data->isFemale = FALSE;
                PrintDigitChars(data);
                UpdateBattlerValue(data);
                ResetShadowSettings(data, data->currentmonId);
                ReloadPokemonSprites(data);
                ResetOffsetSpriteValues(data);
            }
            PlaySE(SE_DEX_SCROLL);
            VBlankIntrWait();
        }
        else if (JOY_NEW(DPAD_UP))
        {
            if (TryMoveDigit(&data->modifyArrows, TRUE))
            {
                data->isFemale = FALSE;
                PrintDigitChars(data);
                UpdateBattlerValue(data);
                ResetShadowSettings(data, data->currentmonId);
                ReloadPokemonSprites(data);
                ResetOffsetSpriteValues(data);
            }

            PlaySE(SE_DEX_SCROLL);
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            if (data->modifyArrows.currentDigit != 0)
            {
                data->modifyArrows.currentDigit--;
                gSprites[data->modifyArrows.arrowSpriteId[0]].x2 -= 6;
                gSprites[data->modifyArrows.arrowSpriteId[1]].x2 -= 6;
            }
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
            if (data->modifyArrows.currentDigit != (data->modifyArrows.maxDigits - 1))
            {
                data->modifyArrows.currentDigit++;
                gSprites[data->modifyArrows.arrowSpriteId[0]].x2 += 6;
                gSprites[data->modifyArrows.arrowSpriteId[1]].x2 += 6;
            }
        }

    }
    else if (data->currentSubmenu == 1) //Submenu 1
    {
        if (JOY_NEW(A_BUTTON))
        {
            data->currentSubmenu = 2;
            PrintInstructionsOnWindow(data);
            SetArrowInvisibility(data);
            SetConstSpriteValues(data);
            UpdateYPosOffsetText(data);

            gSprites[data->followerspriteId].invisible = TRUE;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            data->currentSubmenu = 0;
            if (data->submenuYpos[1] == 3)
            {
                data->submenuYpos[1] = 2;
                data->optionArrows.currentDigit = data->submenuYpos[1];
                gSprites[data->optionArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12;
            }
            SetArrowInvisibility(data);
            PrintInstructionsOnWindow(data);
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            data->submenuYpos[1] += 1;
            if (data->submenuYpos[1] >= 3)
            {
                if ((GetSpeciesFormTable(data->currentmonId) == NULL) || (data->submenuYpos[1] >= 4))
                    data->submenuYpos[1] = 0;
            }
            data->optionArrows.currentDigit = data->submenuYpos[1];
            gSprites[data->optionArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_UP))
        {
            if (data->submenuYpos[1] == 0)
            {
                if (GetSpeciesFormTable(data->currentmonId) != NULL)
                    data->submenuYpos[1] = 3;
                else
                    data->submenuYpos[1] = 2;
            }
            else
                data->submenuYpos[1] -= 1;

            data->optionArrows.currentDigit = data->submenuYpos[1];
            gSprites[data->optionArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            UpdateSubmenuOneOptionValue(taskId, FALSE);
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
            UpdateSubmenuOneOptionValue(taskId, TRUE);
        }
    }
    else if (data->currentSubmenu == 2) //Submenu 2
    {
        if (JOY_NEW(A_BUTTON) && B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE)
        {
            data->currentSubmenu = 3;
            PrintInstructionsOnWindow(data);
            SetArrowInvisibility(data);
            UpdateShadowSettingsText(data);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            data->currentSubmenu = 1;

            SetArrowInvisibility(data);
            PrintInstructionsOnWindow(data);

            gSprites[data->followerspriteId].invisible = FALSE;
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            data->submenuYpos[2] += 1;
            if (data->submenuYpos[2] >= 3)
                data->submenuYpos[2] = 0;

            data->yPosModifyArrows.currentDigit = data->submenuYpos[2];
            gSprites[data->yPosModifyArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->yPosModifyArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_UP))
        {
            if (data->submenuYpos[2] == 0)
                    data->submenuYpos[2] = 2;
            else
                data->submenuYpos[2] -= 1;

            data->yPosModifyArrows.currentDigit = data->submenuYpos[2];
            gSprites[data->yPosModifyArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->yPosModifyArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            UpdateSubmenuTwoOptionValue(taskId, FALSE);
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
            UpdateSubmenuTwoOptionValue(taskId, TRUE);
        }
    }
    else if (data->currentSubmenu == 3) // Submenu 3
    {
        if (JOY_NEW(B_BUTTON))
        {
            data->currentSubmenu = 2;
            PrintInstructionsOnWindow(data);
            SetArrowInvisibility(data);
            SetConstSpriteValues(data);
            UpdateYPosOffsetText(data);
        }
        else if (JOY_NEW(DPAD_DOWN))
        {
            data->submenuYpos[2] += 1;
            if (data->submenuYpos[2] >= 3)
                data->submenuYpos[2] = 0;

            data->yPosModifyArrows.currentDigit = data->submenuYpos[2];
            gSprites[data->yPosModifyArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->yPosModifyArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_UP))
        {
            if (data->submenuYpos[2] == 0)
                data->submenuYpos[2] = 2;
            else
                data->submenuYpos[2] -= 1;

            data->yPosModifyArrows.currentDigit = data->submenuYpos[2];
            gSprites[data->yPosModifyArrows.arrowSpriteId[0]].y = OPTIONS_ARROW_Y + data->yPosModifyArrows.currentDigit * 12;
        }
        else if (JOY_NEW(DPAD_LEFT))
        {
            if (data->submenuYpos[2] < 2)
                UpdateShadowSettingsValue(taskId, FALSE);
            else
                UpdateShadowSizeValue(taskId, FALSE);
        }
        else if (JOY_NEW(DPAD_RIGHT))
        {
            if (data->submenuYpos[2] < 2)
                UpdateShadowSettingsValue(taskId, TRUE);
            else
                UpdateShadowSizeValue(taskId, TRUE);
        }
    }
}

static void ReloadPokemonSprites(struct PokemonSpriteVisualizer *data)
{
    const u16 *palette;
    u16 species = data->currentmonId;
    s16 offset_y;
    u8 front_x = sBattlerCoords[0][1].x;
    u8 front_y;

    DestroySprite(&gSprites[data->frontspriteId]);
    DestroySprite(&gSprites[data->backspriteId]);
    DestroySprite(&gSprites[data->iconspriteId]);
    DestroySprite(&gSprites[data->followerspriteId]);

    DestroySprite(&gSprites[data->frontShadowSpriteIdPrimary]);
    if (B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE)
        DestroySprite(&gSprites[data->frontShadowSpriteIdSecondary]);

    FreeMonSpritesGfx();
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    ResetAllPicSprites();
    FreeMonIconPalettes();

    AllocateMonSpritesGfx();
    LoadMonIconPalettePersonality(species, (data->isFemale ? FEMALE_PERSONALITY : MALE_PERSONALITY));

    //Update instructions
    PrintInstructionsOnWindow(data);

    //Palettes
    palette = GetMonSpritePalFromSpecies(species, data->isShiny, data->isFemale);
    LoadSpritePaletteWithTag(palette, species);
    //Front
    HandleLoadSpecialPokePic(TRUE, gMonSpritesGfxPtr->spritesGfx[1], species, (data->isFemale ? FEMALE_PERSONALITY : MALE_PERSONALITY));
    BattleLoadOpponentMonSpriteGfxCustom(species, data->isFemale, data->isShiny, 1);
    SetMultiuseSpriteTemplateToPokemon(species, 1);
    gMultiuseSpriteTemplate.paletteTag = species;
    front_y = GetBattlerSpriteFinal_YCustom(species, 0, 0);
    data->frontspriteId = CreateSprite(&gMultiuseSpriteTemplate, front_x, front_y, 0);
    gSprites[data->frontspriteId].oam.paletteNum = 1;
    gSprites[data->frontspriteId].callback = SpriteCallbackDummy;
    gSprites[data->frontspriteId].oam.priority = 0;
    //Front Shadow
    LoadAndCreateEnemyShadowSpriteCustom(data, species);

    //Back
    HandleLoadSpecialPokePic(FALSE, gMonSpritesGfxPtr->spritesGfx[2], species, (data->isFemale ? FEMALE_PERSONALITY : MALE_PERSONALITY));
    BattleLoadOpponentMonSpriteGfxCustom(species, data->isFemale, data->isShiny, 5);
    SetMultiuseSpriteTemplateToPokemon(species, 2);
    offset_y = gSpeciesInfo[species].backPicYOffset;
    data->backspriteId = CreateSprite(&gMultiuseSpriteTemplate, VISUALIZER_MON_BACK_X, VISUALIZER_MON_BACK_Y + offset_y, 0);
    gSprites[data->backspriteId].oam.paletteNum = 5;
    gSprites[data->backspriteId].callback = SpriteCallbackDummy;
    gSprites[data->backspriteId].oam.priority = 0;

    //Icon Sprite
    data->iconspriteId = CreateMonIcon(species, SpriteCB_MonIcon, VISUALIZER_ICON_X, VISUALIZER_ICON_Y, 4, (data->isFemale ? FEMALE_PERSONALITY : MALE_PERSONALITY));
    gSprites[data->iconspriteId].oam.priority = 0;

    //Follower Sprite
    u16 graphicsId = species + OBJ_EVENT_MON;
    if (data->isShiny)
        graphicsId += OBJ_EVENT_MON_SHINY;
    if (data->isFemale)
        graphicsId += OBJ_EVENT_MON_FEMALE;
    data->followerspriteId = CreateObjectGraphicsSprite(graphicsId,
                                                        SpriteCB_Follower,
                                                        VISUALIZER_FOLLOWER_X,
                                                        VISUALIZER_FOLLOWER_Y,
                                                        0);
    gSprites[data->followerspriteId].oam.priority = 0;
    gSprites[data->followerspriteId].anims = sAnims_Follower;

    //Modify Arrows
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->modifyArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X + (data->modifyArrows.currentDigit * 6), MODIFY_DIGITS_ARROW1_Y, 0);
    data->modifyArrows.arrowSpriteId[1] = CreateSprite(&gSpriteTemplate_Arrow, MODIFY_DIGITS_ARROW_X + (data->modifyArrows.currentDigit * 6), MODIFY_DIGITS_ARROW2_Y, 0);
    gSprites[data->modifyArrows.arrowSpriteId[1]].animNum = 1;

    //Option Arrow
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->optionArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, OPTIONS_ARROW_Y + data->optionArrows.currentDigit * 12, 0);
    gSprites[data->optionArrows.arrowSpriteId[0]].animNum = 2;

    //Y Pos Modify Arrow
    LoadSpritePalette(&gSpritePalette_Arrow);
    data->yPosModifyArrows.arrowSpriteId[0] = CreateSprite(&gSpriteTemplate_Arrow, OPTIONS_ARROW_1_X, OPTIONS_ARROW_Y + data->yPosModifyArrows.currentDigit * 12, 0);
    gSprites[data->yPosModifyArrows.arrowSpriteId[0]].animNum = 2;

    //Arrow invisibility
    SetArrowInvisibility(data);

    //Footprint
    DrawFootprint(WIN_FOOTPRINT, species);
    CopyWindowToVram(WIN_FOOTPRINT, COPYWIN_GFX);
}

static void Exit_PokemonSpriteVisualizer(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        struct PokemonSpriteVisualizer *data = GetStructPtr(taskId);
        Free(data);
        FreeMonSpritesGfx();
        DestroyTask(taskId);
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
    }
}
