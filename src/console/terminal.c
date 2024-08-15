#include "../../include/console.h"
//#include "../../include/gamecore.h"
#include "../../include/utilities.h"

struct termios default_term_attr;
/**
 * @author Muhammed A. Senghore
 * @brief This function stores the default attributes of the terminal to the above termios structure
 */
void initialize_terminal(void) {
    if (tcgetattr(STDIN_FILENO, &default_term_attr) == VALUE_ERROR) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
}


/**
 * @author Muhammed A. Senghore
 * @brief This Function restores the default attributes of the terminal after the program terminates
 */
void restore_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &default_term_attr);
}


/**
 * @author Muhammed A. Senghore
 * @brief This function handles the SIGSEGV/SIGINT signal (caused by pressing CTRL+C).
 *        It resets the terminal attributes back to default when SIGINT/SIGSEGV is received.
 * @param signal signal
 */
void signal_handler(const int signal) {
    if (signal == SIGINT) {
        log_error(__func__, __FILE__, __LINE__, "Program Abruptly terminated.\n");
        exit_program(EXIT_SUCCESS);
    }
    if (signal == SIGSEGV) {
        void *array[BUF_SIZE];
        const size_t size = backtrace(array, BUF_SIZE);

        log_error(__func__, __FILE__, __LINE__, "SIGNAL %i (Segmentation fault) received.\n", signal);

        const int fd = open(ERROR_LOGPATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == VALUE_ERROR) {
            log_error(__func__, __FILE__, __LINE__, "Error opening log file");
            exit_program(EXIT_FAILURE);
        }

        dprintf(fd, "Error : signal %d (Segmentation fault) received.\n", signal);
        backtrace_symbols_fd(array, size, fd);
        close(fd);

        exit_program(EXIT_FAILURE);
    }
}


/**
 * @author Muhammed A. Senghore
 * @brief This function set's up and calls the 'signal_handler' function which listens for 'SIGINT'
 */
void setup_signal_handler(void) {
    if (signal(SIGINT, signal_handler) == SIG_ERR || signal(SIGSEGV, signal_handler) == SIG_ERR) {
        print("Error setting up signal handler");
        exit_program(EXIT_FAILURE);
    }
}


/**
 * @author Muhammed A. Senghore
 * @brief This function uses escape sequence '\033c', to reset
 *        the terminal to its initial state (ie: clear the terminal).
**/
void clr_scr(void) {
    print("\033c");
    set_console_cursor_mode(DISABLE);
}


/**
 * @author Muhammed A. Senghore
 * @brief This function uses ANSI escape secuence to move the cursor to the given X-Y coordinates
 *
 * @param x_coord X-coordinate of the terminal
 * @param y_coord Y-coordinate of the terminal
 */
void set_console_cursor_position(const int x_coord, const int y_coord) {
    print("\033[%i;%iH", y_coord, x_coord);
}


/**
 * @author Muhammed A. Senghore
 * @brief This function uses ansi escape sequence to enable/disable cursor on the terminal
 *
 * @param mode Mode to determine whether to enable/disable the cursor
 */
void set_console_cursor_mode(const int mode) {
    print(mode == ENABLE ? "\033[?25h" : "\033[?25l");
}


/**
 * @author Muhammed A. Senghore
 * @brief This function sets the terminal foreground color to given rgb values
 *
 * @param color rgb color code of the foreground
 */
void set_fg_color(const uint8_t color) {
    print("\033[%i;%i;%hhum", FOREGROUND, BIT_MODE, color);
}


/**
 * @author Muhammed A. Senghore
 * @brief This function sets the terminal background color to given rgb values
 *
 * @param color rgb color code of the background
 */
void set_bg_color(const uint8_t color) {
    print("\033[%i;%i;%hhum", BACKGROUND, BIT_MODE, color);
}


/**
 * @author Muhammed A. Senghore
 * @brief This function styles the terminal text attributes
 *
 * @param textStyle text mode/style
 */
void set_console_text_attr(const int textStyle) {
    switch (textStyle) {
        case DIM :
            print("\033[%im\033[%im", BOLD, DIM);
        break;

        case BOLD:
            print("\033[%im", BOLD);
        break;

        case INVERT :
            print("\033[%im\033[%im", BOLD, INVERT);
        break;

        default :
            print("\033[%im", textStyle);
        break;
    }
}


/**
 * @author Muhammed A. Senghore
 * @brief This function sets the terminal foreground & background color to the given set of RGB values
 *
 * @param colors : rgb color set of the foreground and background
 */
void set_console_color_attr(const RGBset_t colors) {
    set_fg_color(rgb_256(colors.foreground));
    set_bg_color(rgb_256(colors.background));
}


/**
 * @author Muhammed A. Senghore
 * Function to delete 'n' number of character from the terminal
 * @param n The number of characters to delete
 */
void del_char(const size_t n) {
    for (size_t i = 0; i < n; i++) {
        //? Move back one character, Overwrite it with a space, then move back again to position it correctly
        putchar('\b');
        putchar(' ');
        putchar('\b');
    }
}


/**
 * @author Muhammed A. Senghore
 * @brief This function draws a box on the terminal at start
 *
 * @param x_coord X-coordinate of the cursor
 * @param y_coord Y-coordinate of the cursor
 * @param width Width of the box
 * @param height Height of the box
 */
void draw_box(const int x_coord, const int y_coord, const int width, const int height) {
    clr_scr();
    mvprint(x_coord, y_coord, BOLD, "%s", TOP_LEFT_CORNER);
    for (int i = 1; i < width - 1; ++i) {
        mvprint(NUL, NUL, BOLD, "%s", DOUBLE_HOR_BAR);
    }
    mvprint(NUL, NUL, BOLD, "%s", TOP_RIGHT_CORNER);

    for (int i = 1; i < height; ++i) {
        if (i == 2) {
            mvprint(x_coord, y_coord + i, BOLD, "╞");
            for (int j = 1; j < width - 1; j++) {
                mvprint(NUL, NUL, BOLD, "%s", DOUBLE_HOR_BAR);
            }
            mvprint(NUL, NUL, BOLD, "╡");
        }
        else if (i == height - 2) {
            mvprint(x_coord, y_coord + i, BOLD, "├");
            for (int j = 1; j < width - 1; j++) {
                mvprint(NUL, NUL, BOLD, "%s", SINGLE_HOR_BAR);
            }
            mvprint(NUL, NUL, BOLD, "┤");
        }
        else {
            mvprint(x_coord, y_coord + i, BOLD, "%s", VERTICAL_BAR);
            mvprint(x_coord + width - 1, y_coord + i, BOLD, "%s", VERTICAL_BAR);
        }
    }
    
    mvprint(x_coord, y_coord + height, BOLD, "%s", BOTTOM_LEFT_CORNER);
    for (int i = 1; i < width - 1; ++i) {
        mvprint(NUL, NUL, BOLD, "%s", SINGLE_HOR_BAR);
    }
    mvprint(NUL, NUL, BOLD, "%s\n", BOTTOM_RIGHT_CORNER);
}


/**
 * @author Muhammed A. Senghore
 * @brief This function get's the size of the terminal windo
 *
 * @param rows height of the window
 * @param cols width of the window
 *
 * @return 0 if the function executes successfully
 */
int get_win_size(int *rows, int *cols) {
    struct winsize win;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win) == VALUE_ERROR) {
        return IOCTL_ERROR;
    }
    *rows = win.ws_row;
    *cols = win.ws_col;

    return VALUE_SUCCESS;
}


/**
 * @author Muhammed A. Senghore
 * @brief This function detects whenever the window is resized and exits if the size is
 *        less than the minimum height and width oof the terminal window
 *
 * @param sig no use
 */
void handle_sigwinch(const int sig) {
    (void)sig;
    int rows, cols;

    if (get_win_size(&rows, &cols) == IOCTL_ERROR) {
        print("\n Failed to get win size!...");
        exit_program(EXIT_FAILURE);
    }

    if (rows <= MIN_WIN_HEIGHT || cols <= MIN_WIN_WIDTH) {
        log_error(__func__, __FILE__, __LINE__, "The program can only run on %i terminal width and %i height!...", MIN_WIN_WIDTH, MIN_WIN_HEIGHT);
        clr_scr();
        mvprint(1, 3, BOLD, "PROGRAM TERMINATED : \n\t MAKE SURE THE TERMINAL IS OPEN IN FULL SCREEN\n");
        cgetch();
        exit_program(EXIT_FAILURE);
    }
}


/**
 * @author Muhammed A. Senghore
 * @brief THis function sets up the sigwinch handler and set the appropriate value for the offset position of the box
 *
 * @param offset_pos The variable to hold the offset position of the box to be printed
 */
void setup_sigwinch_handler(int *offset_pos) {
    struct sigaction sa;
    sa.sa_handler = handle_sigwinch;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGWINCH, &sa, NULL);

    int rows, cols;
    if (get_win_size(&rows, &cols) == IOCTL_ERROR) {
        print("\n Failed to get win size!...");
        exit_program(EXIT_FAILURE);
    }

    *offset_pos = (cols - MIN_BOX_WIDTH) / 2;
    if (rows <= MIN_WIN_HEIGHT || cols <= MIN_WIN_WIDTH) {
        log_error(__func__, __FILE__, __LINE__, "The program can only run on %i terminal width and %i height!...", MIN_WIN_WIDTH, MIN_WIN_HEIGHT);
        exit_program(EXIT_FAILURE);
    }
}
