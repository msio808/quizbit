/**
 * @file console.h
 * @author Muhammed A. Senghore
 * @brief This header file contains functions, structures, & constant values for terminal control functions.
**/
#ifndef CONSOLE_H
#define CONSOLE_H
#pragma once

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>

// TODO : Flag to ignore the set_console_sursor_position in the mvprint function
typedef enum COORDINATES {
    NUL = -0x00000001
} coords_t;

// TODO : For terminal utilities like enabling/disabling the cursor, min/max size of the terminal, and many more
typedef enum TERMUTILS {
    ENABLE         =  0x00000000,
    DISABLE        =  0x00000001,
    IOCTL_ERROR    = -0x00000001,
    WINSIZ_ERROR   =  0x00000001,
    HEADER_HEIGHT  =  0x00000002,
    FOOTER_HEIGHT  =  0x00000001,
    MIN_WIN_WIDTH  =  0x00000064,
    MIN_BOX_WIDTH  =  0x0000005A,
    MIN_BOX_HEIGHT =  0x0000001E,
    MIN_WIN_HEIGHT =  0x00000023,
    MIN_OFFSET_POS =  0x00000005,
} tstats_t;

// TODO : For styling output texts both foreground and background
typedef enum TEXT_STYLES {
    RAW        = -0x00000001,
    DIM        =  0x00000002,
    BOLD       =  0x00000001,
    HIDDEN     =  0x00000008,
    INVERT     =  0x00000007,
    ITALIC     =  0x00000003,
    UNDERLINE  =  0x00000004,
    RESET_ATTR =  0x00000000,
} styles_t;

// TODO : Flags for styling/coloring function & functions to convert from RGB values to 256 bit mode
typedef enum COLOR_MODES {
    RGB_MAX     = 0x000000FF, //? Maximum RGB value
    BIT_MODE    = 0x00000005, //? 256 bit mode
    BIT_OFFSET  = 0x00000010,

    R_FACTOR    = 0x00000024,
    G_FACTOR    = 0x00000006,
    B_FACTOR    = 0x00000001,

    FOREGROUND  = 0x00000026,
    BACKGROUND  = 0x00000030
} cmodes_t;

// TODO : Struct for holding rgb color values
typedef struct RGB_COLOR {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGB_t;

// TODO : Struct for holding rgb color set of the 'RGB_t' struct for styling text on the terminal
typedef struct TERM_COLORS {
    RGB_t foreground;
    RGB_t background;
} RGBset_t;

// TODO : Global variable for storing the terminal attributes at the start of the program
extern struct termios default_term_attr;

// TODO : Macros for checking validity of the values passed to the text_styling & cursor positioning functions
#define TS_VALID(style) (style > RAW)
#define XY_VALID(X, Y) (X > NUL && Y > NUL)

void clr_scr(void);
void del_char(size_t);
void set_fg_color(uint8_t);
void set_bg_color(uint8_t);

void signal_handler(int);
void handle_sigwinch(int);
void setup_signal_handler(void);
int  get_win_size(int *, int *);
void setup_sigwinch_handler(int *);

int get_arrow_key(int);
int highlight_line(int, int, int);

void restore_terminal(void);
int  check_console_req(void);
void get_box_offset_pos(int);
void initialize_terminal(void);
void set_console_text_attr(int);
void set_console_cursor_mode(int);
void draw_box(int, int, int, int);
void set_console_color_attr(RGBset_t);
void set_console_cursor_position(int, int);

#endif //CONSOLE_H
