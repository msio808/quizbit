#define _XOPEN_SOURCE
#include "../../include/db.h"
#include "../../include/gamemath.h"


/**
 * @brief This function calculates the new average score of the player after every quiz game
 *
 * @param avg_score current average score of the player
 * @param new_score newly acquired score of the player
 * @param game_count Total number of games played
 *
 * @returns the newly calculated average score
 */
uint32_t get_average_score(const uint32_t avg_score, const uint32_t new_score, const uint32_t game_count) {
    if (new_score == INITIAL_SCORE) {
        return avg_score > new_score ?
            (avg_score + new_score) / game_count : INITIAL_SCORE;
    }
    return (avg_score + new_score) / game_count;
}


/**
 * @brief This function calculate the average time spent per each game played
 *
 * @param old_time The current average time
 * @param new_time The newly elapsed time after the quiz
 * @param game_count The total number of games played
 *
 * @returns The newly calculated average playtime
 */
uint32_t get_average_time(const uint32_t old_time, const double new_time, const uint32_t game_count) {
    return (old_time + new_time) / game_count;
}


/**
 * @brief This functions calculates the highest score achieved by the currently logged-in player
 *
 * @param score The current score of the player
 * @param new_score The newly achieved score of the player
 *
 * @returns The newly calculated highest score
 */
uint32_t get_highest_score(const uint32_t score, const uint32_t new_score) {
    return score > new_score ? score : new_score;
}


/**
 * @brief This function calculates the total correctly answered questions after every game
 *
 * @param score The newly acquired score of the player after the game
 *
 * @returns The total number of correct answers
 */
uint32_t get_total_correct_answers(const uint32_t score) {
    return score > INITIAL_SCORE ? score / MAXIMUM_SCORE : INITIAL_SCORE;
}


/**
 * @brief This function calculates the total number of incorrectly answered questions
 *
 * @param score newly acquired score of the player after the quiz
 *
 * @returns The total number of incorrect answers
 */
uint32_t get_total_incorrect_answers(const uint32_t score) {
    return score > INITIAL_SCORE ? MAX_QUESTIONS - score / MAXIMUM_SCORE : MAX_QUESTIONS;
}


/**
 * @brief This function counts the total number of questions that were answered by the player
 *
 * @param skipped The number of questions that were skipped
 *
 * @returns The total number of question attempted
 */
uint32_t get_total_question_attempted(const uint32_t skipped) {
    return MAX_QUESTIONS - skipped;
}


/**
 * @brief This function calculates the percentage of the score after the quiz game
 *
 * @param p The percent value of the score to calculate
 * @param n The hoghest achievable score of the game
 *
 * @returns The 'P' percentage of 'N'
 */
double get_percentage(const double p, const double n) {
    return p / MAXIMUM_SCORE * n;
}


/**
 * @brief This function calculates the total number of games completed bby the player
 *
 * @param skipped The number of questions that were skipped during the game
 *
 * @returns the newly calculated number of games completed
 */
uint32_t games_completed(const uint32_t skipped) {
    return skipped == NIL ? COMPLETED : INCOMPLETED;
}


/**
 * @brief This function calculates the performance rate of the player after every game
 *
 * @param c_ans number of correct answers
 * @param g_played number of games played
 *
 * @returns the performance rate of the player
 */
double get_performance_rate(const uint32_t c_ans, const uint32_t g_played) {
    return (double)c_ans / (double)(MAX_QUESTIONS * g_played) * 100.00;
}


/**
 * @brief This function calculated the quiz completion rate of the player
 *
 * @param q_attmpt Total number of questions attempted by the player
 * @param g_played Total number of games played/attempted
 *
 * @returns the rate of completion of the player
 */
double get_completion_rate(const uint32_t q_attmpt, const uint32_t g_played) {
    return (double)q_attmpt / (double)(MAX_QUESTIONS * g_played) * 100.00;
}


/**
 * @brief This function is used to update the longes streak of the player which is
 *        updated when a player achieves 90% or more in a game
 *
 * @param new_score The newly acquired score of the player after the game
 * @param old_streak The current streak value of the player
 *
 * @returns 1 if player achieves a score >= 90 else returns the current streak
 */
uint32_t get_lstreaks(const uint32_t new_score, const uint32_t old_streak) {
    const uint32_t new_streak = MAXIMUM_SCORE * (MAX_QUESTIONS - 1);
    if (new_score >= new_streak) {
        return 1;
    }
    return old_streak;
}


/**
 * @brief This function calls all the above function and update the player stats
 *
 * @param player A pointer to the player stats structure of the currently logged-in player
 * @param score Current score of the player
 * @param bonus Bonus score acquired based on the performance of the player
 * @param new_score Newly acquired score of the player
 * @param skipped The number of questions that were skipped during the quiz game
 * @param __time_ The elapsed time if the quiz game
 */
void getnew_playerstats(pstats_t *player, const uint32_t score, const uint32_t bonus, const uint32_t new_score, const uint32_t skipped, const double __time_) {
    player->stats.totalGamesPlayed += 1;
    player->answers.totalQuestionAttempted += get_total_question_attempted(skipped);

    const uint32_t tmp_new_score = bonus + new_score;
    player->scores.highestScore = get_highest_score(score, tmp_new_score);
    const uint32_t tmp_avg = player->scores.averageScore;
    player->scores.averageScore = get_average_score(tmp_avg, tmp_new_score, player->stats.totalGamesPlayed);

    player->answers.totalCorrectAnswers += get_total_correct_answers(new_score);
    player->answers.totalIncorrectAnswers += get_total_incorrect_answers(new_score);

    const uint32_t tmp = get_lstreaks(new_score, player->stats.longestStreak);
    player->stats.longestStreak = tmp == 1 ? player->stats.longestStreak + tmp : NIL;

    player->stats.timeSpentPerGame =
        get_average_time(player->stats.timeSpentPerGame, __time_, player->stats.totalGamesPlayed);

    player->stats.totalGamesCompleted += games_completed(skipped);
    player->stats.performanceRate =
        (uint32_t)get_performance_rate(player->answers.totalCorrectAnswers, player->stats.totalGamesPlayed);
    player->stats.quizCompletionRate =
        (uint32_t)get_completion_rate(player->answers.totalQuestionAttempted, player->stats.totalGamesPlayed);
}


/**
 * @brief This function rewards players with badges based on their player stats
 *
 * @param player A pointer to the currently logged in player stats structure
 * @param score Newly acquired score of the player including the bonus
 * @param time Elapsed time of the quiz
 */
void unlock_achievements(pstats_t *player, const uint32_t score, const double time) {
    if (player->stats.totalGamesPlayed >= 5) {
        player->badge.starter = UNLOCKED;
    }

    const uint32_t percentage = (uint32_t)get_percentage(90, MAX_QUESTIONS * MAXIMUM_SCORE);
    if (score >= percentage) {
        if (player->stats.longestStreak >= 25) {
            player->badge.glowingStar = UNLOCKED;

            player->treasure.totalMoneyBags += 10;
            player->treasure.totalGoldCoins += 10;
            player->treasure.totalGemStones += 1;
        }
    }

    if (score == MAX_QUESTIONS * MAXIMUM_SCORE) {
        player->badge.hundredPoints = UNLOCKED;

        if (time < LSTREAK_TTHRESHOLD) {
            player->badge.rocket = UNLOCKED;
            player->treasure.totalMoneyBags += 5;
            player->treasure.totalGoldCoins += 10;
        }

        if (player->stats.longestStreak == 10) {
            player->badge.bullseye = UNLOCKED;
            player->treasure.totalMoneyBags += 3;
        }

        player->treasure.totalMoneyBags += 1;
        player->treasure.totalGoldCoins += 1;
        player->treasure.totalGemStones += 1;
    }

    if (player->stats.longestStreak >= 50) {
        char *__new_time = get_current_datetime();
        const uint32_t sec = (uint32_t)get_time_difference(player->profile.date, __new_time);

        const uint32_t day = sec / (24 * 3600);

        if (day <= 1) {
            player->badge.shield = UNLOCKED;
            player->treasure.totalMoneyBags += 50;
            player->treasure.totalGoldCoins += 25;
            player->treasure.totalGemStones += 25;
        }
        free(__new_time);
    }

    if (player->badge.rocket && player->badge.shield &&
        player->badge.trophy && player->badge.starter &&
        player->badge.bullseye && player->badge.glowingStar &&
        player->badge.hundredPoints && player->badge.perfectionist &&
        player->stats.longestStreak > 100 && player->treasure.totalGemStones > 100 &&
        player->treasure.totalGoldCoins > 100 && player->treasure.totalMoneyBags > 100) {
            player->badge.perfectionist = UNLOCKED;
    }
}


/**
 * @brief This function calculates the time difference between the
 *        previously played date & time and the most recent played date & time
 *
 * @param old_time The previous played date & time
 * @param new_time The most recent played date & time
 *
 * @returns The difference in seconds
 */
double get_time_difference(const char *old_time, const char *new_time) {
    struct tm old_tm = {0};
    struct tm new_tm = {0};

    strptime(old_time, "%d-%b-%Y %I:%M %p", &old_tm);
    strptime(new_time, "%d-%b-%Y %I:%M %p", &new_tm);

    const time_t old = mktime(&old_tm);
    const time_t new = mktime(&new_tm);

    return difftime(new, old);
}


/**
 * @brief This function assigns bonus to players after the quiz challenge have been completed
 *        It only assign bonus to players that have correctly answered atleast half of the questions
 *
 * @param score A variable holding the score of the player
 * @param time A variable holding the elapsed time during the game
 *
 * @returns The bonus acquired
 */
uint32_t assign_bonus(const uint32_t score, const double time) {
    if (score < MAXIMUM_SCORE * MAX_QUESTIONS / 2) {
        return NIL;
    }

    //? Assign bonus to players that correctly answered atleast half of the questions
    const uint32_t bonus = score / MAX_QUESTIONS;
    if (time > HIGH_TIME_THRESHOLD) {
        return bonus + BONUS_LOW;
    }
    if (time <= LOW_TIME_THRESHOLD) {
        if (score == MAX_QUESTIONS * MAXIMUM_SCORE) {
            return (bonus + BONUS_HIGH) * 2; //? twice the bonus for players that scored 100% within 120 seconds
        }
        return bonus + BONUS_HIGH;
    }
    return bonus + BONUS_MID;
}
