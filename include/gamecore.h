/**
 * @file gamecore.h
 * @author my-self
 * @brief This header file defines the structures and function prototypes necessary
 *        for handling player stats, account details, and some game-related information.
**/
#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H
#pragma once

#include "global.h"
#include "utilities.h"
#include "gamemath.h"

// TODO : struct to hold player treasures
typedef struct TREASURES {
    uint32_t totalGoldCoins;
    uint32_t totalMoneyBags;
    uint32_t totalGemStones;
} treasures_t;

// TODO : struct to hold the game score stats
typedef struct SCORE_STATS {
    uint32_t currentScore;
    uint32_t averageScore;
    uint32_t highestScore;
} points_t;

// TODO : struct to hold the gameplay stats
typedef struct GAMEPLAY_STATS {
    uint32_t longestStreak;
    uint32_t timeSpentPerGame;
    uint32_t totalGamesPlayed;

    uint32_t performanceRate;
    uint32_t quizCompletionRate;
    uint32_t totalGamesCompleted;
} gstats_t;

// TODO : struct to hold quiz answer stats
typedef struct ANSWER_STATS{
    uint32_t totalCorrectAnswers;
    uint32_t totalIncorrectAnswers;
    uint32_t totalQuestionAttempted;
} qstats_t;

// TODO : struct to hold player login/account details
typedef struct PLAYER_INFO {
    char date[DATE_SIZE];
    char playerId[ID_SIZE];
    char username[UNAME_SIZE];
} player_t;

// TODO : struct to hold player account details
typedef struct ACCOUNTS {
    char    name[NAME_SIZE];
    char    surname[NAME_SIZE];
    char    password[KEY_SIZE];
    player_t profile;
} account_t;

// TODO : struct to hold player badge info
typedef struct BADGES {
    bool rocket;
    bool shield;
    bool trophy;
    bool starter;
    bool bullseye;
    bool glowingStar;
    bool hundredPoints;
    bool perfectionist;
} badges_t ;


// TODO : struct to hold the current player & game statistics
typedef struct PLAYERSTATS {
    gstats_t stats;
    badges_t badge;
    points_t scores;
    qstats_t answers;
    player_t profile;
    treasures_t treasure;
} pstats_t ;


void login(int);
void signup(int);
void logout(int, void *);
void homepage(int);
void mainmenu(int, pstats_t *);
void settings(int, pstats_t *);
void gameplay(int, pstats_t *);
void play_as_guest(int);
void guestplayer_mode(int);

void reset_score(int, pstats_t *);
void edit_account(int, pstats_t *);
void delete_account(int, pstats_t *);
void manage_account(int, pstats_t *);
void assign_new_player_id(char *);
void display_gamestats(int, pstats_t *);
void display_scoreboard(int , pstats_t *);
void reset_account_data(int, pstats_t *);
void modify_account(int, int, pstats_t *);

bool get_new_user_info(int, int, account_t *);
bool init_default_player_stats(account_t *, pstats_t *);
bool init_new_player_stats(pstats_t *, account_t *);
ssize_t edit_field(int, int, const char *, char *, size_t);

void unlock_achievements(pstats_t *, uint32_t, double);
void getnew_playerstats(pstats_t *, uint32_t, uint32_t, uint32_t, uint32_t, double);

#endif //PLAYERSTATS_H
