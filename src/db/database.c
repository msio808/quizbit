#include "../../include/db.h"
#include "../../include/gamecore.h"


/**
 * @brief This function creates a new database for the game and initializes necessary tables.
 *        The function creates 5 tables : 'users' to store user details after signing up,
 *        'players' and 'badges' table to keep track of player records,
 *        and three additional tables to store questions from three different categories,
 *        along with their answers, and correct choice.
 *
 * @returns true if the database and it's tables are successflly created, otherwise false.
 */
bool create_new_database(void) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        fprintf(stderr, " [%s] Error opening db : errMsg => %s!...\n", WARNING_EMOJI, sqlite3_errmsg(db));
        return false;
    }

    char *createtablesQUERY[] = {
        "CREATE TABLE IF NOT EXISTS accounts ("
                        "player_id TEXT NOT NULL UNIQUE PRIMARY KEY,"
                        "name TEXT NOT NULL,"
                        "surname TEXT NOT NULL,"
                        "username TEXT NOT NULL UNIQUE,"
                        "password TEXT NOT NULL,"
                        "registration_date TEXT NOT NULL"
        ");",
        "CREATE TABLE IF NOT EXISTS players ("
                        "player_id TEXT NOT NULL UNIQUE PRIMARY KEY,"
                        "username TEXT NOT NULL UNIQUE,"

                        "goldcoin INTEGER NOT NULL,"
                        "moneybag INTEGER NOT NULL,"
                        "gemstone INTEGER NOT NULL,"

                        "current_score INTEGER NOT NULL,"
                        "average_score INTEGER NOT NULL,"
                        "highest_score INTEGER NOT NULL,"

                        "longest_streak INTEGER NOT NULL,"
                        "average_playtime INTEGER NOT NULL,"
                        "total_games_played INTEGER NOT NULL,"

                        "performanceRate INTEGER NOT NULL,"
                        "quizCompletionRate INTEGER NOT NULL,"
                        "totalGamesCompleted INTEGER NOT NULL,"

                        "correct_answers INTEGER NOT NULL,"
                        "incorrect_answers INTEGER NOT NULL,"
                        "total_attempts INTEGER NOT NULL,"

                        "last_played_timestamp TEXT NOT NULL,"
                        "FOREIGN KEY (player_id) REFERENCES accounts(player_id)"
        ");",
        "CREATE TABLE IF NOT EXISTS badges ("
                        "player_id TEXT NOT NULL UNIQUE PRIMARY KEY,"
                        "username TEXT NOT NULL UNIQUE,"

                        "rocket INTEGER NOT NULL,"
                        "shield INTEGER NOT NULL,"
                        "trophy INTEGER NOT NULL,"
                        "starter INTEGER NOT NULL,"
                        "bulls_eye INTEGER NOT NULL,"
                        "glowing_star INTEGER NOT NULL,"
                        "hundred_points INTEGER NOT NULL,"
                        "perfectionist INTEGER NOT NULL,"

                        "FOREIGN KEY (player_id) REFERENCES accounts(player_id)"
        ");",

            "CREATE TABLE IF NOT EXISTS Science ("
                        "question_id INTEGER UNIQUE PRIMARY KEY,"
                        "question TEXT NOT NULL,"
                        "choice_a TEXT NOT NULL,"
                        "choice_b TEXT NOT NULL,"
                        "choice_c TEXT NOT NULL,"
                        "choice_d TEXT NOT NULL,"
                        "correct_choice TEXT NOT NULL"
            ");",

            "CREATE TABLE IF NOT EXISTS Sports ("
                        "question_id INTEGER UNIQUE PRIMARY KEY,"
                        "question TEXT NOT NULL,"
                        "choice_a TEXT NOT NULL,"
                        "choice_b TEXT NOT NULL,"
                        "choice_c TEXT NOT NULL,"
                        "choice_d TEXT NOT NULL,"
                        "correct_choice TEXT NOT NULL"
            ");",

            "CREATE TABLE IF NOT EXISTS Basics ("
                        "question_id INTEGER UNIQUE PRIMARY KEY,"
                        "question TEXT NOT NULL,"
                        "choice_a TEXT NOT NULL,"
                        "choice_b TEXT NOT NULL,"
                        "choice_c TEXT NOT NULL,"
                        "choice_d TEXT NOT NULL,"
                        "correct_choice TEXT NOT NULL"
            ");"
    };

    char *errMsg = NULL;
    //? Execute the SQL commands & display error message if execution fails
    const size_t size = sizeof(createtablesQUERY) / sizeof(createtablesQUERY[0]);
    for (size_t i = 0; i < size; i++) {
        if (sqlite3_exec(db, createtablesQUERY[i], NO_CALLBACK, NO_ERR_MSSG, &errMsg) != SQLITE_OK) {
            fprintf(stderr, " [%s] Error executing query : errMsg => %s!...\n", WARNING_EMOJI, errMsg);
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }
    }
    sqlite3_close(db);
    return true;
}


/**
 * @brief This function inserts new user and player data into the 'users', 'players', & 'badges' table of the game db
 *
 * @param user A pointer to the user account structure.
 * @param player A pointer to the player stats structure.
 *
 * @returns true if the data is successfully inserted to the database, otherwise false.
 */
bool insert_new_player_data(const account_t *user, const pstats_t *player) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to open db : %s!...\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_stmt *stmt = NULL;
    //? Prepare & Bind SQL statement to insert data in to the 'accounts' table in the database
    const char *insertUserQUERY = "INSERT INTO accounts (player_id, name, surname, username, password, registration_date) VALUES (?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v3(db, insertUserQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, user->profile.playerId, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, user->name, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, user->surname, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 4, user->profile.username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 5, user->password, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 6, user->profile.date, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to bind parameters : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
        }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);

    //? Prepare & Bind SQL statement to insert data in to the 'players' table in the database
    const char *insertPlayerQUERY = "INSERT INTO players ("
                                    "player_id, username, goldcoin, moneybag, gemstone,"
                                    "current_score, average_score, highest_score, "
                                    "longest_streak, average_playtime, total_games_played, "
                                    "performanceRate, quizCompletionRate, totalGamesCompleted, "
                                    "correct_answers, incorrect_answers, total_attempts, last_played_timestamp) "
                                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v3(db, insertPlayerQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, player->profile.playerId, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, player->profile.username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 3, player->treasure.totalGoldCoins) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 4, player->treasure.totalMoneyBags) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 5, player->treasure.totalGemStones) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 6, player->scores.currentScore) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 7, player->scores.averageScore) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 8, player->scores.highestScore) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 9, player->stats.longestStreak) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 10, player->stats.timeSpentPerGame) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 11, player->stats.totalGamesPlayed) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 12, player->stats.performanceRate) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 13, player->stats.quizCompletionRate) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 14, player->stats.totalGamesCompleted) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 15, player->answers.totalCorrectAnswers) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 16, player->answers.totalIncorrectAnswers) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 17, player->answers.totalQuestionAttempted) != SQLITE_OK ||
         sqlite3_bind_text(stmt, 18, player->profile.date, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
            log_error(__func__, __FILE__, __LINE__, "Failed to bind parameters : %s!...\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);

    const char *insertBadgesQUERY = "INSERT INTO badges (player_id, username, "
                                    "rocket, shield, trophy, starter, "
                                    "bulls_eye, glowing_star, hundred_points, perfectionist"
                                    ") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v3(db, insertBadgesQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, player->profile.playerId, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, player->profile.username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 3, player->badge.rocket) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 4, player->badge.shield) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 5, player->badge.trophy) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 6, player->badge.starter) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 7, player->badge.bullseye) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 8, player->badge.glowingStar) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 9, player->badge.hundredPoints) != SQLITE_OK ||
         sqlite3_bind_int(stmt, 10, player->badge.perfectionist) != SQLITE_OK) {
            log_error(__func__, __FILE__, __LINE__, "Failed to bind parameters : %s!...\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}


/**
 * @brief This function retrieves the currently logged-in players name & surname to display in the 'stats' page
 *
 * @param db A pointer to the sqlite3 db connection
 * @param player A pointer to the account structure
 * @param username A pointer to the username of the account to retrieve
 *
 * @returns True if data is successfully retrieved, otherwise false
 */
bool retrieve_accounts_table(sqlite3 *db, account_t *player, const char *username) {
    const char *query = "SELECT name, surname FROM accounts WHERE username = ?";
    sqlite3_stmt *stmt;


    int rc = sqlite3_prepare_v3(db, query, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL);
    if (rc != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    rc = sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        snprintf(player->name, NAME_SIZE, "%s", (const char *)sqlite3_column_text(stmt, 0));
        snprintf(player->surname, NAME_SIZE, "%s", (const char *)sqlite3_column_text(stmt, 1));
    }

    if (rc != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}


/**
 * @brief This functions retrieves stats of the currently logged-in player from the 'players' table
 *
 * @param db Pointer to the sqlite3 db connection
 * @param playerData A Pointer to the currently logged in player stats
 * @param username A pointer to the username of the currently logged-in player
 *
 * @returns true if data is successfully retrieved, otherwise false
 */
bool retrieve_players_table(sqlite3 *db, pstats_t *playerData, const char *username) {
    char playerStatsQUERY[MAX_BUFF * 2];
    snprintf(playerStatsQUERY, sizeof(playerStatsQUERY),
        "SELECT player_id, username, goldcoin, moneybag, gemstone, current_score, average_score,"
              " highest_score, longest_streak, average_playtime, total_games_played,"
              "performanceRate, quizCompletionRate, totalGamesCompleted, correct_answers,"
              "incorrect_answers, total_attempts, last_played_timestamp FROM players WHERE username = ?;");

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, playerStatsQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to bind parameter : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    const int step = sqlite3_step(stmt);
    //? Retrieve data from the result set and store it in the account_t structure
    if (step == SQLITE_ROW) {
        snprintf(playerData->profile.playerId, ID_SIZE, "%s", (const char *)sqlite3_column_text(stmt, 0));
        snprintf(playerData->profile.username, UNAME_SIZE, "%s", (const char *)sqlite3_column_text(stmt, 1));
        playerData->treasure.totalGoldCoins = sqlite3_column_int(stmt, 2);
        playerData->treasure.totalMoneyBags = sqlite3_column_int(stmt, 3);
        playerData->treasure.totalGemStones = sqlite3_column_int(stmt, 4);

        playerData->scores.currentScore = sqlite3_column_int(stmt, 5);
        playerData->scores.averageScore = sqlite3_column_int(stmt, 6);
        playerData->scores.highestScore = sqlite3_column_int(stmt, 7);

        playerData->stats.longestStreak = sqlite3_column_int(stmt, 8);
        playerData->stats.timeSpentPerGame = sqlite3_column_int(stmt, 9);
        playerData->stats.totalGamesPlayed = sqlite3_column_int(stmt, 10);

        playerData->stats.performanceRate = sqlite3_column_int(stmt, 11);
        playerData->stats.quizCompletionRate = sqlite3_column_int(stmt, 12);
        playerData->stats.totalGamesCompleted = sqlite3_column_int(stmt, 13);

        playerData->answers.totalCorrectAnswers = sqlite3_column_int(stmt, 14);
        playerData->answers.totalIncorrectAnswers = sqlite3_column_int(stmt, 15);
        playerData->answers.totalQuestionAttempted = sqlite3_column_int(stmt, 16);
        snprintf(playerData->profile.date, DATE_SIZE, "%s", (const char *)sqlite3_column_text(stmt, 17));
    }
    else if (step == SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "No data found for username : {%s} ERR : %s\n", username, sqlite3_errmsg(db));
    }
    else {
        log_error(__func__, __FILE__, __LINE__, "No rows found : %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return step == SQLITE_ROW;
}


/**
 * @brief This functions retrieves data of the currently logged-in player from the 'badges' table
 *
 * @param db Pointer to the sqlite3 db connection
 * @param playerData A Pointer to the currently logged in player stats
 * @param username A pointer to the username of the currently logged-in player
 *
 * @returns true if data is successfully retrieved, otherwise false
 */
bool retrieve_badges_table(sqlite3 *db, pstats_t *playerData, const char *username) {
    char badgessQUERY[MAX_BUFF * 2];
    snprintf(badgessQUERY, sizeof(badgessQUERY),
        "SELECT player_id, username, rocket, shield, trophy, starter, "
              "bulls_eye, glowing_star, hundred_points, perfectionist FROM badges WHERE username = ?;");

    sqlite3_stmt *stmt = NULL;
    //? Prepare and execute the query, bind the username parameter, and execute the statement
    if (sqlite3_prepare_v3(db, badgessQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s\n", sqlite3_errmsg(db));
        return false;
    }
    if (sqlite3_bind_text(stmt, 1, username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to bind parameter : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    const int step = sqlite3_step(stmt);

    if (step == SQLITE_ROW) {
        //? Skip first two columns since it has been retrieved from the players table
        playerData->badge.rocket = sqlite3_column_int(stmt, 2);
        playerData->badge.shield = sqlite3_column_int(stmt, 3);
        playerData->badge.trophy = sqlite3_column_int(stmt, 4);
        playerData->badge.starter = sqlite3_column_int(stmt, 5);

        playerData->badge.bullseye = sqlite3_column_int(stmt, 6);
        playerData->badge.glowingStar = sqlite3_column_int(stmt, 7);
        playerData->badge.hundredPoints = sqlite3_column_int(stmt, 8);
        playerData->badge.perfectionist = sqlite3_column_int(stmt, 9);
    }
    else if (step == SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "No data found for username : {%s}\n", username);
    }
    else {
        log_error(__func__, __FILE__, __LINE__, "No rows found : %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return step == SQLITE_ROW;
}


/**
 * @brief This function retrieves updated information of currently logged in player
 *        from the database and updates the player stats structure members accordingly.
 *        It is usually called after a new signup, login, gameplay, or after account is editted
 *
 * @param username A pointer to the username of the player whose information needs to be retrieved.
 * @param player A pointer to the user player stats structure to store the retrieved player information.
 *
 * @return true if the new/updated data is successfully retrieved, otherwise false.
 */
bool get_player_data(const char *username, pstats_t *player) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to open database : %s!...\n", sqlite3_errmsg(db));
        return false;
    }

    bool ret = true;
    if (!retrieve_players_table(db, player, username)) {
        log_error(__func__, __FILE__, __LINE__, "Failed to retrieve from 'players' table : %s!...\n", sqlite3_errmsg(db));
        ret = false;
    }

    if (!retrieve_badges_table(db, player, username)) {
        log_error(__func__, __FILE__, __LINE__, "Failed to retrieve from 'badges' table : %s!...\n", sqlite3_errmsg(db));
        ret = false;
    }

    sqlite3_close(db);
    return ret;
}


/**
 * @brief This function updates a specific column on a given table in the database.
 *
 * @param db A pointer to the SQLite database connection.
 * @param col A pointer to the name of the column to be updated.
 * @param uname A pointer to the username of the currently logged-in player.
 * @param newV A pointer to the new value to replace the old value in the 'col' of the logged-in player.
 * @param tableName A pointer to the name of the table to update.
 *
 * @returns true if the player data is successfully updataded in the database, otherwise false.
 */
bool update_player_data(sqlite3 *db, const char *col, const char *uname, const char *newV, const char *tableName) {
    if (db == NULL || tableName == NULL || col == NULL || uname == NULL || newV == NULL) {
        log_error(__func__, __FILE__, __LINE__, "Invalid input parameters!...\n");
        return false;
    }

    const char *sqlTemplate = "UPDATE %s SET %s = ? WHERE username = ?;";
    const size_t querySize = strlen(sqlTemplate) + strlen(tableName) + strlen(col) * 2 + 1;

    char sqlQUERY[querySize];
    snprintf(sqlQUERY, querySize, sqlTemplate, tableName, col, col);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, sqlQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        return false;
    }
    if (sqlite3_bind_text(stmt, 1, newV, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK||
        sqlite3_bind_text(stmt, 2, uname, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
            log_error(__func__, __FILE__, __LINE__, "Failed to bind values : %s!...\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}



/**
 * @brief This function deletes player data from the database in the specified table.
 *
 * @param db The SQLite database connection.
 * @param player A pointer to the player stats structure.
 * @param tableName The name of the table to delete data from.
 *
 * @returns true if the data is successfully deleted from the db, otherwise false.
 */
bool delete_player_data(sqlite3 *db, const pstats_t *player, const char *tableName) {
    if (NULL == db || tableName == NULL || player == NULL) {
        log_error(__func__, __FILE__, __LINE__, "Invalid input parameters!...\n");
        return false;
    }

    char sqlQUERY[MAX_BUFF];
    const char *sqlTemplate = "DELETE FROM %s WHERE username = ?;";
    snprintf(sqlQUERY, sizeof(sqlQUERY), sqlTemplate, tableName);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, sqlQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        return false;
    }
    if (sqlite3_bind_text(stmt, 1, player->profile.username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to bind values : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}


/**
 * @brief This function updates the currently logged in player's score in the database.
 *
 * @param player A pointer to the user account structure representing the current player.
 * @param currentScore The new current score to update in the database.
 * @param averageScore The new average score to update in the database.
 * @param highestScore The new highest score to update in the database.
 *
 * @returns true if the score is successfully updated, otherwise false.
 */
bool update_score(pstats_t *player, const uint32_t currentScore, const uint32_t averageScore, const uint32_t highestScore) {
    char query[MAX_BUFF];
    sqlite3_stmt *stmt = NULL;
    snprintf(query, sizeof(query),
             "UPDATE players SET current_score = ?, average_score = ?, highest_score = ?, last_played_timestamp = ? WHERE username = ?;");

    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to open database : %s!...\n", sqlite3_errmsg(db));
        return false;
    }

    char *lastUpdateDatetime = get_current_datetime();
    if (lastUpdateDatetime == NULL) {
        log_error(__func__, __FILE__, __LINE__, "%s to 'time_t' struct!...\n", strerror(ENOMEM));
        sqlite3_close(db);
        return false;
    }
    snprintf(player->profile.date, DATE_SIZE, "%s", lastUpdateDatetime);
    free(lastUpdateDatetime);

    //? Prepare, execute, then finalize the statement
    if (sqlite3_prepare_v3(db, query, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, currentScore) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, averageScore) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, highestScore) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 4, player->profile.date, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 5, player->profile.username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
            log_error(__func__, __FILE__, __LINE__, "Failed to bind parameters : %s!...\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to updating score : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}


/**
 * @brief This function updates the currently logged in player's stats in the database,
 *        It is called after every played game.
 *
 * @param player A pointer to the user account structure representing the current player.
 *
 * @returns true if the score is successfully updated, otherwise false.
 */
bool update_gamestats(const pstats_t *player) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to open database : %s!...\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_stmt *stmt = NULL;
    char query[MAX_BUFF * 2];
    snprintf(query, sizeof(query),
             "UPDATE players SET goldcoin = ?, moneybag = ?, gemstone = ?, "
             "longest_streak = ?, average_playtime = ?, total_games_played = ?, "
             "performanceRate = ?, quizCompletionRate = ?, totalGamesCompleted = ?, "
             "correct_answers = ?, incorrect_answers = ?, total_attempts = ? WHERE username = ? ");

    if (sqlite3_prepare_v3(db, query, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, player->treasure.totalGoldCoins) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, player->treasure.totalMoneyBags) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, player->treasure.totalGemStones) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 4, player->stats.longestStreak) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 5, player->stats.timeSpentPerGame) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 6, player->stats.totalGamesPlayed) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 7, player->stats.performanceRate) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 8, player->stats.quizCompletionRate) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 9, player->stats.totalGamesCompleted) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 10, player->answers.totalCorrectAnswers) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 11, player->answers.totalIncorrectAnswers) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 12, player->answers.totalQuestionAttempted) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 13, player->profile.username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
            log_error(__func__, __FILE__, __LINE__, "Failed to bind parameters : %s!...\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to update player stats : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);

    snprintf(query, sizeof(query),
        "UPDATE badges SET "
        "rocket = ?, shield = ?, trophy = ?, starter = ?, bulls_eye = ?, "
        "glowing_star = ?, hundred_points = ?, perfectionist = ? WHERE username = ?;");

    if (sqlite3_prepare_v3(db, query, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, player->badge.rocket) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, player->badge.shield) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, player->badge.trophy) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 4, player->badge.starter) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 5, player->badge.bullseye) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 6, player->badge.glowingStar) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 7, player->badge.hundredPoints) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 8, player->badge.perfectionist) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 9, player->profile.username, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
            log_error(__func__, __FILE__, __LINE__, "Failed to bind parameters : %s!...\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to update player stats : %s!...\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return true;
}

/**
 * @brief This function clears/resets all the player stats in the 'players' table
 *
 * @param db A pointer to the SQLite database connection
 * @param uname A pointer to the username of the currently logged in player
 *
 * @returns true if data reset is successful otherwise false
 */
bool reset_players_table(sqlite3 *db, const char *uname) {
    const char *sqlQUERY = "UPDATE players SET "
                           "goldcoin = 0, "
                           "moneybag = 0, "
                           "gemstone = 0, "
                           "current_score = 0, "
                           "average_score = 0, "
                           "highest_score = 0, "
                           "longest_streak = 0, "
                           "average_playtime = 0, "
                           "total_games_played = 0, "
                           "performanceRate = 0, "
                           "quizCompletionRate = 0, "
                           "totalGamesCompleted = 0,"
                           "correct_answers = 0, "
                           "incorrect_answers = 0, "
                           "total_attempts = 0 WHERE username = ? ";

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, sqlQUERY, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to preparing statement : %s\n", sqlite3_errmsg(db));
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, uname, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to bind {username} parameter : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    return true;
}


/**
 * @brief This function clears/resets all the currently logged-in players data in the 'badges' table
 *
 * @param db A pointer to the SQLite database connection
 * @param uname A pointer to the username of the currently logged in user
 *
 * @returns true if data reset is successful otherwise false
 */
bool reset_badges_table(sqlite3 *db, const char *uname) {
    const char *sqlQuery = "UPDATE badges SET "
                           "rocket = 0, "
                           "shield = 0, "
                           "trophy = 0, "
                           "starter = 0, "
                           "bulls_eye = 0, "
                           "glowing_star = 0, "
                           "hundred_points = 0, "
                           "perfectionist = 0 WHERE username = ?;";

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, sqlQuery, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s\n", sqlite3_errmsg(db));
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, uname, NULL_BYTE, SQLITE_TRANSIENT) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to bind {username} parameter : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        log_error(__func__, __FILE__, __LINE__, "Failed to execute statement : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    return true;
}


/**
 * @brief This function checks if the 'database' file already exist. If not, it creates
 *        and populates the database file with a default user account, player details,
 *        & quiz questions & answers.
 *      The 'database' contains multiple tables for keeping track of 'account info', 'player stats info', 'badges info',
 *      and 'quiz questions & answers'.
 *      The function also creates two default/demo accounts a John & Jane Doe
 *
 * @returns true if the database already exists, or if the other database utility functions successfully executes, otherwise false.
**/
bool setup(void) {
    if (access(DBFILEPATH, F_OK) == 0) {
        print(" [%s] Database already exists.\n", SUCCESS_EMOJI);
        return true;
    }

    if (!create_new_database()) {
        return false;
    }

    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        print(" [%s] Error: Failed to open database!...\n", WARNING_EMOJI);
        return false;
    }
    print(" [%s] Database created successfully.\n", SUCCESS_EMOJI);

    if (!insert_quiz_questions_answers(db)) {
        return false;
    }
    sqlite3_close(db);

    account_t demo_acc1 = {
        .name = "John Alvin", .surname = "Doe",
        .password = "Avjohn12345$", .profile.date = "01-01-2024 00:00:00",
        .profile.username = "johndoe_11", .profile.playerId = "pl-1010101010"
    };
    account_t demo_acc2 = {
        .name = "Jane M", .surname = "Doe",
        .password = "jB@13Svnh45#", .profile.date = "01-01-2024 00:00:00",
        .profile.username = "jdoe12_m", .profile.playerId = "pl-2020202020"
    };

    pstats_t default_p1, default_p2;
    struct_memset(&default_p1, sizeof(pstats_t));
    struct_memset(&default_p2, sizeof(pstats_t));

    if (!init_default_player_stats(&demo_acc1, &default_p1)) {
        print(" [%s] Error initializing player stats 1!...\n", WARNING_EMOJI);
        return false;
    }
    if (!init_default_player_stats(&demo_acc2, &default_p2)) {
        print(" [%s] Error initializing player stats 2!...\n", WARNING_EMOJI);
        return false;
    }

    if (!insert_new_player_data(&demo_acc1, &default_p1)) {
        print(" [%s] Error inserting 'default player 1' data into the database!...\n", WARNING_EMOJI);
        return false;
    }

    if (!insert_new_player_data(&demo_acc2, &default_p2)) {
        print(" [%s] Error inserting 'default player 2' data into the database!...\n", WARNING_EMOJI);
        return false;
    }

    return true;
}