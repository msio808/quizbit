#include "../include/db.h"
#include "../include/console.h"


/**
 * @brief Ehh main function
 *        Calls the functions that sets up the signal handlers for SIGINT, SIGSEGV, & SIGWINCH,
 *        Function that stores the default terminal attribute to a termios structure
 *        Calls the setup functionthat set's up the database & initializes all the demo player accounts
 *        Then calls the homepage function if no error is encountered
 *
 * @return Exit Status Code ie: EXIT_SUCCESS '0' or EXIT_FAILURE '1'
 */
int main(void) {
    if (!setup()) {
        return EXIT_FAILURE;
    }

    setlocale(LC_ALL, "en_US.UTF-8");

    initialize_terminal();
    setup_signal_handler();

    int offset_position;
    setup_sigwinch_handler(&offset_position);

    homepage(offset_position);

    return EXIT_SUCCESS;
}