#pragma once

// Colors
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_YELLOW "\x1b[33m"

// Log macros
#define okay(msg, ...) printf("[" ANSI_COLOR_GREEN "+" ANSI_COLOR_RESET "] " msg "\n", ##__VA_ARGS__)

#define info(msg, ...) printf("[" ANSI_COLOR_BLUE "*" ANSI_COLOR_RESET "] " msg "\n", ##__VA_ARGS__)

#define warn(msg, ...) printf("[" ANSI_COLOR_YELLOW "!" ANSI_COLOR_RESET "] " msg "\n", ##__VA_ARGS__)

#define error(msg, ...) fprintf(stderr, "[" ANSI_COLOR_RED "-" ANSI_COLOR_RESET "] " msg "\n", ##__VA_ARGS__)

#define perror_m(msg) perror("[" ANSI_COLOR_RED "-" ANSI_COLOR_RESET "] " msg)
