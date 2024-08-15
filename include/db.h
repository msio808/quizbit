/**
 * @file db.h
 * @author Muhammed A. Senghore
 * @brief This header file defines function prototypes & structures related to database operations used in the Game.
**/
#ifndef DB_H
#define DB_H
#pragma once

#include "global.h"
#include "gamecore.h"

#define DBFILEPATH "../data/database.db"

// TODO : Structfor holding the question & answers in the Quiz Game.
typedef struct QUIZ {
    int question_id;
    char question[MAX_BUFF];
    char correct_choice[BUF_SIZE];
    char selected_choice[BUF_SIZE];
    char choices[MAX_CHOICES][BUF_SIZE];
} quiz_t;


bool setup(void);
bool create_new_database(void);
bool insert_quiz_questions_answers(sqlite3 *);
bool get_player_data(const char *, pstats_t *);
bool reset_badges_table(sqlite3 *, const char *);
bool reset_players_table(sqlite3 *, const char *);

bool retrieve_badges_table(sqlite3 *, pstats_t *, const char *);
bool retrieve_players_table(sqlite3 *, pstats_t *, const char *);
bool retrieve_accounts_table(sqlite3 *, account_t *, const char *);

bool update_gamestats(const pstats_t *);
bool update_score(pstats_t *, uint32_t, uint32_t, uint32_t);
bool insert_new_player_data(const account_t *, const pstats_t *);
bool delete_player_data(sqlite3 *, const pstats_t *, const char *);
bool insert_quiz_data(sqlite3 *, const quiz_t *, size_t, const char *);
bool update_player_data(sqlite3 *, const char *, const char *, const char *, const char *);

uint32_t check_answer(quiz_t *, short);
void display_answered(int, pstats_t *, quiz_t *);
void display_questions(int, pstats_t *, const char *);
bool get_questions(sqlite3 *, quiz_t *, const char *, size_t);

#endif //DB_H
