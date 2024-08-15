/**
 * @file gamemath.h
 * @author Muhammed A. Senghore
 * @brief This header file defines function prototypes related to mathematical operations done in the Quiz Game.
 *        Operations like calculating bonuses to be assigned, game completion/performance rate, and many more.
**/
#ifndef GAMEMATH_H
#define GAMEMATH_H
#pragma once

#include "utilities.h"

uint32_t games_completed(uint32_t);
uint32_t assign_bonus(uint32_t, double);
uint32_t get_lstreaks(uint32_t, uint32_t);
uint32_t get_total_correct_answers(uint32_t);
uint32_t get_total_incorrect_answers(uint32_t);
uint32_t get_highest_score(uint32_t, uint32_t);
uint32_t get_total_question_attempted(uint32_t);
uint32_t get_average_time(uint32_t, double, uint32_t);
uint32_t get_average_score(uint32_t, uint32_t, uint32_t);

double get_percentage(double, double);
double get_completion_rate(uint32_t, uint32_t);
double get_performance_rate(uint32_t, uint32_t);
double get_time_difference(const char *, const char *);

#endif //GAMEMATH_H
