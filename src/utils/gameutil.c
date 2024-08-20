#include "../../include/gamecore.h"
#include "../../include/utilities.h"


/**
 * @brief This function gets the padding value of the item to be printed within the box
 *
 * @param item The length of the string/item to be printed
 *
 * @returns the offset value of the string/item to be printed
 */
int get_padding(const size_t item) {
    return (MIN_BOX_WIDTH - item) / 2;
}


/**
 * @brief This function converts the given rgb values to 256 bit mode
 *
 * @param rgb RGB color structure
 *
 * @returns The 256-bit equivalent of the RGB values passed
 */
uint8_t rgb_256(const RGB_t rgb) {
    const uint8_t R_index = (uint8_t)round((double)rgb.red / RGB_MAX * BIT_MODE);
    const uint8_t G_index = (uint8_t)round((double)rgb.green / RGB_MAX * BIT_MODE);
    const uint8_t B_index = (uint8_t)round((double)rgb.blue / RGB_MAX * BIT_MODE);

    return BIT_OFFSET + R_index * R_FACTOR + G_index * G_FACTOR + B_index * B_FACTOR;
}


/**
 * @brief This function reads user input from the specified stream using fgets
 *        and replaces the trailing newline character with a null character.
 *
 * @param prompt A pointer to the prompt message to be displayed before reading input.
 * @param array A pointer to the character array to store the user input.
 * @param size The size of the character array.
 *
 * @returns the length of the array if successful, 0 if not read, or -1 if an error is encountered
 */
ssize_t input(const char *prompt, char *array, const size_t size) {
    if (prompt != NULL) {
        print("%s", prompt);
    }

    set_console_cursor_mode(ENABLE);

    if (size < 1 || array == NULL) {
        return NULL_STRING;
    }

    if (fgets(array, size, stdin) == NULL) {
        return VALUE_ERROR;
    }

    ssize_t len = strlen(array);
    if (len < 1 || array[len - 1] != ENTER_KEY) {
        return NULL_STRING;
    }

    len -= 1;
    array[len] = NULL_TERM;
    set_console_cursor_mode(DISABLE);
    return len;
}


/**
 *
 * @brief This function reads a password from the user without echoing the input
 *        and returns the length of the password.
 *        The function echoes a dot symbol on the screen as the user enters their password
 *
 * @param prompt A pointer to the prompt message to be displayed before reading the password input.
 * @param passwd A pointer to the character array to store the entered password.
 * @param size The maximum size/length of the password
 *
 * @returns The length of the entered password if successful, otherwise -1 if an error is encountered.
 */
ssize_t get_pass(const char *prompt, char *passwd, const size_t size) {
    if (prompt != NULL) {
        print("%s", prompt);
    }

    set_console_cursor_mode(ENABLE);

    if (size < 1 || passwd == NULL) {
        return VALUE_ERROR;
    }

    int c;
    size_t len = 0;
    while ((c = cgetch()) != ENTER_KEY && len < size - 1) {
        if (c == BACKSPACE && len > 0) {
            del_char(1);
            len -= 1;
        }
        else if (c != BACKSPACE) {
            passwd[len] = (char)c;
            print("%s", DOT_EMOJI);
            len += 1;
        }
    }

    //? Null-terminate the string & return length of password
    passwd[len] = NULL_TERM;
    set_console_cursor_mode(DISABLE);
    return len;
}


/**
 * @brief Displays a progress bar at the specified position on the screen.
 *
 * @param progress The percentage value of the progress bar.
 * @param length The length/frame size of the progress bar.
 */
void progress_bar(const size_t progress, const size_t length) {
    const RGB_t colors[] = {
        {255, 215, 0},   //? Gold
        {112, 128, 144}  //? Slate Grey
    };

    for (size_t i = 0; i < length; ++i) {
        if (i < progress) {
            set_bg_color(rgb_256(colors[0]));
            print(" ");
        }
        else {
            set_bg_color(rgb_256(colors[1]));
            print(" ");
        }
        set_console_text_attr(RESET_ATTR);
    }
    print(" %zu%%", FRAME_SIZE * progress * 2 / length);
}


/**
 * @brief This function calls the 'progress bar' function
 *        and displays a loader before starting any new game
 *
 * @param x X-coordinate value of the cursor
 * @param y Y-coordinate value of the cursor
 * @param message A pointer to the message to be displayed while the loader is running.
 */
void display_progress_bar(const int x, const int y, const char *message) {
    int tmp_x;
    if (message != NULL) {
        tmp_x = get_padding(strlen(message));
        mvprint(x + tmp_x, y, BOLD, "%s", message);
    }

    tmp_x = get_padding(FRAME_SIZE);

    for (size_t i = 0; i <= FRAME_SIZE; ++i) {
        set_console_cursor_position(x + tmp_x, y + 2);
        progress_bar(i, FRAME_SIZE);
        delay(50); //? milliseconds
    }

    mvprint(x + tmp_x + PROMPT_PADDING, y + 4, BOLD, "Press any key to start : ");
    cgetch();
}


/**
 * @brief This function displays a loader while user credentials authenticate.
 *
 * @param x X-coordinate value of the cursor
 * @param y Y-coordinate value of the cursor
 * @param message A pointer to the message to be displayed while the loader is running.
 */
void display_loader(const int x, const int y, const char *message) {
    const char *char_set[] = {"◒", "◐", "◓", "◑"};
    const size_t char_size = sizeof(char_set) / sizeof(char_set[0]);

    if (message != NULL) {
        mvprint(x, y, BOLD, "[%s] %s", LOADING_EMOJI, message);
    }

    for (size_t i = 0; i < FRAME_SIZE; i++) {
        print("%s \b\b", char_set[i % char_size]);
        delay(50); //? sleep 50 milliseconds
    }
}


/**
 * @brief This function uses the getch function and listens for only the arrow keys
 * @param key The value of the key pressed
 * 
 * @returns the integer value of the left/right/up/down keys
 */
int get_arrow_key(int key) {
    if (key != ESC_KEY) {
        return VALUE_ERROR;
    }
    key = cgetch();
    if (key != LEFT_SQB) { //? handle the '[' character
        return VALUE_ERROR;
    }
    key = cgetch();
    switch (key) {
        case UP_KEY:
            return UP_KEY;
        case DOWN_KEY:
            return DOWN_KEY;
        case LEFT_KEY:
            return LEFT_KEY;
        case RIGHT_KEY:
            return RIGHT_KEY;
        default:
            return VALUE_ERROR;
    }
}


/**
 * @brief This function highlights the currnet line that the user scrolled to
 *
 * @param key value of the key pressed to move to the next/previous line
 * @param len The number of lines in the menu list
 * @param line The currently highlighted line number
 * 
 * @return The index value of the text to be highlighted
 */
int highlight_line(const int key, const int len, const int line) {
    switch (key) {
        case UP_KEY :
            return (line - 2 + len) % len; // Allows to move up or down skipping one line on the list

        case LEFT_KEY :
            return (line - 1 + len) % len; // Move to the previous line on the menu list

        case DOWN_KEY :
            return (line + 2) % len; // Allows to move up or down skipping one line on the menu list

        case RIGHT_KEY :
            return (line + 1) % len; // Move to the next line on the menu list

        default :
            return VALUE_ERROR;
    }
}


/**
 * @brief This function displays a banner containing a title text with characters on both sides.
 *
 * @param colors A pointer to the set of RGB colors for styling the text foreground & background
 * @param content A pointer to the heading/text to be displayed in the banner.
 */
void display_banner(const RGBset_t *colors, const char *content) {
    if (colors == NULL || content == NULL) {
        log_error(__func__, __FILE__, __LINE__, "NULL INPUT PARAMETERS!...\n");
        return;
    }
    for (size_t i = 0; i < LEFT_PADDING; i++) {
        if (i % 2 == 0) {
            set_console_color_attr(colors[0]);
            mvprint(NUL, NUL, BOLD, "%s", LBANNER_EMOJI);
        }
        else {
            set_console_color_attr(colors[1]);
            mvprint(NUL, NUL, BOLD, "%s", LBANNER_EMOJI);
        }
    }

    set_console_color_attr(colors[1]);
    mvprint(NUL, NUL, BOLD, "%s", content);

    for (size_t i = 0; i < RIGHT_PADDING; i++) {
        if (i % 2 == 0) {
            set_console_color_attr(colors[0]);
            mvprint(NUL, NUL, BOLD, "%s", RBANNER_EMOJI);
        }
        else {
            set_console_color_attr(colors[1]);
            mvprint(NUL, NUL, BOLD, "%s", RBANNER_EMOJI);
        }
    }
}


/**
 * @brief This function calls the 'display_banner' function and also displays player info on the top of the page
 *
 * @param content A pointer to the content of the banner to be printed
 * @param player A pointer to the 'player stats' structure of the currently logged-in player
 * @param box_offset The offset position of the box, which helps in determining the padding value of the banner and player info
 */
void display_header(const int box_offset, const char *content, pstats_t *player) {
    const RGBset_t colors[] = {
        { //? YELLOW on DARK RED
            .foreground = {255, 255, 0},
            .background = {178, 0, 0}
        },
        { //? BLACK on WHITE
            .foreground = {0, 0, 0},
            .background = {255, 255, 255}
        }
    };

    //? Offset position of the banner
    const int banner_padding = get_padding(3 * (LEFT_PADDING + RIGHT_PADDING) + strlen(content) - 4);
    //? '3' because the emojis on the banner are a combination of 3 different emojis
    //? '-4' because strlen returns '3' for an emoji and the content of the banner has 2 dash emojis

    const int y = HEADER_HEIGHT;
    const int x = box_offset;

    set_console_cursor_position(x + banner_padding, y);
    display_banner(colors, content);

    if (player != NULL) {
        char sub_heading[MIN_BUFF] = {0};

        snprintf(sub_heading, sizeof(sub_heading), "%s PLAYER : %s  🔸  %s ID : %s  🔸  %s SCORE : %03u",
            PROFILE_EMOJI, player->profile.username, ID_EMOJI, player->profile.playerId,
            SPORTS_EMOJI, player->scores.currentScore);

        const int padding = get_padding(strlen(sub_heading) - 10);
        mvprint(x + padding, y + 2, BOLD, "%s", sub_heading);
    }
}


/**
 * @brief This displays the page/items on the terminal. It calls the function to
 *        draw the box, print the banner, the heading, subheading, menu, & other contents on the terminal
 *
 * @param box_offset The offset value/position of the box
 * @param highlight The line number on the menu to be highlighted
 * @param len The size of the menu array contents
 * @param player A pointer to the player stats structure
 * @param heading A pointer to the title/heading of the current page
 * @param sub_heading A pointer to the sub-heading, if any.
 * @param emojis A 2d array of the emojis associated with the menu contents
 * @param menus A 2d array of the menu list to be displayed
 */
void display_main_page(const int box_offset, const size_t highlight, const size_t len, pstats_t *player,
    const char *heading, const char *sub_heading, char emojis[][BADGE_SIZE], char menus[][BUF_SIZE]) {

    const int y_coord = 1;
    const int x_coord = box_offset;
    draw_box(x_coord, y_coord, MIN_BOX_WIDTH, MIN_BOX_HEIGHT);

    display_header(box_offset, heading, player);

    if (sub_heading != NULL) {
        const int padding = get_padding(strlen(sub_heading));
        mvprint(x_coord + padding, y_coord + HEADER_HEIGHT + 2, BOLD,
                "%s", sub_heading);
    }

    print_menu(x_coord, y_coord + HEADER_HEIGHT + 2, highlight, len, emojis, menus);

    display_footer(box_offset);
}


/**
 * @brief This function displays the footer info on the bottom of thw page
 *
 * @param box_offset offset position of the box
 */
void display_footer(const int box_offset) {
    const int y_coord = MIN_BOX_HEIGHT;
    const int x_coord = box_offset + get_padding(strlen(COPYRIGHT) - 4);

    mvprint(x_coord, y_coord, DIM, "%s", COPYRIGHT);
}


/**
 * @brief This function prints the menu/options while highlighting the line for users to select
 *
 * @param highlight The line number to be highlighted
 * @param len The number of lines to be printed
 * @param emoji The emojis associated with each line
 * @param content The content of the menu variable
 * @param x X-coordinate value of the cursor
 * @param y Y-coordinate value of the cursor
 */
void print_menu(const int x, int y, const size_t highlight,
    const size_t len, char emoji[][BADGE_SIZE], char content[][BUF_SIZE]) {

    for (size_t i = 0; i < len; i++) {
        if (i == highlight) {
            mvprint(x + 20, y += 2, INVERT,
                    "%s %s", emoji[i], content[i]);
        } else {
            mvprint(x + 20, y += 2, BOLD,
                    "%s %s", emoji[i], content[i]);
        }

        i++;
        if (i < len) {
            const int tmp_x = MIN_BOX_WIDTH / 2 + 10;
            if (i == highlight) {
                mvprint(x + tmp_x, y, INVERT,
                        "%s %s", emoji[i], content[i]);
            } else {
                mvprint(x + tmp_x, y, BOLD,
                        "%s %s", emoji[i], content[i]);
            }
        }
        fflush(stdout);
    }
}


/**
 * @brief This function displays the account requirements for new users
 *
 * @param x X-coordinate value of the cursor
 * @param y Y-coordinate value of the cursor
 */
void display_requirements(const int x, int y) {
    mvprint(x, y, UNDERLINE,
            " [%s] Username Requirements :                                        ", INFO_EMOJI);
    mvprint(x, y += 1, INVERT, /*swap the fg & bg colors of the terminal*/
            " [%s] Must be atleast %i characters and no more than %i characters   ", INFO_EMOJI, MIN_UNAME_LEN, MAX_UNAME_LEN);
    mvprint(x, y += 1, INVERT,
            " [%s] Can only contain lowercase, digits or an Underscore.           ", INFO_EMOJI);
    mvprint(x, y += 1, INVERT,
            " [%s] Usernames can't start with a digit or an underscore.           ", INFO_EMOJI);

    mvprint(x, y += 1, UNDERLINE,
            " [%s] Password Requirements :", INFO_EMOJI);
    mvprint(x, y += 1, INVERT,
            " [%s] Must be atleast %i characters and no more than %i characters   ", INFO_EMOJI, MIN_PASSWD_LEN, MAX_PASSWD_LEN);
    mvprint(x, y + 1, INVERT,
            " [%s] Must contain both Alphanumeric & Special characters.           ", INFO_EMOJI);
}
