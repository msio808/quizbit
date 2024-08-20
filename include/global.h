/**
 * @file global.h
 * @brief This header file defines constant global values & function flags of the game.
**/
#ifndef GLOBAL_H
#define GLOBAL_H
#pragma once

// TODO : Flags for determining the status of the questions during a test
typedef enum QUIZ_FLAGS {
    NIL         = 0x00000000,
    SKIP        = 0x00000001,
    COMPLETED   = 0x00000001,
    INCOMPLETED = 0x00000000,
} qconst_t;

// TODO : Tracker for the time spent while playing game
typedef enum TIMETHRESHOLD {
    LSTREAK_TTHRESHOLD   = 0x0000005A,
    LOW_TIME_THRESHOLD   = 0x00000078,
    HIGH_TIME_THRESHOLD  = 0x000000B4,
} ttime_t;


// TODO : Bonus points to add to players score after each game
typedef enum BONUSPOINTS {
    BONUS_LOW   = 0x00000064,
    BONUS_MID   = 0x0000012C,
    BONUS_HIGH  = 0x000001F4,
} bonus_t;

// TODO : Flags to determine whether a player achieved/unlocked a new badges
typedef enum BADGE_STATUS_FLAG {
    LOCKED   = 0x00000000,
    UNLOCKED = 0x00000001
} bsf_t;


// TODO : Constant values for Minimum & Maximum Username & Password lengths
typedef enum PWDUTILS {
    MIN_UNAME_LEN   = 0x00000006,
    MIN_PASSWD_LEN  = 0x0000000C,
    MAX_UNAME_LEN   = 0x00000014,
    MAX_PASSWD_LEN  = 0x00000020
} pwd_t;


/**
 * @brief Error codes and flags to use with functions like : sqlite_prepare_v3 & sqlite_bind_text functions
 *      to indicate the use of entire length of statement and no use of prep flags
 *      sqlite_exec to indicate no use of callback functions & no capturing of error message
 *      'openssl evp functions' to indicate successful execution
 */
typedef enum FLAGS {
    //? Flags for sqlite3 functios
    NULL_BYTE          = -0x00000001,
    NO_ERR_MSSG        =  0x00000000,
    NO_CALLBACK        =  0x00000000,
    NO_PREP_FLAG       =  0x00000000,

    //? For openssl hashing functions
    EVP_SUCCESS        =  0x00000001,

    //? Ehh more flags
    NULL_STRING        = -0x00000002,
    VALUE_ERROR        = -0x00000001,
    VALUE_SUCCESS      =  0x00000000,
    MAX_LOGIN_ATTEMPT  =  0x00000003
} flags_t;


// TODO : Similar to the 'flags_t' type, for constants, buffer limits, etc...
typedef enum CONSTANTS {
    MIN_BUFF       = 0x00000080,
    MAX_BUFF       = 0x000000100,
    MAX_CHOICES    = 0x00000004,
    MAX_QUESTIONS  = 0x0000000A,
    INITIAL_SCORE  = 0x00000000,
    MAXIMUM_SCORE  = 0x00000064,
    LEFT_PADDING   = 0x00000007,
    RIGHT_PADDING  = 0x00000007,
    PROMPT_PADDING = 0x0000000D,
} const_t;

// TODO : Constant sizes and array buffer limits
typedef enum SIZES {
    ID_SIZE        = 0x0000000E,
    BUF_SIZE       = 0x00000040,
    KEY_SIZE       = 0x00000041, //? For Hashed password
    PASS_SIZE      = 0x00000020, //? For Password Size
    NAME_SIZE      = 0x00000020,
    DATE_SIZE      = 0x00000020,
    UNAME_SIZE     = 0x00000014,
    FRAME_SIZE     = 0x00000032, //? For the loader & progress bar
    BADGE_SIZE     = 0x00000010, //? For badges & emojis
} sizes_t;

// TODO : For string terminator, terminal control keys, etc...
typedef enum CONTROL_KEYS{
    LEFT_SQB     = 0x0000005b,  //? Left Square Bracket '['
    ESC_KEY      = 0x0000001b,  //? Escape Key
    UP_KEY       = 0x00000041,  //? 'A'
    DOWN_KEY     = 0x00000042,  //? 'B'
    LEFT_KEY     = 0x00000044,  //? 'D'
    RIGHT_KEY    = 0x00000043,  //? 'C'
    ENTER_KEY    = 0x0000000A,  //? '\n'
    BACKSPACE    = 0x0000007F,  //? '\b'
    SPACE_BAR    = 0x00000020,  //? '  '
    NULL_TERM    = '\0',        //? Null terminator
} ctrl_t;

// TODO : Constants for menu options used in switch expression
typedef enum MENUS {
    //? For Homepage & Guest Mode Options
    LOGIN              = 0x00000000,
    GUEST_PLAYER       = 0x00000000,
    SIGNUP             = 0x00000001,
    GUEST_MODE         = 0x00000002,
    HOMEPAGE           = 0x00000002,
    EXIT               = 0x00000003,

    //? For Mainmenu Options
    GAMEPLAY           = 0x00000000,
    HELP               = 0x00000001,
    SETTINGS           = 0x00000002,
    LOGOUT             = 0x00000003,

    //? For Settings Options
    LEADERBOARD        = 0x00000000,
    STATISTICS         = 0x00000001,
    MANAGEACCOUNT      = 0x00000002,

    //? For Manage Account Options
    EDITACCOUNT        = 0x00000000,
    DELETEACCOUNT      = 0x00000001,
    RESETACCOUNT       = 0x00000002,

    //? For Gameplay Options
    SCIENCE            = 0x00000000,
    SPORTS             = 0x00000001,
    GENERALKNOWLEDGE   = 0x00000002,

    //? Shared
    MAINMENU           = 0x00000003
} menu_t;

#endif //GLOBAL_H
