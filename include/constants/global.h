#ifndef GUARD_CONSTANTS_GLOBAL_H
#define GUARD_CONSTANTS_GLOBAL_H

// You can use the ENABLED_ON_RELEASE and DISABLED_ON_RELEASE macros to
// control whether a feature is enabled or disabled when making a release build.
//
// For example, the overworld debug menu is enabled by default, but when using
// `make release`, it will be automatically disabled.
//
// #define DEBUG_OVERWORLD_MENU DISABLED_ON_RELEASE
#ifdef RELEASE
#define ENABLED_ON_RELEASE TRUE
#define DISABLED_ON_RELEASE FALSE
#else
#define ENABLED_ON_RELEASE FALSE
#define DISABLED_ON_RELEASE TRUE
#endif

#include "config/ai.h"
#include "config/battle.h"
#include "config/caps.h"
#include "config/contest.h"
#include "config/debug.h"
#include "config/dexnav.h"
#include "config/follower_npc.h"
#include "config/general.h"
#include "config/item.h"
#include "config/overworld.h"
#include "config/pokemon.h"
#include "config/summary_screen.h"

// party sizes
#define PARTY_SIZE 6
#define MULTI_PARTY_SIZE (PARTY_SIZE / 2)
#define FRONTIER_PARTY_SIZE         3
#define FRONTIER_DOUBLES_PARTY_SIZE 4
#define FRONTIER_MULTI_PARTY_SIZE   2
#define MAX_FRONTIER_PARTY_SIZE    (max(FRONTIER_PARTY_SIZE,        \
                                    max(FRONTIER_DOUBLES_PARTY_SIZE,\
                                        FRONTIER_MULTI_PARTY_SIZE)))
#define UNION_ROOM_PARTY_SIZE       2

// capacities of various saveblock objects
#define DAYCARE_MON_COUNT 2
#define POKEBLOCKS_COUNT 40
#define OBJECT_EVENTS_COUNT 16
#define MAIL_COUNT (10 + PARTY_SIZE)
#define SECRET_BASES_COUNT 20
#define POKE_NEWS_COUNT 16
#define PC_ITEMS_COUNT 50
#define OBJECT_EVENT_TEMPLATES_COUNT 64
#define DECOR_MAX_SECRET_BASE 16
#define DECOR_MAX_PLAYERS_HOUSE 12
#define MAX_REMATCH_ENTRIES 100 // only REMATCH_TABLE_ENTRIES (78) are used
#define NUM_CONTEST_WINNERS 13
#define UNION_ROOM_KB_ROW_COUNT 10
#define SAVED_TRENDS_COUNT 5
#define ROAMER_COUNT 1 // Number of maximum concurrent active roamers

// Bag constants
#define BAG_ITEMS_COUNT 30
#define BAG_KEYITEMS_COUNT 30
#define BAG_POKEBALLS_COUNT 16
#define BAG_TMHM_COUNT 64
#define BAG_BERRIES_COUNT 46

#define TRAINER_ID_LENGTH 4
#define MAX_MON_MOVES 4
#define ALL_MOVES_MASK ((1 << MAX_MON_MOVES) - 1)

#define CONTESTANT_COUNT 4
#define CONTEST_CATEGORY_COOL      0
#define CONTEST_CATEGORY_BEAUTIFUL 1
#define CONTEST_CATEGORY_BEAUTY    CONTEST_CATEGORY_BEAUTIFUL
#define CONTEST_CATEGORY_CUTE      2
#define CONTEST_CATEGORY_CLEVER    3
#define CONTEST_CATEGORY_SMART     CONTEST_CATEGORY_CLEVER
#define CONTEST_CATEGORY_TOUGH     4
#define CONTEST_CATEGORIES_COUNT   5

// string lengths
#define ITEM_NAME_LENGTH 20
#define ITEM_NAME_PLURAL_LENGTH ITEM_NAME_LENGTH + 2 // 2 is used for the instance where a word's suffix becomes y->ies
#define POKEMON_NAME_LENGTH 12
#define VANILLA_POKEMON_NAME_LENGTH 10
#define POKEMON_NAME_BUFFER_SIZE max(20, POKEMON_NAME_LENGTH + 1) // Frequently used buffer size. Larger than necessary
#define PLAYER_NAME_LENGTH 7
#define MAIL_WORDS_COUNT 9
#define EASY_CHAT_BATTLE_WORDS_COUNT 6
#define MOVE_NAME_LENGTH 16
#define NUM_QUESTIONNAIRE_WORDS 4
#define QUIZ_QUESTION_LEN 9
#define TYPE_NAME_LENGTH 8
#define ABILITY_NAME_LENGTH 16
#define TRAINER_NAME_LENGTH 10
#define CODE_NAME_LENGTH 11

#define MAX_STAMP_CARD_STAMPS 7

enum
{
    MALE,
    FEMALE,
    GENDER_COUNT,
};

enum
{
    OPTIONS_BUTTON_MODE_NORMAL,
    OPTIONS_BUTTON_MODE_LR,
    OPTIONS_BUTTON_MODE_L_EQUALS_A,
};

enum
{
    OPTIONS_TEXT_SPEED_SLOW,
    OPTIONS_TEXT_SPEED_MID,
    OPTIONS_TEXT_SPEED_FAST,
    OPTIONS_TEXT_SPEED_INSTANT,
};

enum
{
    OPTIONS_SOUND_MONO,
    OPTIONS_SOUND_STEREO,
};

enum
{
    OPTIONS_BATTLE_STYLE_SHIFT,
    OPTIONS_BATTLE_STYLE_SET,
};

enum
{
    DIR_NONE,
    DIR_SOUTH,
    DIR_NORTH,
    DIR_WEST,
    DIR_EAST,
    DIR_SOUTHWEST,
    DIR_SOUTHEAST,
    DIR_NORTHWEST,
    DIR_NORTHEAST,
};

#define CARDINAL_DIRECTION_COUNT DIR_SOUTHWEST

enum
{
    AXIS_X,
    AXIS_Y,
    AXIS_COUNT,
};

enum
{
    CONNECTION_INVALID = -1,
    CONNECTION_NONE,
    CONNECTION_SOUTH,
    CONNECTION_NORTH,
    CONNECTION_WEST,
    CONNECTION_EAST,
    CONNECTION_DIVE,
    CONNECTION_EMERGE,
};

#if TESTING
#include "config/test.h"
#endif

#endif // GUARD_CONSTANTS_GLOBAL_H
