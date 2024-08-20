#include "../../include/db.h"
#include "../../include/utilities.h"


/**
 * @brief This function reads 10 random questions along with their choices, from the given table in the database
 *
 * @param db A pointer to the SQLite3 database connection.
 * @param tableName A pointer to the name of the quiz table to read data from.
 * @param questions A pointer to the quiz_t structure to store the quiz data.
 * @param maxQuests MAximum number fo questions to retrieve from the database
 *
 * @returns true if quiz questions are successfully retrieved from the database, otherwise false.
 */
bool get_questions(sqlite3 *db, quiz_t *questions, const char *tableName, const size_t maxQuests) {
    if (NULL == db || NULL == questions || tableName == NULL || maxQuests <= 1) {
        log_error(__func__, __FILE__, __LINE__, "Invalid input parameters!...\n");
        return false;
    }

    char sql[MAX_BUFF];
    snprintf(sql, MAX_BUFF, "SELECT * FROM %s ORDER BY RANDOM() LIMIT %zu;", tableName, maxQuests);

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, sql, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed to prepare statement : %s!...\n", sqlite3_errmsg(db));
        return false;
    }

    int idx = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && (size_t)idx < maxQuests) {
        const unsigned char *question = sqlite3_column_text(stmt, 1);
        if (strlen((const char*)question) >= MAX_BUFF) {
            log_error(__func__, __FILE__, __LINE__, "Question exceeds buffer size!...\n");
            sqlite3_finalize(stmt);
            return false;
        }
        snprintf(questions[idx].question, MAX_BUFF, "%s", (const char*)question);

        for (size_t i = 0; i < MAX_CHOICES; i++) {
            const unsigned char *choice = sqlite3_column_text(stmt, 2 + i);
            if (strlen((const char*)choice) >= BUF_SIZE) {
                log_error(__func__, __FILE__, __LINE__, "Choice %zu exceeds buffer size.\n", i);
                sqlite3_finalize(stmt);
                return false;
            }
            snprintf(questions[idx].choices[i], BUF_SIZE, "%s", (const char*)choice);
        }

        const unsigned char *correct_answer = sqlite3_column_text(stmt, 6);
        if (strlen((const char*)correct_answer) >= BUF_SIZE) {
            log_error(__func__, __FILE__, __LINE__, "Correct choice exceeds buffer size!...\n");
            sqlite3_finalize(stmt);
            return false;
        }
        snprintf(questions[idx].correct_choice, BUF_SIZE, "%s", (const char*)correct_answer);

        idx += 1;
    }
    sqlite3_finalize(stmt);
    return true;
}


/**
 * @brief This function writes the randomly generated questions by the get_questions function,
 *        to the quiz_t structure and then displays the questions for the player to answer.
 *
 * @param box_offset Offset position value of the box to be printed
 * @param player A pointer to the player stats structure.
 * @param category The category of questions to display.
 */
void display_questions(const int box_offset, pstats_t *player, const char *category) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Error opening database : errMsg = %s!...\n", sqlite3_errmsg(db));
        cgetch();
        mainmenu(box_offset, player);
    }

    char content[BUF_SIZE];
    sprintf(content, " QUIZBIT - %s TEST ", category);
    quiz_t questions[MAX_QUESTIONS + 1];

    //? Read '10' random questions from the given table
    if (get_questions(db, questions, category, MAX_QUESTIONS) == false) {
        log_error(__func__, __FILE__, __LINE__, "Failed to get questions from db %s!...\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        cgetch();
        mainmenu(box_offset, player);
    }
    sqlite3_close(db);

    const int x_coord = box_offset, y_coord = 1;
    char heading[BUF_SIZE] = " QUIZBIT ━━ GAMEPLAY ";

    draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
    display_header(box_offset, heading, player);
    display_footer(box_offset);

    const int tmp_x = x_coord + PROMPT_PADDING;
    const int tmp_y = y_coord + HEADER_HEIGHT + 3;

    display_progress_bar(x_coord, tmp_y + 1, "Gameplay loading please wait");

    short qid = 0, skipped = 0;
    uint32_t score = 0;

    const clock_t start_time = clock();
    do {
        memset(heading, 0, sizeof(heading));
        snprintf(heading, BUF_SIZE, " QUIZBIT ━━ %s TEST ", category);

        draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
        display_header(box_offset, heading, player);
        display_footer(box_offset);

        questions[qid].question_id = (int)qid + 1;
        mvprint(tmp_x, tmp_y + 1, BOLD, "%i.) %s", questions[qid].question_id, questions[qid].question);

        for (size_t i = 0; i < MAX_CHOICES; ++i) {
            mvprint(tmp_x + 3, tmp_y + i + 2, BOLD, "%s", questions[qid].choices[i]);
        }
        mvprint(tmp_x, tmp_y + 9, BOLD, " PRESS [S] TO SKIP THE QUESTION");

        const uint32_t check = check_answer(questions, qid);
        if (check == SKIP) {
            skipped += 1;
        } else {
            score += check;
        }

        qid += 1;
    } while (qid < MAX_QUESTIONS);
    const clock_t end_time = clock();

    const double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    print_success(tmp_x, tmp_y + 11, "You scored : %04u points", score);
    const uint32_t bonusScore = assign_bonus(score, elapsed_time);
    print_success(tmp_x, tmp_y + 13, "Your new score after the bonus is %04u", score + bonusScore);
    getnew_playerstats(player, player->scores.currentScore, bonusScore, score, skipped, elapsed_time);

    player->scores.currentScore = score + bonusScore;

    unlock_achievements(player, player->scores.currentScore, elapsed_time);

    if (!update_score(player, player->scores.currentScore, player->scores.averageScore, player->scores.highestScore)) {
        print_error(tmp_x, tmp_y + 15, "Failed to update new score!...");
                delay(2000);
        gameplay(box_offset, player);
    }
    if (!update_gamestats(player)) {
        print_error(tmp_x, tmp_y + 15, "Failed to update player stats!...");
            delay(2000);
        gameplay(box_offset, player);
    }
    if (!get_player_data(player->profile.username, player)) {
        print_error(tmp_x, tmp_y + 15, "Failed to get updated data, please login again!...");
        delay(2000);
        logout(box_offset, player);
    }

    mvprint(tmp_x, tmp_y + 15, BOLD, " %s Do you want to view your answers [Y/N] ? ", EYES_EMOJI);
    if (get_confirmation(NULL) == 'Y') {
        display_answered(box_offset, player, questions);
    }

    mvprint(tmp_x, tmp_y + 20, BOLD, "%s Press any key to go back to the main menu : ", BACK_EMOJI);
    cgetch();
    mainmenu(box_offset, player);
}


/**
 * @brief This function keeps records of the answers the user entered while playing and displays them after the game.
 *
 * @param box_offset Offset position value of the box to be printed
 * @param player A pointer to the player stats structure.
 * @param quest A pointer to the Quiz_t structure containing questions and correct answers.
**/
void display_answered(const int box_offset, pstats_t *player, quiz_t *quest) {
    short qid = 0, check = 0;
    for (int i = 0; i < 2; ++i) {
        const int x_coord = box_offset, y_coord = 1;
        const char *heading = " QUIZBIT ━━ TEST ANSWERS ";

        draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
        display_header(box_offset, heading, player);
        display_footer(box_offset);

        const int tmp_x = x_coord + PROMPT_PADDING; int tmp_y = y_coord + HEADER_HEIGHT + 1;

        while (check < MAX_QUESTIONS) {
            quest[qid].question_id = (int)qid + 1;
            mvprint(tmp_x, tmp_y += 2, BOLD, "%hi.) %s", quest[qid].question_id, quest[qid].question);

            if (strcmp(quest[qid].selected_choice, quest[qid].correct_choice) == 0) {
                mvprint(tmp_x + 2, tmp_y += 1, BOLD, "%s Your Answer    : %s", CHECKM_EMOJI, quest[qid].selected_choice);
                mvprint(tmp_x + 2, tmp_y += 1, BOLD, "%s Correct Answer : %s", CHECKM_EMOJI, quest[qid].correct_choice);
            }
            else {
                mvprint(tmp_x + 2, tmp_y += 1, BOLD, "%s Your Answer    : %s", CROSSM_EMOJI, quest[qid].selected_choice);
                mvprint(tmp_x + 2, tmp_y += 1, BOLD, "%s Correct Answer : %s", CHECKM_EMOJI, quest[qid].correct_choice);
            }
            qid += 1;
            check += 1;
            if (MAX_QUESTIONS / 2 == check) {
                break;
            }
        }
        if (check > MAX_QUESTIONS - 1) {
            break;
        }
        mvprint(tmp_x, tmp_y + 2, BOLD, " Press the [SPACE-BAR] key to view the rest : ");
        do { } while (cgetch() != SPACE_BAR);
    }
}



/**
 * @brief This function compares the selected choice to the correct choice and return the score of that question.
 *
 * @param questions A pointer to the quiz questions and answers structure
 * @param qid A variable to hold the current question id
 * @returns 100 if the selected choice is correct, else return 0
 */
uint32_t check_answer(quiz_t *questions, const short qid) {
    short check;
    do {
        const int choice = toupper(cgetch());
        switch (choice) {
            case 'A' :
            case 'B' :
            case 'C' :
            case 'D' :
                snprintf(questions[qid].selected_choice, BUF_SIZE, "%s", questions[qid].choices[choice - 'A']);
            check = VALUE_SUCCESS;
            break;
            case 'S' :
                snprintf(questions[qid].selected_choice, BUF_SIZE, "SKIPPED");
                return SKIP;
            default:
                check = VALUE_ERROR;
        }
    } while (VALUE_ERROR == check);

    return strcmp(questions[qid].selected_choice, questions[qid].correct_choice) == VALUE_SUCCESS ? MAXIMUM_SCORE : INITIAL_SCORE;
}


/**
 * @brief This function is only called when user wants to play as guest
 *        It displays 5 random question from a random game category
 */
void play_as_guest(const int BOX_OFFSET) {
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        log_error(__func__, __FILE__, __LINE__, "Failed To Open Database : %s!...\n", sqlite3_errmsg(db));
        cgetch();
        homepage(BOX_OFFSET);
    }

    const char *categorySet[] = {"SPORTS", "SCIENCE", "BASICS"};
    srand(time(NULL));
    const int randomIndex = rand() % 3;
    const char *category = categorySet[randomIndex];

    char content[BUF_SIZE];
    sprintf(content, " QUIZBIT - %s TEST ", category);

    const size_t maxQuestions = MAX_QUESTIONS / 2;
    quiz_t questions[maxQuestions + 1];

    //? Read 5 random questions from the given table
    if (!get_questions(db, questions, category, maxQuestions)) {
        log_error(__func__, __FILE__, __LINE__, "Failed to get questions from db!...\n");
        sqlite3_close(db);
        cgetch();
        homepage(BOX_OFFSET);
    }
    sqlite3_close(db);

    char guestId[ID_SIZE];
    assign_new_player_id(guestId);

    const int x_coord = BOX_OFFSET, y_coord = 1;
    char heading[BUF_SIZE] = " QUIZBIT ━━ GUESTMODE ";

    draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
    display_header(BOX_OFFSET, heading, NULL);
    display_footer(BOX_OFFSET);

    const int tmp_x = x_coord + PROMPT_PADDING;
    const int tmp_y = y_coord + HEADER_HEIGHT + 3;

    mvprint(tmp_x + 18, tmp_y - 1, BOLD, "%s PLAYERID : %s", ID_EMOJI, guestId);
    display_progress_bar(x_coord, tmp_y + 1, "Gameplay loading please wait");

    memset(heading, 0, sizeof(heading));
    snprintf(heading, BUF_SIZE, " QUIZBIT ━━ %s TEST ", category);

    short qid = 0, skipped = 0;
    uint32_t guestScore = 0;

    do {
        draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
        display_header(BOX_OFFSET, heading, NULL);
        display_footer(BOX_OFFSET);

        mvprint(tmp_x + 18, tmp_y - 1, BOLD, "%s PLAYERID : %s", ID_EMOJI, guestId);

        questions[qid].question_id = (int)qid + 1;
        mvprint(tmp_x, tmp_y + 1, BOLD, "%i.) %s", questions[qid].question_id, questions[qid].question);

        for (size_t i = 0; i < MAX_CHOICES; ++i) {
            mvprint(tmp_x + 3, tmp_y + i + 2, BOLD, "%s", questions[qid].choices[i]);
        }
        mvprint(tmp_x, tmp_y + 8, BOLD, " PRESS [S] TO SKIP THE QUESTION");

        const uint32_t check = check_answer(questions, qid);
        if (check == SKIP) {
            skipped += 1;
        } else {
            guestScore += check;
        }

        qid += 1;
    } while ((size_t)qid < maxQuestions);

    print_success(tmp_x, tmp_y + 10, "You scored : %04u", guestScore);

    mvprint(tmp_x, tmp_y + 15, BOLD, "To enjoy additional features of the game, please login/create an account.");

    mvprint(tmp_x, tmp_y + 17, BOLD, "Press any key to go back to the main menu : ");
    cgetch();
    homepage(BOX_OFFSET);
}