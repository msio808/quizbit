/**
 * @file utilities.h
 * @author my-self
 * @brief This header file defines utility functions for various tasks needed in the Game.
 *        Including functions for terminal control, user input handling, password hashing,
 *        some database operations, and username/password validation/authentication.
 */
#ifndef UTILITIES_H
#define UTILITIES_H
#pragma once

#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <sqlite3.h>
#include <termios.h>
#include <stdbool.h>
#include <execinfo.h>
#include <sys/ioctl.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include "gamecore.h"
#include "console.h"
#include "global.h"
#include "emojis.h"

#define MEMCHECH_LOGS "../logs/memcheck.txt"
#define ERROR_LOGPATH "../logs/errorlogs.log"

typedef struct PLAYERSTATS pstats_t;

int  cgetch(void);
void delay(uint32_t);
void exit_program(int);
int  get_padding(size_t);
void struct_memset(void *, size_t);
void display_requirements(int, int);
int  get_confirmation(const char *);

bool is_valid_uname(const char *);
bool is_valid_passwd(const char *);
bool is_username_taken(const char *);
bool is_login_valid(const char *, const char *);
bool valid_credentials(const char *, const char *);
bool regex_match(regex_t *, const char *, const char *);

uint8_t rgb_256(RGB_t);
char *get_current_datetime(void);
char *sha256_hashpass(const char *);
ssize_t input(const char *, char *, size_t);
ssize_t get_pass(const char *, char *, size_t);

void progress_bar(size_t, size_t);
void display_help(int, pstats_t *);
void display_loader(int, int, const char *);
void display_progress_bar(int, int, const char *);

void display_footer(int);
void display_header(int, const char *, pstats_t *);
void display_banner(const RGBset_t *, const char *);
void print_menu(int, int, size_t, size_t, char [][BADGE_SIZE], char [][BUF_SIZE]);
void display_main_page(int, size_t, size_t, pstats_t *, const char *, const char *, char [][BADGE_SIZE], char [][BUF_SIZE]);

void print(const char *,...) __attribute__((format(printf, 1, 2)));
void print_error(int, int, const char *,...) __attribute__((format(printf, 3, 4)));
void print_success(int, int, const char *,...) __attribute__((format(printf, 3, 4)));
void mvprint(int, int, int, const char *,...) __attribute__((format(printf, 4, 5)));
void log_error(const char *, const char *, int, const char *,...) __attribute__((format(printf, 4, 5)));


#endif //UTILITIES_H
