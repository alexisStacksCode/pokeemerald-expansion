#include "constants/abilities.h"
#include "species_info/shared_dex_text.h"
#include "species_info/shared_front_pic_anims.h"

// Macros for ease of use.

#define EVOLUTION(...) (const struct Evolution[]) { __VA_ARGS__, { EVOLUTIONS_END }, }
#define CONDITIONS(...) ((const struct EvolutionParam[]) { __VA_ARGS__, {CONDITIONS_END} })

#define ANIM_FRAMES(...) (const union AnimCmd *const[]) { sAnim_GeneralFrame0, (const union AnimCmd[]) { __VA_ARGS__ ANIMCMD_END, }, }

#if P_FOOTPRINTS
#define FOOTPRINT(sprite) .footprint = gMonFootprint_## sprite,
#else
#define FOOTPRINT(sprite)
#endif

#if B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE
#define SHADOW(x, y, size)  .enemyShadowXOffset = x, .enemyShadowYOffset = y, .enemyShadowSize = size,
#define NO_SHADOW           .suppressEnemyShadow = TRUE,
#else
#define SHADOW(x, y, size)  .enemyShadowXOffset = 0, .enemyShadowYOffset = 0, .enemyShadowSize = 0,
#define NO_SHADOW           .suppressEnemyShadow = FALSE,
#endif

#define SIZE_32x32 1
#define SIZE_64x64 0

// Set .compressed = OW_GFX_COMPRESS
#define COMP OW_GFX_COMPRESS

#if OW_POKEMON_OBJECT_EVENTS
#if OW_PKMN_OBJECTS_SHARE_PALETTES == FALSE
#define OVERWORLD_PAL(...)                                  \
    .overworldPalette = DEFAULT(NULL, __VA_ARGS__),         \
    .overworldShinyPalette = DEFAULT_2(NULL, __VA_ARGS__),
#if P_GENDER_DIFFERENCES
#define OVERWORLD_PAL_FEMALE(...)                                 \
    .overworldPaletteFemale = DEFAULT(NULL, __VA_ARGS__),         \
    .overworldShinyPaletteFemale = DEFAULT_2(NULL, __VA_ARGS__),
#else
#define OVERWORLD_PAL_FEMALE(...)
#endif //P_GENDER_DIFFERENCES
#else
#define OVERWORLD_PAL(...)
#define OVERWORLD_PAL_FEMALE(...)
#endif //OW_PKMN_OBJECTS_SHARE_PALETTES == FALSE

#define OVERWORLD_DATA(picTable, _size, shadow, _tracks, _anims)                                                                     \
{                                                                                                                                       \
    .tileTag = TAG_NONE,                                                                                                                \
    .paletteTag = OBJ_EVENT_PAL_TAG_DYNAMIC,                                                                                            \
    .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,                                                                                     \
    .size = (_size == SIZE_32x32 ? 512 : 2048),                                                                                         \
    .width = (_size == SIZE_32x32 ? 32 : 64),                                                                                           \
    .height = (_size == SIZE_32x32 ? 32 : 64),                                                                                          \
    .paletteSlot = PALSLOT_NPC_1,                                                                                                       \
    .shadowSize = shadow,                                                                                                               \
    .inanimate = FALSE,                                                                                                                 \
    .compressed = COMP,                                                                                                                 \
    .tracks = _tracks,                                                                                                                  \
    .oam = (_size == SIZE_32x32 ? &gObjectEventBaseOam_32x32 : &gObjectEventBaseOam_64x64),                                             \
    .subspriteTables = (_size == SIZE_32x32 ? sOamTables_32x32 : sOamTables_64x64),                                                     \
    .anims = _anims,                                                                                                                    \
    .images = picTable,                                                                                                                 \
    .affineAnims = gDummySpriteAffineAnimTable,                                                                                         \
}

#define OVERWORLD(objEventPic, _size, shadow, _tracks, _anims, ...)                                 \
    .overworldData = OVERWORLD_DATA(objEventPic, _size, shadow, _tracks, _anims),                   \
    OVERWORLD_PAL(__VA_ARGS__)

#if P_GENDER_DIFFERENCES
#define OVERWORLD_FEMALE(objEventPic, _size, shadow, _tracks, _anims, ...)                          \
    .overworldDataFemale = OVERWORLD_DATA(objEventPic, _size, shadow, _tracks, _anims),             \
    OVERWORLD_PAL_FEMALE(__VA_ARGS__)
#else
#define OVERWORLD_FEMALE(...)
#endif //P_GENDER_DIFFERENCES

#else
#define OVERWORLD(...)
#define OVERWORLD_FEMALE(...)
#define OVERWORLD_PAL(...)
#define OVERWORLD_PAL_FEMALE(...)
#endif //OW_POKEMON_OBJECT_EVENTS

// Maximum value for a female Pokémon is 254 (MON_FEMALE) which is 100% female.
// 255 (MON_GENDERLESS) is reserved for genderless Pokémon.
#define PERCENT_FEMALE(percent) min(254, ((percent * 255) / 100))

#define MON_TYPES(type1, ...) { type1, DEFAULT(type1, __VA_ARGS__) }
#define MON_EGG_GROUPS(group1, ...) { group1, DEFAULT(group1, __VA_ARGS__) }

#define FLIP    0
#define NO_FLIP 1

const struct SpeciesInfo gSpeciesInfo[] =
{
    [SPECIES_NONE] =
    {
        .speciesName = _("??????????"),
        .cryId = CRY_NONE,
        .natDexNum = NATIONAL_DEX_NONE,
        .categoryName = _("Unknown"),
        .height = 0,
        .weight = 0,
        .description = gFallbackPokedexText,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_CircledQuestionMark,
        .frontPicSize = MON_COORDS_SIZE(40, 40),
        .frontPicYOffset = 12,
        .frontAnimFrames = sAnims_TwoFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_CircledQuestionMark,
        .backPicSize = MON_COORDS_SIZE(40, 40),
        .backPicYOffset = 12,
        .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_CircledQuestionMark,
        .shinyPalette = gMonShinyPalette_CircledQuestionMark,
        .iconSprite = gMonIcon_QuestionMark,
        .iconPalIndex = 0,
        .pokemonJumpType = PKMN_JUMP_TYPE_NONE,
        FOOTPRINT(QuestionMark)
        SHADOW(-1, 0, SHADOW_SIZE_M)
    #if OW_POKEMON_OBJECT_EVENTS
        .overworldData = {
            .tileTag = TAG_NONE,
            .paletteTag = OBJ_EVENT_PAL_TAG_SUBSTITUTE,
            .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,
            .size = 512,
            .width = 32,
            .height = 32,
            .paletteSlot = PALSLOT_NPC_1,
            .shadowSize = SHADOW_SIZE_M,
            .inanimate = FALSE,
            .compressed = COMP,
            .tracks = TRACKS_FOOT,
            .oam = &gObjectEventBaseOam_32x32,
            .subspriteTables = sOamTables_32x32,
            .anims = sAnimTable_Following,
            .images = sPicTable_Substitute,
            .affineAnims = gDummySpriteAffineAnimTable,
        },
    #endif
        .levelUpLearnset = sNoneLevelUpLearnset,
        .teachableLearnset = sNoneTeachableLearnset,
        .eggMoveLearnset = sNoneEggMoveLearnset,
    },

    #include "species_info/gen_1_families.h"
    #include "species_info/gen_2_families.h"
    #include "species_info/gen_3_families.h"
    #include "species_info/gen_4_families.h"
    #include "species_info/gen_5_families.h"
    #include "species_info/gen_6_families.h"
    #include "species_info/gen_7_families.h"
    #include "species_info/gen_8_families.h"
    #include "species_info/gen_9_families.h"

    [SPECIES_EGG] =
    {
        .frontPic = gMonFrontPic_Egg,
        .frontPicSize = MON_COORDS_SIZE(24, 24),
        .frontPicYOffset = 20,
        .backPic = gMonFrontPic_Egg,
        .backPicSize = MON_COORDS_SIZE(24, 24),
        .backPicYOffset = 20,
        .palette = gMonPalette_Egg,
        .shinyPalette = gMonPalette_Egg,
        .iconSprite = gMonIcon_Egg,
        .iconPalIndex = 1,
    },

    /* You may add any custom species below this point based on the following structure: */

    /*
    [SPECIES_NONE] =
    {
        .baseHP        = 1,
        .baseAttack    = 1,
        .baseDefense   = 1,
        .baseSpeed     = 1,
        .baseSpAttack  = 1,
        .baseSpDefense = 1,
        .types = MON_TYPES(TYPE_MYSTERY),
        .catchRate = 255,
        .expYield = 67,
        .evYield_HP = 1,
        .evYield_Defense = 1,
        .evYield_SpDefense = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_NONE, ABILITY_CURSED_BODY, ABILITY_DAMP },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("??????????"),
        .cryId = CRY_NONE,
        .natDexNum = NATIONAL_DEX_NONE,
        .categoryName = _("Unknown"),
        .height = 0,
        .weight = 0,
        .description = COMPOUND_STRING(
            "This is a newly discovered Pokémon.\n"
            "It is currently under investigation.\n"
            "No detailed information is available\n"
            "at this time."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_CircledQuestionMark,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_None,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_CircledQuestionMark,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 7,
#if P_GENDER_DIFFERENCES
        .frontPicFemale = gMonFrontPic_CircledQuestionMark,
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .backPicFemale = gMonBackPic_CircledQuestionMarkF,
        .backPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .paletteFemale = gMonPalette_CircledQuestionMarkF,
        .shinyPaletteFemale = gMonShinyPalette_CircledQuestionMarkF,
        .iconSpriteFemale = gMonIcon_QuestionMarkF,
        .iconPalIndexFemale = 1,
#endif //P_GENDER_DIFFERENCES
        .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_CircledQuestionMark,
        .shinyPalette = gMonShinyPalette_CircledQuestionMark,
        .iconSprite = gMonIcon_QuestionMark,
        .iconPalIndex = 0,
        FOOTPRINT(QuestionMark)
        .levelUpLearnset = sNoneLevelUpLearnset,
        .teachableLearnset = sNoneTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 100, SPECIES_NONE},
                                {EVO_ITEM, ITEM_MOOMOO_MILK, SPECIES_NONE}),
        //.formSpeciesIdTable = sNoneFormSpeciesIdTable,
        //.formChangeTable = sNoneFormChangeTable,
        //.perfectIVCount = NUM_STATS,
    },
    */

    [SPECIES_CLOUD_STRIFE] =
    {
        .baseHP        = 85,
        .baseAttack    = 120,
        .baseDefense   = 90,
        .baseSpeed     = 75,
        .baseSpAttack  = 95,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_FIGHTING, TYPE_GRASS),
        .catchRate = 45,
        .expYield = 240,
        .evYield_Attack = 2,
        .genderRatio = MON_MALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_SHARPNESS, ABILITY_NONE, ABILITY_MOXIE },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Cloud Strife"),
        .cryId = CRY_NONE,
        .natDexNum = NATIONAL_DEX_CLOUD_STRIFE,
        .categoryName = _("EcoTerrorist"),
        .height = 0,
        .weight = 0,
        .description = COMPOUND_STRING(
            "This is a newly discovered Pokémon.\n"
            "It is currently under investigation.\n"
            "No detailed information is available\n"
            "at this time."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_CircledQuestionMark,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_CircledQuestionMark,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 7,
        .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_CircledQuestionMark,
        .shinyPalette = gMonShinyPalette_CircledQuestionMark,
        .iconSprite = gMonIcon_QuestionMark,
        .iconPalIndex = 0,
        FOOTPRINT(QuestionMark)
        .levelUpLearnset = sCloudStrifeLevelUpLearnset,
        .teachableLearnset = sCloudStrifeTeachableLearnset,
    },
    [SPECIES_DAPHNE_BLAKE] = // Scooby-Doo
    {
        .baseHP        = 80,
        .baseAttack    = 90,
        .baseDefense   = 65,
        .baseSpeed     = 100,
        .baseSpAttack  = 110,
        .baseSpDefense = 85,
        .types = MON_TYPES(TYPE_PSYCHIC, TYPE_FIRE),
        .catchRate = 45,
        .expYield = 240,
        .evYield_SpAttack = 2,
        .genderRatio = MON_FEMALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_RATTLED, ABILITY_NONE, ABILITY_REGENERATOR },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Daphne Blake"),
        .cryId = CRY_NONE,
        .natDexNum = NATIONAL_DEX_DAPHNE_BLAKE,
        .categoryName = _("DangerProne"),
        .height = 0,
        .weight = 0,
        .description = COMPOUND_STRING(
            "When threatened by a predator, it\n"
            "enters a panic state in its attempt\n"
            "to escape; it is often seen near\n"
            "areas thought to contain a treasure."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_CircledQuestionMark,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_CircledQuestionMark,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 7,
        .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_CircledQuestionMark,
        .shinyPalette = gMonShinyPalette_CircledQuestionMark,
        .iconSprite = gMonIcon_QuestionMark,
        .iconPalIndex = 0,
        FOOTPRINT(QuestionMark)
        .levelUpLearnset = sDaphneBlakeLevelUpLearnset,
        .teachableLearnset = sDaphneBlakeTeachableLearnset,
    },
    [SPECIES_DUKE_NUKEM] = // Duke Nukem
    {
        .baseHP        = 100,
        .baseAttack    = 95,
        .baseDefense   = 75,
        .baseSpeed     = 90,
        .baseSpAttack  = 95,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_DARK, TYPE_WATER),
        .catchRate = 45,
        .expYield = 240,
        .evYield_Attack = 1,
        .evYield_SpAttack = 1,
        .genderRatio = MON_MALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Duke Nukem"),
        .cryId = CRY_NONE,
        .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
        .categoryName = _("Womanizer"),
        .height = 0,
        .weight = 0,
        .description = COMPOUND_STRING(
            "This is a newly discovered Pokémon.\n"
            "It is currently under investigation.\n"
            "No detailed information is available\n"
            "at this time."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_CircledQuestionMark,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_CircledQuestionMark,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 7,
        .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_CircledQuestionMark,
        .shinyPalette = gMonShinyPalette_CircledQuestionMark,
        .iconSprite = gMonIcon_QuestionMark,
        .iconPalIndex = 0,
        FOOTPRINT(QuestionMark)
        .levelUpLearnset = sDukeNukemLevelUpLearnset,
        .teachableLearnset = sDukeNukemTeachableLearnset,
    },
    // [SPECIES_BANSHEE] = // Doom (1993) - based on The Blob
    // {
    //     .baseHP        = 25,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 5,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_GHOST),
    //     .catchRate = 255,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_LEVITATE, ABILITY_NONE, ABILITY_NONE },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Lost Soul"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("Hell Skull"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_LOST_SOUL] = // Doom (1993)
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 140,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_GHOST, TYPE_FIRE),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_LEVITATE, ABILITY_NONE, ABILITY_NONE },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Lost Soul"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("Hell Skull"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_GHOUL] = // Doom (1993) - based on pre-release Lost Soul
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 50,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_GHOST, TYPE_PSYCHIC),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_LEVITATE, ABILITY_NONE, ABILITY_NONE },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Lost Soul"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("Hell Skull"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_IMPLING] = // Doom RPG
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_FIRE),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Imp"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("CannonFodder"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_IMP] = // Doom (1993)
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_FIRE),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Imp"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("CannonFodder"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_IMP_LORD] = // Doom RPG
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_FIRE),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Imp"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("CannonFodder"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_NIGHTMARE_IMP] = // Doom 64
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_FIRE, TYPE_GHOST),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Terror Imp"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("Nightmare"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_MALWRATH] = // Doom RPG
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_ELECTRIC, TYPE_FLYING),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Malwrath"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("Womanizer"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_CACODEMON] = // Doom (1993)
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_ELECTRIC, TYPE_FLYING),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Cacodemon"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("Womanizer"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_WRETCHED] = // Doom RPG
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_ELECTRIC, TYPE_FLYING),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Wretched"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DUKE_NUKEM,
    //     .categoryName = _("Womanizer"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sDukeNukemLevelUpLearnset,
    //     .teachableLearnset = sDukeNukemTeachableLearnset,
    // },
    // [SPECIES_OGRE] = // Doom RPG
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_FIRE, TYPE_DRAGON),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Ogre"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_BARON_OF_HELL,
    //     .categoryName = _("Bruiser"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sHellKnightLevelUpLearnset,
    //     .teachableLearnset = sHellKnightTeachableLearnset,
    // },
    // [SPECIES_HELL_KNIGHT] = // Doom II: Hell on Earth
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_FIRE, TYPE_DRAGON),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Hell Knight"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_BARON_OF_HELL,
    //     .categoryName = _("Bruiser"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sHellKnightLevelUpLearnset,
    //     .teachableLearnset = sHellKnightTeachableLearnset,
    // },
    // [SPECIES_BARON_OF_HELL] = // Doom (1993)
    // {
    //     .baseHP        = 100,
    //     .baseAttack    = 95,
    //     .baseDefense   = 75,
    //     .baseSpeed     = 90,
    //     .baseSpAttack  = 95,
    //     .baseSpDefense = 75,
    //     .types = MON_TYPES(TYPE_FIRE, TYPE_DRAGON),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    //     .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_NONE, ABILITY_OPPORTUNIST },
    //     .bodyColor = BODY_COLOR_WHITE,
    //     .speciesName = _("Baron"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_BARON_OF_HELL,
    //     .categoryName = _("Bruiser"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sBaronOfHellLevelUpLearnset,
    //     .teachableLearnset = sBaronOfHellTeachableLearnset,
    // },
    // [SPECIES_CORRUPTOR] = // Horizon Zero Dawn
    // {
    //     .baseHP        = 55,
    //     .baseAttack    = 70,
    //     .baseDefense   = 40,
    //     .baseSpeed     = 50,
    //     .baseSpAttack  = 45,
    //     .baseSpDefense = 40,
    //     .types = MON_TYPES(TYPE_STEEL),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
    //     .abilities = { ABILITY_FULL_METAL_BODY, ABILITY_NONE, ABILITY_NONE },
    //     .bodyColor = BODY_COLOR_GRAY,
    //     .speciesName = _("Corruptor"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_CORRUPTOR,
    //     .categoryName = _("Bruiser"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sBaronOfHellLevelUpLearnset,
    //     .teachableLearnset = sBaronOfHellTeachableLearnset,
    // },
    // [SPECIES_DEATHBRINGER] = // Horizon Zero Dawn
    // {
    //     .baseHP        = 105,
    //     .baseAttack    = 75,
    //     .baseDefense   = 70,
    //     .baseSpeed     = 15,
    //     .baseSpAttack  = 85,
    //     .baseSpDefense = 70,
    //     .types = MON_TYPES(TYPE_STEEL, TYPE_GROUND),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
    //     .abilities = { ABILITY_FULL_METAL_BODY, ABILITY_NONE, ABILITY_NONE },
    //     .bodyColor = BODY_COLOR_GRAY,
    //     .speciesName = _("Deathbringer"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_DEATHBRINGER,
    //     .categoryName = _("Weapons Platform"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sBaronOfHellLevelUpLearnset,
    //     .teachableLearnset = sBaronOfHellTeachableLearnset,
    // },
    // [SPECIES_METAL_DEVIL] = // Horizon Zero Dawn
    // {
    //     .baseHP        = 205,
    //     .baseAttack    = 90,
    //     .baseDefense   = 105,
    //     .baseSpeed     = 5,
    //     .baseSpAttack  = 90,
    //     .baseSpDefense = 105,
    //     .types = MON_TYPES(TYPE_STEEL, TYPE_GROUND),
    //     .catchRate = 45,
    //     .expYield = 240,
    //     .evYield_Attack = 1,
    //     .evYield_SpAttack = 1,
    //     .genderRatio = MON_GENDERLESS,
    //     .eggCycles = 20,
    //     .friendship = STANDARD_FRIENDSHIP,
    //     .growthRate = GROWTH_SLOW,
    //     .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
    //     .abilities = { ABILITY_FULL_METAL_BODY, ABILITY_NONE, ABILITY_NONE },
    //     .bodyColor = BODY_COLOR_GRAY,
    //     .speciesName = _("Metal Devil"),
    //     .cryId = CRY_NONE,
    //     .natDexNum = NATIONAL_DEX_METAL_DEVIL,
    //     .categoryName = _("Titan"),
    //     .height = 0,
    //     .weight = 0,
    //     .description = COMPOUND_STRING(
    //         "This is a newly discovered Pokémon.\n"
    //         "It is currently under investigation.\n"
    //         "No detailed information is available\n"
    //         "at this time."),
    //     .pokemonScale = 256,
    //     .pokemonOffset = 0,
    //     .trainerScale = 256,
    //     .trainerOffset = 0,
    //     .frontPic = gMonFrontPic_CircledQuestionMark,
    //     .frontPicSize = MON_COORDS_SIZE(64, 64),
    //     .frontPicYOffset = 0,
    //     .frontAnimFrames = sAnims_SingleFramePlaceHolder,
    //     .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
    //     .backPic = gMonBackPic_CircledQuestionMark,
    //     .backPicSize = MON_COORDS_SIZE(64, 64),
    //     .backPicYOffset = 7,
    //     .backAnimId = BACK_ANIM_NONE,
    //     .palette = gMonPalette_CircledQuestionMark,
    //     .shinyPalette = gMonShinyPalette_CircledQuestionMark,
    //     .iconSprite = gMonIcon_QuestionMark,
    //     .iconPalIndex = 0,
    //     FOOTPRINT(QuestionMark)
    //     .levelUpLearnset = sBaronOfHellLevelUpLearnset,
    //     .teachableLearnset = sBaronOfHellTeachableLearnset,
    // },
};
