#ifndef GUARD_CONSTANTS_POKEDEX_H
#define GUARD_CONSTANTS_POKEDEX_H

// National Pokédex order
// These constants are NOT disabled by P_GEN_X_POKEMON to keep pokedex_orders.h clean.
enum NationalDexOrder
{
    NATIONAL_DEX_NONE,
    NATIONAL_DEX_DAPHNE_BLAKE,
    NATIONAL_DEX_SCARAB,
    NATIONAL_DEX_KHOPESH,
    NATIONAL_DEX_HORUS,
};

#define KANTO_DEX_COUNT     NATIONAL_DEX_MEW
#define JOHTO_DEX_COUNT     NATIONAL_DEX_CELEBI

#define NATIONAL_DEX_COUNT  NATIONAL_DEX_HORUS
#define POKEMON_SLOTS_NUMBER (NATIONAL_DEX_COUNT + 1)

// Hoenn Pokédex order
enum HoennDexOrder
{
    HOENN_DEX_NONE,
    HOENN_DEX_DAPHNE_BLAKE,
    HOENN_DEX_SCARAB,
    HOENN_DEX_KHOPESH,
    HOENN_DEX_HORUS,
};

#define HOENN_DEX_COUNT (HOENN_DEX_HORUS + 1)

#define DECAGRAMS_IN_POUND             4536
#define CM_PER_INCH                    2.54
#define CM_PER_INCH_FACTOR             (CM_PER_INCH * 100)
#define INCHES_IN_FOOT                 12
#define INCHES_IN_ONE_AND_HALF_FOOT    (INCHES_IN_FOOT * 1.5)
#define INCHES_IN_FOOT_FACTOR          (INCHES_IN_FOOT * 10)

#define WEIGHT_HEIGHT_STR_LEN          16
#define WEIGHT_HEIGHT_STR_MEM          (WEIGHT_HEIGHT_STR_LEN * sizeof(u8))

#define DEX_HEADER_X                   96
#define DEX_Y_TOP                      57
#define DEX_Y_BOTTOM                   73
#define DEX_MEASUREMENT_X              129

#define DEX_HGSS_HEADER_X_PADDING      59
#define DEX_HGSS_Y_TOP_PADDING         7
#define DEX_HGSS_Y_BOTTOM_PADDING      4
#define DEX_HGSS_MEASUREMENT_X_PADDING 51

enum
{
    DEX_MODE_HOENN,
    DEX_MODE_NATIONAL
};

enum
{
    FLAG_GET_SEEN,
    FLAG_GET_CAUGHT,
    FLAG_SET_SEEN,
    FLAG_SET_CAUGHT
};

#endif // GUARD_CONSTANTS_POKEDEX_H
