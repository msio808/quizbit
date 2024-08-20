#include "../../include/gamecore.h"
#include "../../include/db.h"


/**
 * @brief This function displays the displays available options on the homepage.
 *        Users can choose to login, signup, play as guest, or exit the program.
 *
 * @param BOX_OFFSET Offset position value of the box
 */
void homepage(const int BOX_OFFSET) {
    char menu[][BUF_SIZE] = {
        "LOG IN TO ACCOUNT ",
        "CREATE NEW ACCOUNT",
        "CONTINUE AS GUEST ",
        "EXIT/QUIT THE GAME"
    };
    const size_t size = sizeof(menu)/sizeof(menu[0]);
    char emojis[][BADGE_SIZE] = {LOGIN_EMOJI, SIGNUP_EMOJI, GAME_EMOJI, EXIT_EMOJI};

    clr_scr();
    int highlight = 0;

    do {
        const char *heading = " QUIZBIT ━━ QUIZGAME ";
        const char *sub_heading = "60+ questions to challenge the level of your knowledge on different topics";

        display_main_page(BOX_OFFSET, highlight, size, NULL, heading, sub_heading, emojis, menu);

        const int c = cgetch();
        if (c == ENTER_KEY) {
            switch (highlight) {
                case LOGIN :
                    login(BOX_OFFSET);
                    break;

                case SIGNUP :
                    signup(BOX_OFFSET);
                    break;

                case GUEST_MODE :
                    guestplayer_mode(BOX_OFFSET);
                    break;

                case EXIT :
                    exit_program(EXIT_SUCCESS);
                    break;

                default:
                    break;
            }
            break;
        }
        const int arrow_key = get_arrow_key(c);
        if (arrow_key != VALUE_ERROR) {
            highlight = highlight_line(arrow_key, size, highlight);
        }
    } while (true);
}


/**
 * @brief This function displays the main menu options, including gameplay, help, settings, and logout.
 *
 * @param player A pointer to the user account structure representing the current player.
 * @param box_offset Offset value of the box to be printed
 */
void mainmenu(const int box_offset, pstats_t *player) {
    char menu[][BUF_SIZE] = {
        "START NEW GAME",
        "DISPLAY HELP",
        "PREFERRENCES",
        "LOGOUT"
    };
    char emojis[][BADGE_SIZE] = {GAME_EMOJI, HELP_EMOJI, PREFS_EMOJI, LOGOUT_EMOJI};
    const size_t size = sizeof(menu)/sizeof(menu[0]);

    int highlight = 0;
    do {
        const char *heading = " QUIZBIT ━━ MAIN MENU ";
        display_main_page(box_offset, highlight, size, player, heading, NULL, emojis, menu);

        const int c = cgetch();
        if (c == ENTER_KEY) {
            switch (highlight) {
                case GAMEPLAY :
                    gameplay(box_offset, player);
                    break;

                case HELP :
                    display_help(box_offset, player);
                    break;

                case SETTINGS :
                    settings(box_offset, player);
                    break;

                case LOGOUT :
                    logout(box_offset, player);
                    break;

                default:
                    break;
            }
            break;
        }
        const int arrow_key = get_arrow_key(c);
        if (arrow_key != VALUE_ERROR) {
            highlight = highlight_line(arrow_key, size, highlight);
        }
    } while (true);
}


/**
 * @brief This function displays menu options for game stats and account management utilities.
 *
 * @param player A pointer to the user account structure representing the current player.
 * @param box_offset Offset position value of the box
 */
void settings(const int box_offset, pstats_t *player) {
    char menu[][BUF_SIZE] = {
        "VIEW LEADER BOARD",
        "VIEW PLAYER STATS",
        "MANAGE ACCOUNT",
        "MAIN MENU"
    };
    const size_t size = sizeof(menu)/sizeof(menu[0]);
    char emojis[][BADGE_SIZE] = {TROPHY_MEDAL, BCHART_EMOJI, GEAR_EMOJI, MENU_EMOJI};

    int highlight = 0;
    do {
        const char *heading = " QUIZBIT ━━ SETTINGS ";
        display_main_page(box_offset, highlight, size, player, heading, NULL, emojis, menu);

        const int c = cgetch();
        if (c == ENTER_KEY) {
            switch (highlight) {
                case LEADERBOARD :
                    display_scoreboard(box_offset, player);
                    break;

                case STATISTICS :
                    display_gamestats(box_offset, player);
                    break;

                case MANAGEACCOUNT :
                    manage_account(box_offset, player);
                    break;

                case MAINMENU :
                    mainmenu(box_offset, player);
                    break;

                default:
                    break;
            }
            break;
        }
        const int arrow_key = get_arrow_key(c);
        if (arrow_key != VALUE_ERROR) {
            highlight = highlight_line(arrow_key, size, highlight);
        }
    } while (true);
}


/**
 * @brief This function displays help/guide information on playing the game.
 *
 * @param player A pointer to the player's account structure.
 * @param box_offset Offset value of the box to be printed
 */
void display_help(const int box_offset, pstats_t *player) {
    display_header(box_offset, " QUIZBIT ━━ GAMEPLAY HELP ", player);

    const RGBset_t color = {
        .foreground = {0, 0, 0},
        .background = {255, 255, 255}
    };
    set_console_color_attr(color);

    int y = HEADER_HEIGHT + 4;
    const int x = box_offset + 5;

    mvprint(x, y, BOLD,
            "[%s] Welcome to QUIZBIT Quiz Challenge! This is a fun and educational game.    ", INFO_EMOJI);
    mvprint(x, y += 1, BOLD,
            "[%s] The game consists of 3 categories: science, sports, and general knowledge.", INFO_EMOJI);
    mvprint(x, y += 1, BOLD,
            "[%s] The system will randomly select questions from the chosen category.       ", INFO_EMOJI);
    mvprint(x, y += 1, BOLD,
            "[%s] You can reset scores, edit & delete your account from the settings page.  ", INFO_EMOJI);
    mvprint(x, y += 1, BOLD,
            "[%s] You will earn points (ie: Qbit Coins) and Qbit bonuses after every game.  ", INFO_EMOJI);
    mvprint(x, y += 1, BOLD,
            "[%s] If the new points is lower than old points, your points won't be updated. ", INFO_EMOJI);
    mvprint(x, y += 1, BOLD,
            "[%s] TIPS: Try to answer as quickly as possible to get extra Qbit points.      ", INFO_EMOJI);

    mvprint(x, y + 2, BOLD,
        "[%s] Press any key to go back : ", BULLET_EMOJI);
    cgetch();

    mainmenu(box_offset, player);
}


/**
 * @brief Displays gameplay options, allowing the player to choose categories before starting the game.
 *
 * @param player A pointer to the user account structure representing the current player.
 * @param box_offset Offset value of the box to be printed
 */
void gameplay(const int box_offset, pstats_t *player) {
    char menu[][BUF_SIZE] = {
        "SCIENCE TEST",
        "SPORTS TEST",
        "GENERAL KNOWLEDGE",
        "MAIN MENU"
    };
    const size_t size = sizeof(menu)/sizeof(menu[0]);
    char emojis[][BADGE_SIZE] = {SCIENCE_EMOJI, SPORTS_EMOJI, GLOBE_EMOJI, MENU_EMOJI};

    int highlight = 0;
    do {
        const char *heading = " QUIZBIT ━━ NEW GAME ";
        const char *sub_heading = "Select the game category you want to play : ";
        display_main_page(box_offset, highlight, size, player, heading, sub_heading, emojis, menu);

        const int c = cgetch();
        if (c == ENTER_KEY) {
            switch (highlight) {
                case SCIENCE :
                     display_questions(box_offset, player, "SCIENCE");
                     break;
                case SPORTS :
                     display_questions(box_offset, player, "SPORTS");
                     break;
                case GENERALKNOWLEDGE :
                     display_questions(box_offset, player, "BASICS");
                     break;
                case MAINMENU :
                     mainmenu(box_offset, player);
                     break;
                default:
                    break;
            }
            break;
        }
        const int arrow_key = get_arrow_key(c);
        if (arrow_key != VALUE_ERROR) {
            highlight = highlight_line(arrow_key, size, highlight);
        }
    } while (true);
}


/**
 * @brief This function calls the 'play as guest' feature for  users who don't want to signup/login to their account
 *
 * @param BOX_OFFSET Offset value of the box to be printed in the terminal window
 */
void guestplayer_mode(const int BOX_OFFSET) {
    char menu[][BUF_SIZE] = {
        "CONTINUE AS GUEST",
        "CREATE NEW ACCOUNT",
        "BACK TO HOMEPAGE",
        "EXIT/QUIT THE GAME"
    };
    const size_t size = sizeof(menu)/sizeof(menu[0]);
    char emojis[][BADGE_SIZE] = {LOGIN_EMOJI, SIGNUP_EMOJI, GAME_EMOJI, EXIT_EMOJI};

    clr_scr();
    int highlight = 0;

    do {
        const char *heading = " QUIZBIT ━━ GUEST MODE ";
        const char *sub_heading = "Inorder to get the full experience, signup for a new account!...";

        display_main_page(BOX_OFFSET, highlight, size, NULL, heading, sub_heading, emojis, menu);

        const int c = cgetch();
        if (c == ENTER_KEY) {
            switch (highlight) {
                case GUEST_PLAYER :
                    play_as_guest(BOX_OFFSET);
                break;

                case SIGNUP :
                    signup(BOX_OFFSET);
                break;

                case HOMEPAGE :
                    homepage(BOX_OFFSET);
                break;

                case EXIT :
                    exit_program(EXIT_SUCCESS);
                break;

                default:
                    break;
            }
            break;
        }
        const int arrow_key = get_arrow_key(c);
        if (arrow_key != VALUE_ERROR) {
            highlight = highlight_line(arrow_key, size, highlight);
        }
    } while (true);
}


/**
 * @brief This function resets the currently logged-in player's scores back to 0
 * @deprecated replaced by the '@reset_player_data' function
 *
 * @param box_offset Offset position of the box
 * @param player A pointer to the player stats 'pstats_t' structure
 */
/*
void reset_score(const int box_offset, pstats_t *player) {
    const int x_coord = box_offset + 5, y_coord = HEADER_HEIGHT + 2;
    print_error(x_coord, y_coord, "This action is irreversible. Are you sure you want to continue? [Y/N] : ");
    if (get_confirmation(NULL) == 'N') {
        settings(box_offset, player);
    }
    if (update_score(player, INITIAL_SCORE, INITIAL_SCORE, INITIAL_SCORE)) {
        char tmp[UNAME_SIZE];
        snprintf(tmp, UNAME_SIZE, "%s", player->profile.username);
        get_player_data(tmp, player);
    }
}*/


/**
 * @brief This function displays the top five players with the highest scores from the database.
 *        The players are ranked in order of current score, which updates after every game.
 *
 * @param box_offset Offset position value of the box to be printed
 * @param player A pointer to the player's account structure.
 */
void display_scoreboard(const int box_offset, pstats_t *player) {
    char query[MAX_BUFF];
    snprintf(query, sizeof(query),
    "SELECT player_id, current_score, average_score, highest_score FROM players ORDER BY current_score DESC LIMIT 5;");

    const int x = box_offset, y = 1;
    const char *heading = " QUIZBIT ━━ LEADERBOARD ";

    draw_box(x, y, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
    display_header(box_offset, heading, player);
    display_footer(box_offset);

    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        print_error(x + 7, y + HEADER_HEIGHT + 3,
                    " Error openning db : %s!...", sqlite3_errmsg(db));
        settings(box_offset, player);
    }

    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v3(db, query, NULL_BYTE, NO_PREP_FLAG, &stmt, NULL) != SQLITE_OK) {
        print_error(x + 7, y + HEADER_HEIGHT + 3,
                    " Error preparing statement : %s!...", sqlite3_errmsg(db));
        sqlite3_close(db);
        cgetch();
        settings(box_offset, player);
    }
    char sub_heading[BUF_SIZE];
    snprintf(sub_heading, sizeof(sub_heading), "%s Top Five Players Ranked On The Scoreboard", PAGE_EMOJI);

    const int tmp_y = y + HEADER_HEIGHT + 4;
    int padding = get_padding(strlen(sub_heading) - 2);
    mvprint(x + padding, tmp_y, BOLD, "%s", sub_heading);

    char content[MIN_BUFF] = {0};
                                                 //" %s CURRENT SCORE"
    snprintf(content, sizeof(content), " %s PLAYER ID  │ %s SCORE  │ %s AVG-SCORE  │ %s HIGH-SCORE  ",
            ID_EMOJI, SPORTS_EMOJI, BCHART_EMOJI, UCHART_EMOJI);

    const size_t len = strlen(content) - 14;

    padding = get_padding(len);
    mvprint(x + padding, tmp_y + 2, BOLD, "%s", content);

    set_console_cursor_position(x + padding - 1, tmp_y + 3);
    for (size_t i = 0; i <= len + 1; i++) {
        if (i == 16 || i == 28 || i == 44) print("┼");
        else print("─");
    }

    int i = 1;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *playerid = (const char *)sqlite3_column_text(stmt, 0);
        const int current_score = sqlite3_column_int(stmt, 1);
        const int average_score = sqlite3_column_int(stmt, 2);
        const int highest_score = sqlite3_column_int(stmt, 3);

        mvprint(x + padding - 2, tmp_y + i + 3, BOLD, "%s",
            i == 1 ? GOLD_MEDAL : i == 2 ? SILVER_MEDAL : i == 3 ? BRONZE_MEDAL : MILITARY_MEDAL);

        mvprint(x + padding + 01, tmp_y + i + 3, BOLD, "%s", playerid);
        mvprint(x + padding + 15, tmp_y + i + 3, BOLD, "│   %04u", current_score);
        mvprint(x + padding + 27, tmp_y + i + 3, BOLD, "│     %04u", average_score);
        mvprint(x + padding + 43, tmp_y + i + 3, BOLD, "│     %04u", highest_score);

        i += 1;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    mvprint(x + 18, y + tmp_y + 12, BOLD, " [%s] Players are ranked inorder of current score {%s}", INFO_EMOJI, SPORTS_EMOJI);
    mvprint(x + 20, y + tmp_y + 13, BOLD, " [%s] press any key to go back : ", BULLET_EMOJI);
    cgetch();
    settings(box_offset, player);
}


/**
 * @brief This function displays all the stats  & achievements of the currently logged-in player
 *
 * @param box_offset Offset position value of the box
 * @param player A pointer to the player stats structure
 */
void display_gamestats(const int box_offset, pstats_t *player) {
    const int x = box_offset, y = 1;
    const char *heading = " QUIZBIT ━━ PLAYERSTATS ";

    draw_box(x, y, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
    display_header(box_offset, heading, NULL);
    display_footer(box_offset);

    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        print_error(x + 7, y + HEADER_HEIGHT + 3,
                    " Error openning db : %s!...", sqlite3_errmsg(db));
        settings(box_offset, player);
    }

    char uname[UNAME_SIZE];

    account_t *user = calloc(1, sizeof(account_t));
    if (user == NULL) {
        log_error(__func__, __FILE__, __LINE__, "calloc() : %s\n", strerror(ENOMEM));
        settings(box_offset, player);
    }

    snprintf(uname, UNAME_SIZE, "%s", player->profile.username);

    struct_memset(user, sizeof(account_t));
    struct_memset(player, sizeof(pstats_t));

    bool ret = true;
    if (!retrieve_accounts_table(db, user, uname)) {
        log_error(__func__, __FILE__, __LINE__,
                "Failed to retrieve from 'accounts' table : %s!...\n", sqlite3_errmsg(db));
        ret = false;
    }

    if (!retrieve_players_table(db, player, uname)) {
        log_error(__func__, __FILE__, __LINE__,
                "Failed to retrieve from 'players' table : %s!...\n", sqlite3_errmsg(db));
        ret = false;
    }

    if (!retrieve_badges_table(db, player, uname)) {
        log_error(__func__, __FILE__, __LINE__,
                "Failed to retrieve from 'badges' table : %s!...\n", sqlite3_errmsg(db));
        ret = false;
    }
    sqlite3_close(db);

    if (!ret) {
        print_error(x, y, "Error retrieving player data, quitting!... see errorlog.txt");
        free(user);
        logout(box_offset, player);
    }

    const int tmp_x = x, tmp_y = y + HEADER_HEIGHT;

    mvprint(tmp_x + 10, tmp_y + 1, BOLD, "%s FULLNAME : %s, %s", NAME_EMOJI, user->surname, user->name);
    mvprint(tmp_x + 10, tmp_y + 2, BOLD, "%s USERNAME : %s", PROFILE_EMOJI, player->profile.username);
    mvprint(tmp_x + 10, tmp_y + 3, BOLD, "%s PLAYERID : %s", ID_EMOJI, player->profile.playerId);
    free(user);

    mvprint(tmp_x + 15, tmp_y + 6, BOLD, "%s CURRENT SCORE     : %04u  │  %s LONGEST STREAK    : %04u",
            SPORTS_EMOJI, player->scores.currentScore, FIRE_EMOJI, player->stats.longestStreak);
    mvprint(tmp_x + 15, tmp_y + 7, BOLD, "%s AVERAGE SCORE     : %04u  │  %s AVERAGE PLAYTIME  : %04u",
            BCHART_EMOJI, player->scores.averageScore, CLOCK_EMOJI, player->stats.timeSpentPerGame);
    mvprint(tmp_x + 15, tmp_y + 8, BOLD, "%s HIGHEST SCORE     : %04u  │  %s TOTALGAMES PLAYED : %04u",
            UCHART_EMOJI, player->scores.highestScore, GAME_EMOJI, player->stats.totalGamesPlayed);


    set_console_cursor_position(tmp_x + 15, tmp_y + 9);
    for (size_t i = 0; i <= 58; i++) {
        if (i == 29) {
            print("┼");
        }
        else {
            print("─");
        }
    }

    mvprint(tmp_x + 15, tmp_y + 10, BOLD, "%s CORRECT ANSWERS   : %04u  │  %s TOTAL GOLD COINS  : %04u",
            CHECKM_EMOJI, player->answers.totalCorrectAnswers, GOLDCOIN_EMOJI, player->treasure.totalGoldCoins);
    mvprint(tmp_x + 15, tmp_y + 11, BOLD, "%s INCORRECT ANSWERS : %04u  │  %s TOTAL MONEY BAGS  : %04u",
            CROSSM_EMOJI, player->answers.totalIncorrectAnswers, MONEYBAG_EMOJI, player->treasure.totalMoneyBags);
    mvprint(tmp_x + 15, tmp_y + 12, BOLD, "%s QUESTION ATTEMPTS : %04u  │  %s TOTAL GEM STONES  : %04u",
            HELP_EMOJI, player->answers.totalQuestionAttempted, GEMSTONE_EMOJI, player->treasure.totalGemStones);
    mvprint(tmp_x + 15, tmp_y + 13, BOLD, "%s PERFORMANCE RATE  : %03u%%  │  %s COMPLETION RATE   : %03u%%",
            PERCENT_EMOJI, player->stats.performanceRate, PERCENT_EMOJI, player->stats.quizCompletionRate);
    mvprint(tmp_x + 15, tmp_y + 14, BOLD, "%s GAMES COMPLETED   : %04u  │",
            NUMBERS_EMOJI, player->stats.totalGamesCompleted);

    mvprint(tmp_x + 12, tmp_y + 17, BOLD, "%s",
        player->badge.perfectionist == UNLOCKED ? "PERFECTIONIST : " PERFECTIONIST_BADGE : "UNLOCKED BADGES : ");

    if (player->badge.rocket == UNLOCKED) {
        mvprint(NUL, NUL, BOLD, "%s ", ROCKET_BADGE);
    }
    else if (player->badge.shield == UNLOCKED) {
        mvprint(NUL, NUL, BOLD, "%s ", SHIELD_BADGE);
    }
    else if (player->badge.trophy == UNLOCKED) {
        mvprint(NUL, NUL, BOLD, "%s ", TROPHY_MEDAL);
    }
    else if (player->badge.starter == UNLOCKED) {
        mvprint(NUL, NUL, BOLD, "%s ", STARTER_BADGE);
    }
    else if (player->badge.bullseye == UNLOCKED) {
        mvprint(NUL, NUL, BOLD, "%s ", BULLSEYE_BADGE);
    }
    else if (player->badge.glowingStar == UNLOCKED) {
        mvprint(NUL, NUL, BOLD, "%s ", G_STAR_BADGE);
    }
    else if (player->badge.hundredPoints == UNLOCKED) {
        mvprint(NUL, NUL, BOLD, "%s ", H_POINTS_BADGE);
    }
    else {
        mvprint(NUL, NUL, DIM, "NONE");
    }

    mvprint(tmp_x + 20, tmp_y + 19, BOLD, " %s Press any key to go back to the main menu : ", BULLET_EMOJI);
    cgetch();
    settings(box_offset, player);
}