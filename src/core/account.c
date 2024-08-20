#include "../../include/console.h"
#include "../../include/db.h"
#include "../../include/gamecore.h"


/**
 * @brief This function prompts users to signup for a new account.
 *        It reads input from user, checks if the given details meet the requirements
 *        then finalizes the registration process and store the data to the database
 *
 * @param box_offset offset position value of the box to be printed in the terminal
 **/
void signup(const int box_offset) {
    const int y_coord = 1;
    const int x_coord = box_offset;

    account_t *newUser = calloc(1, sizeof(account_t));
    if (newUser == NULL) {
        log_error(__func__, __FILE__, __LINE__, "calloc() : %s\n", strerror(ENOMEM));
        exit_program(EXIT_FAILURE);
    }

    short check = VALUE_ERROR;
    do {
        clr_scr();
        const char *heading = " QUIZBIT ━━ SIGNUP ";

        draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
        display_header(box_offset, heading, NULL);
        display_footer(box_offset);

        //? Show the minimum requirements for usernames & passwords
        display_requirements(x_coord + 10, y_coord + HEADER_HEIGHT + 1);
        struct_memset(newUser, sizeof(account_t));

        if (get_new_user_info(x_coord + PROMPT_PADDING, y_coord + HEADER_HEIGHT + 7, newUser)) {
            //? Validate credentials & check if the username is already in use in the database
            if (!valid_credentials(newUser->profile.username, newUser->password)) {
                print_error(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 17,
                            "Username/password must meet the requirements, try again");
                cgetch();
            }
            else if (is_username_taken(newUser->profile.username)) {
                print_error(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 17,
                            "Username isn't available, try again");
                cgetch();
            }
            else {
                char *hashedPass = sha256_hashpass(newUser->password);
                if (hashedPass == NULL) {
                    free(newUser);
                    exit_program(EXIT_FAILURE);
                }
                snprintf(newUser->password, KEY_SIZE, "%s", hashedPass);
                free(hashedPass);
                assign_new_player_id(newUser->profile.playerId);
                check = VALUE_SUCCESS;
            }
        }
    } while (VALUE_ERROR == check);


    //? Allocate & initialize the player stats for the newly registered account
    pstats_t *newPlayer = calloc(1, sizeof(pstats_t));
    if (newPlayer == NULL) {
        print_error(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 17,
                    " [%s] Error : Memory Allocation Failed!... Quitting.\n", WARNING_EMOJI);
        free(newUser);
        cgetch();
        exit_program(EXIT_FAILURE);
    }

    struct_memset(newPlayer, sizeof(pstats_t));
    if (!init_new_player_stats(newPlayer, newUser)) {
        print_error(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 17,
                    " [%s] Error initializing new player data", WARNING_EMOJI);
        free(newPlayer);
        free(newUser);
        exit_program(EXIT_FAILURE);
    }

    insert_new_player_data(newUser, newPlayer);
    print_success(x_coord + 5, y_coord + HEADER_HEIGHT + 17,
                  "Account Successfully Created!... Press any key to go to the login page : ");
    free(newUser);
    free(newPlayer);
    cgetch();
    login(box_offset);
}


/**
 * @brief Handles the login process, prompting users to enter their credentials.
 *
 * @param box_offset offset position value of the box to be printed in the terminal
 */
void login(const int box_offset) {
    pstats_t *player = calloc(1, sizeof(pstats_t));
    if (player == NULL) {
        log_error(__func__, __FILE__, __LINE__, "calloc() : %s\n", strerror(ENOMEM));
        exit_program(EXIT_FAILURE);
    }

    const int x_coord = box_offset, y_coord = 1;
    //? Temp variable to store the uname and passwd
    char __uname__[UNAME_SIZE];
    short check = VALUE_SUCCESS;
    do {
        clr_scr();
        const char *heading = " QUIZBIT ━━ SIGNIN ";

        draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);

        set_console_text_attr(RESET_ATTR);
        display_header(box_offset, heading, NULL);
        display_footer(box_offset);

        set_console_text_attr(BOLD);

        char __passwd__[PASS_SIZE];
        memset(__uname__, 0, UNAME_SIZE);
        memset(__passwd__, 0, PASS_SIZE);
        set_console_cursor_mode(ENABLE);

        mvprint(x_coord + PROMPT_PADDING, y_coord + HEADER_HEIGHT + 1, BOLD,
                " %s Enter your username : ", NAME_EMOJI);
        input(NULL, __uname__, UNAME_SIZE);

        mvprint(x_coord + PROMPT_PADDING, y_coord + HEADER_HEIGHT + 3, BOLD,
                " %s Enter your password : ", LOGIN_EMOJI);
        get_pass(NULL, __passwd__, PASS_SIZE);

        display_loader(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 5, "Checking!...");

        if (!is_login_valid(__uname__, __passwd__)) {
            print_error(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 6, "Incorrect Username/password, Try Again : ");
            check += 1;
            cgetch();

        }
        else { check = VALUE_SUCCESS; }

    } while (check < MAX_LOGIN_ATTEMPT && check > VALUE_SUCCESS);

    if (check >= MAX_LOGIN_ATTEMPT) {
        print_error(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 8,
                    "Too many login attempts : %s!...", strerror(ECANCELED));
        cgetch();
    }
    else {
        print_success(x_coord + PROMPT_PADDING + 2, y_coord + HEADER_HEIGHT + 8,
                      "Login Successful!... Press any key to go to the main menu : ");

        struct_memset(player, sizeof(pstats_t));
        get_player_data(__uname__, player);
        cgetch();
        mainmenu(box_offset, player);
    }
    free(player);
    homepage(box_offset);
}


/**
 * @brief This function logs out the current user session by deallocating
 *        the memory then redirecting to homepage
 *
 * @param box_offset
 * @param Session A pointer to the user account structure
 */
void logout(const int box_offset, void *Session) {
    if (Session) {
        struct_memset(Session, sizeof(Session));
        free(Session);
        homepage(box_offset);
    }
}


/**
 * @brief This function manages the player account by calling either the edit or delete function
 *
 * @param box_offset offset position value of the box to be printed in the terminal
 * @param player A pointer to the current player structure
 */
void manage_account(const int box_offset, pstats_t *player) {
    char menu[][BUF_SIZE] = {
        "MODIFY ACCOUNT",
        "DELETE ACCOUNT",
        "RESET ACCOUNT DATA",
        "MAIN MENU"
    };
    char emojis[][BADGE_SIZE] = {RESET_EMOJI, PEN_EMOJI, BIN_EMOJI, MENU_EMOJI};

    int highlight = 0;
    do {
        const char *heading = " QUIZBIT ━━ MANAGE ACCOUNT ";
        const size_t size = sizeof(menu)/sizeof(menu[0]);

        display_main_page(box_offset, highlight, size, player, heading, NULL, emojis, menu);

        set_console_text_attr(BOLD);
        const int c = cgetch();
        if (c == ENTER_KEY) {
            switch (highlight) {
                case EDITACCOUNT :
                    modify_account(box_offset, EDITACCOUNT, player);
                    break;

                case DELETEACCOUNT :
                    modify_account(box_offset, DELETEACCOUNT, player);
                    break;

                case RESETACCOUNT :
                    modify_account(box_offset, RESETACCOUNT, player);
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
 * @brief This function calls the delete, edit, or reset account functions based on the status of the 'mode' variable
 *        The function is called in the 'manage account' function above
 *
 * @param box_offset Offset position value of the box to be printed
 * @param mode mode to determine which function to call
 * @param player A pointer to the player stats structure
 */
void modify_account(const int box_offset, const int mode, pstats_t *player) {
    const int x_coord = box_offset, y_coord = 1;

    char heading[BUF_SIZE];
    snprintf(heading, BUF_SIZE, " QUIZBIT ━━ %s ACCOUNT ",
             mode == EDITACCOUNT ? "MODIFY" : mode == DELETEACCOUNT ? "DELETE" : "RESET");

    draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
    display_header(box_offset, heading, player);
    display_footer(box_offset);
;
    print_error(x_coord + 8, y_coord + HEADER_HEIGHT + 4,
                " This action is irreversible. Are you sure you want to continue? [Y/N] : ");
    if (get_confirmation(NULL) == 'N') {
        settings(box_offset, player);
    }

    char passwd[KEY_SIZE];
    mvprint(x_coord + 8, y_coord + HEADER_HEIGHT + 6, BOLD, "Enter your password to continue : ");
    get_pass(NULL, passwd, KEY_SIZE);
    if (!is_login_valid(player->profile.username, passwd)) {
        print_error(x_coord + 8, y_coord + HEADER_HEIGHT + 7, "Incorrect password, Logging out... : ");
        free(player);
        delay(2000); //? sleep for 2 seconds
        homepage(box_offset);
    }

    if (mode == EDITACCOUNT) {
        edit_account(box_offset, player);
    }
    else if (mode == DELETEACCOUNT) {
        delete_account(box_offset, player);
    }
    else if (mode == RESETACCOUNT) {
        reset_account_data(box_offset, player);
    }
    else {
        mainmenu(box_offset, player);
    }
}


/**
 * @brief This function allows the player to edit and update their account information,
 *        such as name, surname, username, or password.
 *
 * @param box_offset offset position value of the box to be printed in the terminal
 * @param player A pointer to the player stats structure.
 */
void edit_account(const int box_offset, pstats_t *player) {
    char fName[NAME_SIZE];
    char lName[NAME_SIZE];
    char uName[UNAME_SIZE];
    char passwd[PASS_SIZE];

    int tmp_x, tmp_y;
    const int x_coord = box_offset;

    short check = VALUE_ERROR;
    ssize_t fName_len, lName_len = 0, uName_len = 0, paswd_len = 0;

    do {
        const int y_coord = 1;
        const char *heading = " QUIZBIT ━━ EDIT ACCOUNT ";

        draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
        display_header(box_offset, heading, player);
        display_footer(box_offset);

        tmp_x = x_coord + PROMPT_PADDING;
        tmp_y = y_coord + HEADER_HEIGHT + 3;

        if ((fName_len = edit_field(tmp_x, tmp_y, "firstname", fName, NAME_SIZE)) == NULL_STRING) {
            print_error(tmp_x + 2, tmp_y + 1, "firstname field can't be empty.");
        }
        else if ((lName_len = edit_field(tmp_x, tmp_y += 3, "lastname", lName, NAME_SIZE)) == NULL_STRING) {
            print_error(tmp_x + 2, tmp_y + 1, "lastname field can't be empty.");
            cgetch();
        }
        else if ((uName_len = edit_field(tmp_x, tmp_y += 3, "username", uName, UNAME_SIZE)) == NULL_STRING) {
            print_error(tmp_x + 2, tmp_y + 1, "username field can't be empty.");
            cgetch();
        }
        else if ((paswd_len = edit_field(tmp_x, tmp_y += 3, "password", passwd, PASS_SIZE)) == NULL_STRING) {
            print_error(tmp_x + 2, tmp_y + 1, "password field can't be empty.");
            cgetch();
        }
        else {
            check = VALUE_SUCCESS;
            if (uName_len > 0) {
                if (!is_valid_uname(uName)) {
                    print_error(tmp_x, tmp_y + 2, "Username doesn't meet the requirements, try again");
                    check = VALUE_ERROR;
                    cgetch();
                }
                else if (is_username_taken(uName)) {
                    print_error(tmp_x, tmp_y + 2, "Username isn't available, try again");
                    check = VALUE_ERROR;
                    cgetch();
                }
            }
            if (paswd_len > 0) {
                if (!is_valid_passwd(passwd)) {
                    print_error(tmp_x, tmp_y + 2, "password must meet the requirements, try again");
                    check = VALUE_ERROR;
                    cgetch();
                }
            }
        }
    } while (check == VALUE_ERROR);

    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        print_error(tmp_x, tmp_y += 2, "Error opening database : %s!...", sqlite3_errmsg(db));
        cgetch();
        mainmenu(box_offset, player);
    }

    // TODO : Update the database for the fields that were editted
    if (fName_len > 0) {
        if (!update_player_data(db, "name", player->profile.username, fName, "accounts")) {
            print_error(tmp_x, tmp_y + 2, "Failed to update database!...'see: errlog.txt'");
            sqlite3_close(db);
            cgetch();
            settings(box_offset, player);
        }
    }

    if (lName_len > 0) {
        if (!update_player_data(db, "surname", player->profile.username, lName, "accounts")) {
            print_error(tmp_x, tmp_y + 2, "Failed to update database!...'see: errlog.txt'");
            sqlite3_close(db);
            cgetch();
            settings(box_offset, player);
        }
    }

    if (paswd_len > 0) {
        char *hashedPass = sha256_hashpass(passwd);
        if (hashedPass == NULL) {
            print_error(tmp_x, tmp_y + 2, "Error : %s to struct member!...", strerror(ENOMEM));
            settings(box_offset, player);
        }
        else if (!update_player_data(db, "password", player->profile.username, hashedPass, "accounts")) {
            print_error(tmp_x, tmp_y + 2, "Failed to update database!...'see: errlog.txt'");
            sqlite3_close(db);
            cgetch();
            free(hashedPass);
            settings(box_offset, player);
        }
        else {
            free(hashedPass);
        }
    }

    if (uName_len > 0) {
        if (!(update_player_data(db, "username", player->profile.username, uName, "accounts") &&
              update_player_data(db, "username", player->profile.username, uName, "players") &&
              update_player_data(db, "username", player->profile.username, uName, "badges"))) {
            print_error(tmp_x, tmp_y + 2, "Failed to update database!...'see: errlog.txt'");
                sqlite3_close(db);
                cgetch();
                settings(box_offset, player);
        }
    }
    else { snprintf(uName, UNAME_SIZE, "%s", player->profile.username);}

    print_success(tmp_x, tmp_y + 2, "Account updated successfully!... press any key to continue");
    struct_memset(player, sizeof(pstats_t));
    if (!get_player_data(uName, player)) {
        print_error(tmp_x, tmp_y + 4, "Failed to get updated data, please login again!...");
        free(player);
        delay(2000);
        logout(box_offset, player);
    }
    cgetch();
    mainmenu(box_offset, player);
}


/**
 * @brief This function prompts the user to edit their account data
 *
 * @param x_coord X-coordinate value of the cursor position
 * @param y_coord Y-coordinate value of the cursor position
 * @param fieldName A pointer to the field to be modified
 * @param newField A pointer to the variable to hld the new data of the edited field
 * @param size The array size of the field to be edited
 *
 * @return 0 if the user doesn't want to edit the field, else return the length of the new value of the editted field
 */
ssize_t edit_field(const int x_coord, const int y_coord, const char *fieldName, char *newField, const size_t size) {
    mvprint(x_coord, y_coord, BOLD,
                "[%s] Do you want to edit your %s [Y/N] ? ", HELP_EMOJI, fieldName);
    if (get_confirmation(NULL) == 'N') {
        return VALUE_SUCCESS;
    }

    memset(newField, 0, size);
    mvprint(x_coord, y_coord + 1, BOLD, "%s Enter your %s : ", NAME_EMOJI, fieldName);
    return input(NULL, newField, size);
}


/**
 * @brief This function allows the player to delete their account, removing all
 *        associated data from the players & users table in the database
 *
 * @param box_offset
 * @param player A pointer to the user player stats structure for the currently logged-in player.
 */
void delete_account(const int box_offset, pstats_t *player) {
    const int x_coord = box_offset, y_coord = 1;
    const char *heading = " QUIZBIT ━━ DELETE ACCOUNT ";

    draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);
    display_header(box_offset, heading, player);
    display_footer(box_offset);

    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        print_error(x_coord + 8, y_coord + HEADER_HEIGHT + 4, " [%s] Error opening database : %s!...\n", WARNING_EMOJI, sqlite3_errmsg(db));
        cgetch();
        mainmenu(box_offset, player);
    }

    mvprint(x_coord + 10, y_coord + HEADER_HEIGHT + 4, BOLD, "[%s] Press any key to finalize account deletion process : ", INFO_EMOJI);
    cgetch();

    if (delete_player_data(db, player, "accounts") &&
        delete_player_data(db, player, "players") && delete_player_data(db, player, "badges")) {
        print_success(x_coord + PROMPT_PADDING, y_coord + HEADER_HEIGHT + 6,
                    "Account deleted successfully!... Press any key to go back : ");
    }
    else {
        print_error(x_coord + PROMPT_PADDING, y_coord + HEADER_HEIGHT + 6,
                    "Error : Failed to delete account. You need to log in again...");
    }
    sqlite3_close(db);
    delay(2000);
    logout(box_offset, player);
}


/**
 * @brief This function resets the currenty logged in players stats/data back to default
 *
 * @param box_offset offset position value of the box to be printed in the terminal
 * @param player A pointer to the player data to clear
 */
void reset_account_data(const int box_offset, pstats_t *player) {
    const int x_coord = box_offset, y_coord = HEADER_HEIGHT + 8;
    sqlite3 *db = NULL;
    if (sqlite3_open_v2(DBFILEPATH, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
        mvprint(x_coord + PROMPT_PADDING, y_coord, BOLD, " [%s] database error : check %s", WARNING_EMOJI, ERROR_LOGPATH);
        log_error(__func__, __FILE__, __LINE__, "%s\n", sqlite3_errmsg(db));
        delay(2000);
        settings(box_offset, player);
    }

    bool ret = true;
    if (!reset_badges_table(db, player->profile.username)) {
        mvprint(x_coord + PROMPT_PADDING, y_coord + 2, BOLD,
                " [%s] Failed to reset the values in the 'badges' table", WARNING_EMOJI);
        ret = false;
    }
    else if (!reset_players_table(db, player->profile.username)) {
        mvprint(x_coord + PROMPT_PADDING, y_coord + 2, BOLD,
                " [%s] Failed to reset the values in the 'players' table", WARNING_EMOJI);
        ret = false;
    }
    sqlite3_close(db);

    if (ret == false) {
        cgetch();
        settings(box_offset, player);
    }

    print_success(x_coord + PROMPT_PADDING, y_coord + 2,
                "You data successfully reset!... press any key to go back : ");
    char uName[UNAME_SIZE];
    snprintf(uName, UNAME_SIZE, "%s", player->profile.username);

    struct_memset(player, sizeof(pstats_t));
    if (!get_player_data(uName, player)) {
        print_error(x_coord + PROMPT_PADDING, y_coord + 4, "Failed to get updated data, please login again!...");
        delay(2000);
        logout(box_offset, player);
    }

    cgetch();
    mainmenu(box_offset, player);
}


/**
 * @brief This function prompts new users to enter their data when signing up for a new account
 *
 * @param user A pointer to the account_t structure to store new user data
 * @param x X-coordinate value of the cursor
 * @param y Y-coordinate value of the cursor
 *
 * @returns true if the function executes successfully, otherwise false.
**/
bool get_new_user_info(const int x, int y, account_t *user) {
    if (NULL == user) {
        print_error(x, y + 2, "Error : NULL input parameters!...\n");
        return false;
    }

    mvprint(x, y += 2, BOLD, "%s Enter your firstname : ", NAME_EMOJI);
    if (input(NULL, user->name, NAME_SIZE) == VALUE_ERROR) {
        print_error(x + 3, y + 1, "Error : Name field can't be empty!...");
        cgetch();
        return false;
    }

    mvprint(x, y += 2, BOLD, "%s Enter your lastname : ", NAME_EMOJI);
    if (input(NULL, user->surname, NAME_SIZE) == VALUE_ERROR) {
        print_error(x, y + 1, "Error : Name field can't be empty!...");
        cgetch();
        return false;
    }

    mvprint(x, y += 2, BOLD, "%s Enter your username : ", NAME_EMOJI);
    if (input(NULL, user->profile.username, UNAME_SIZE) == VALUE_ERROR) {
        print_error(x, y + 1, "Error : Username field can't be empty!...");
        cgetch();
        return false;
    }

    mvprint(x, y += 2, BOLD, "%s Enter your password : ", LOGIN_EMOJI);
    if (input(NULL, user->password, PASS_SIZE) == VALUE_ERROR) {
        print_error(x, y + 1, "Error : Password field can't be empty!...");
        cgetch();
        return false;
    }
    return true;
}
