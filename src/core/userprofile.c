#include "../../include/gamecore.h"
#include "../../include/utilities.h"

/**
 * @brief This function initializes the stats for the demo accounts & players stats.
 *
 * @param demoAccount A pointer to the Account 'account_t' structure
 * @param defaultPlayer A pointer to the Player Stats 'pstats_t' structure
 *
 * @returns true if success otherwise false
 */
bool init_default_player_stats(account_t *demoAccount, pstats_t *defaultPlayer) {
    char *hashedPass = sha256_hashpass(demoAccount->password);
    if (hashedPass == NULL) {
        return false;
    }
    snprintf(demoAccount->password, KEY_SIZE, "%s", hashedPass);
    free(hashedPass);
    if (!init_new_player_stats(defaultPlayer, demoAccount)) {
        return false;
    }
    return true;
}


/**
 * @brief This function initializes the newly registered player stats
 *
 * @param newPlayer A pointer to the Player Stats 'pstats_t' structure
 * @param newAccount A pointer to the Account 'account_t' structure
 *
 * @returns true if functions executes successfully otherwise false
 */
bool init_new_player_stats(pstats_t *newPlayer, account_t *newAccount) {
    char *registeredDatetime = get_current_datetime();
    if (registeredDatetime == NULL) {
        log_error(__func__, __FILE__, __LINE__,
                "%s on function get_current_datetime();\n", strerror(ENOMEM));
        return false;
    }
    snprintf(newAccount->profile.date, DATE_SIZE, "%s", registeredDatetime);
    free(registeredDatetime);

    newPlayer->treasure.totalGoldCoins = NIL;
    newPlayer->treasure.totalMoneyBags = NIL;
    newPlayer->treasure.totalGemStones = NIL;

    newPlayer->badge.rocket = LOCKED;
    newPlayer->badge.shield = LOCKED;
    newPlayer->badge.trophy = LOCKED;
    newPlayer->badge.starter = LOCKED;
    newPlayer->badge.bullseye = LOCKED;
    newPlayer->badge.glowingStar = LOCKED;
    newPlayer->badge.hundredPoints = LOCKED;
    newPlayer->badge.perfectionist = LOCKED;
    
    newPlayer->scores.currentScore = INITIAL_SCORE;
    newPlayer->scores.averageScore = INITIAL_SCORE;
    newPlayer->scores.highestScore = INITIAL_SCORE;

    newPlayer->stats.longestStreak = NIL;
    newPlayer->stats.timeSpentPerGame = NIL;
    newPlayer->stats.totalGamesPlayed = NIL;

    newPlayer->stats.performanceRate = NIL;
    newPlayer->stats.quizCompletionRate = NIL;
    newPlayer->stats.totalGamesCompleted = NIL;

    newPlayer->answers.totalCorrectAnswers = NIL;
    newPlayer->answers.totalIncorrectAnswers = NIL;
    newPlayer->answers.totalQuestionAttempted = NIL;

    snprintf(newPlayer->profile.date, DATE_SIZE, "%s", newAccount->profile.date);
    snprintf(newPlayer->profile.playerId, ID_SIZE, "%s", newAccount->profile.playerId);
    snprintf(newPlayer->profile.username, NAME_SIZE, "%s", newAccount->profile.username);

    return true;
}


/**
 * @brief This function randomly generates a new player ID after a new signup.
 *
 * @param id A pointer to the 'id' array to store the generated player ID.
 */
void assign_new_player_id(char *id) {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    id[0] = 'p';
    id[1] = 'l';
    id[2] = '-';
    for (int i = 3; i < ID_SIZE - 1; ++i) {
        id[i] = '0' + rand() % 10;
    }
    id[ID_SIZE - 1] = NULL_TERM;
}