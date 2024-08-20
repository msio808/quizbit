#include "../../include/console.h"
#include "../../include/utilities.h"


/**
 * @brief Function to exit the program, clears the terminal then display a notice message before exiting
 *
 * @param EXIT_CODE The value of the exit status
 */
void exit_program(const int EXIT_CODE) {
    clr_scr();

    set_console_text_attr(RESET_ATTR);
    set_console_cursor_mode(ENABLE);
    restore_terminal();

    mvprint(1, 4, BOLD, "PROGRAM TERMINATED : \n\tAll errors are logged in the file located in %s\n", ERROR_LOGPATH);
    exit(EXIT_CODE);
}


/**
 * @brief This function is a custom sleep function that delays the process in milliseconds using the nanosleep function
 *
 * @param milliseconds The amount of time to delay in milliseconds
 */
void delay(const uint32_t milliseconds) {
    struct timespec sleep_time;

    sleep_time.tv_sec = milliseconds / 1000;
    sleep_time.tv_nsec = milliseconds % 1000 * 1000000L;

    nanosleep(&sleep_time, NULL);
}


/**
 * @brief This function Resets/Clears/Initializes the fields of
 *        the any structure and it's members to zero
 *
 * @param Session A Pointer to the structure whose memory needs to be cleared.
 * @param size The size of the 'session' structure
 */
void struct_memset(void *Session, const size_t size) {
    memset(Session, 0, size);
}


/**
 * @brief This function prompts user for confirmation and only accept Y/N characters.
 *
 * @param prompt A pointer to the prompt/question to be displayed to the user
 * @returns Only Y/N
 */
int get_confirmation(const char *prompt) {
    if (prompt != NULL) {
        print("%s", prompt);
    }
    int c;
    do {
        c = toupper(cgetch());
    } while (c != 'Y' && c != 'N');
    return c;
}


/**
 * @brief This is a custom printf function
 *
 * @param fmt A pointer to the formated string to print to the user
 * @param ... Additional arguments
 */
void print(const char *fmt,...) {
    va_list args;

    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    fflush(stdout);
}


/**
 * @brief This is a custom printf function that prints styled texts to stdout with given X,Y coordinate values
 *
 * @param x_coord X-coordinate of the cursor
 * @param y_coord Y-coordinate of the cursor
 * @param textStyle The style of the text to be printed
 * @param fmt A pointer to the formatted string to be printed.
 * @param ... Additional parameters to support variable argument list formatting.
 */
void mvprint(const int x_coord, const int y_coord, const styles_t textStyle, const char *fmt,...) {
    if (XY_VALID(x_coord, y_coord)) {
        set_console_cursor_position(x_coord, y_coord);
    }
    if (TS_VALID(textStyle)) {
        set_console_text_attr(textStyle);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    set_console_text_attr(RESET_ATTR);
    fflush(stdout);
}


/**
 * @brief Prints colored text on the screen when the code/functions encounter errors.
 *
 * @param x_coord X-coordinate of the cursor
 * @param y_coord Y-coordinate of the cursor
 * @param error A pointer to the error message to be printed.
 * @param ... Additional parameters to support variable argument list formatting.
 */
void print_error(const int x_coord, const int y_coord, const char *error,...) {
    va_list args;
    va_start(args, error);

    const RGBset_t color_codes = {
        .foreground = {255, 255, 255}, //? Foreground color code (White)
        .background = {255, 0, 0}      //? Background color code (Red)
    };

    set_console_text_attr(BOLD);
    set_console_color_attr(color_codes);

    set_console_cursor_position(x_coord, y_coord);
    fprintf(stderr, " [%s] ", ERROR_EMOJI);

    vfprintf(stderr, error, args);
    va_end(args);

    set_console_text_attr(RESET_ATTR);
    fflush(stdout);
}


/**
 * @brief Prints colored text on the screen for successful operations.
 *
 * @param x_coord X-coordinate of the cursor
 * @param y_coord Y-coordinate of the cursor
 * @param success A pointer to the success message to be printed.
 * @param ... Additional parameters to support variable argument list formatting.
 */
void print_success(const int x_coord, const int y_coord, const char *success,...) {
    va_list args;
    va_start(args, success);

    const RGBset_t color_codes = {
        .foreground = {255, 255, 255}, //? Foreground color code (White)
        .background = {0, 115, 2}      //? Background color code (green)
    };

    set_console_text_attr(BOLD);
    set_console_color_attr(color_codes);

    set_console_cursor_position(x_coord, y_coord);
    fprintf(stdout, " [%s] ", SUCCESS_EMOJI);

    vfprintf(stdout, success, args);
    va_end(args);

    set_console_text_attr(RESET_ATTR);
    fflush(stdout);
}


/**
 * @brief This function logs error messages to a log file
 *
 * @param __FUNC A pointer to the function where the error is encountered
 * @param __FILE A pointer to the file name where the error is encountered
 * @param __LINE A pointer to the line number where the error is encountered
 * @param fmt A pointer to the error message to log into the log file
 * @param ... Additional arguments
 */
void log_error(const char *__FUNC, const char *__FILE, const int __LINE, const char *fmt, ...) {
    FILE *logFile = fopen(ERROR_LOGPATH, "a");
    if (NULL == logFile) {
        mvprint(1, 1, BOLD, "Could not open log file : Quitting...");
        cgetch();
        exit_program(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, fmt);

    const char *str = strrchr(__FILE, '/') ? strrchr(__FILE, '/') + 1 : __FILE;
    char *time_buffer = get_current_datetime();
    fprintf(logFile, "FILE : %s \n"
                           "LINE : %i \n"
                           "FUNC : %s \n"
                           "TIME : %s \n"
                           "ERROR MESSAGE ━━━ ", str, __LINE, __FUNC, time_buffer);
    vfprintf(logFile, fmt, args);
    fprintf(logFile, "\n");

    va_end(args);
    fclose(logFile);
    free(time_buffer);
}


/**
 * @brief This function gets the current date & time and stores it in a formatted string.
 *
 * @returns A dynamically allocated pointer to the currently date & time, caller is responsible for freeing the memory
 */
char *get_current_datetime(void) {
    time_t t;
    time(&t);
    char *formattedDatetime = calloc(DATE_SIZE + 1, sizeof(char));
    if (formattedDatetime != NULL) {
        strftime(formattedDatetime, DATE_SIZE, "%d-%b-%Y %I:%M %p", localtime(&t));
    }
    return formattedDatetime;
}


/**
 * @author Gavin A. S. Cheng
 * @see https://ascheng.medium.com/linux-getch-for-unix-c2c829721a30
 *
 * @brief This function is a custom getch function for linux
 *        The function is cited from a blog post on medium.com
 *        check the link above for reference.
 *
 * @returns the character that was read by the function
 */
int cgetch(void) {
    //? Declare termios structures and store the current terminal settings and store them in old_tio
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);

    //? Modify new_tio to disable canonical mode & echo and apply the settings to the terminal
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    //? Read a character, restore the original terminal settings & return the read character
    const int c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    return c;
}
