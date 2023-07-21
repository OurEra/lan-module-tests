#ifndef LITTLE_HELP_H
#define LITTLE_HELP_H
#include <stdio.h>

#define LOG_COLOR_RED(format, ...) \
  printf("\033[0;31m" format "\033[0m\n", ##__VA_ARGS__)
#define LOG_COLOR_BLUE(format, ...) \
  printf("\033[0;34m" format "\033[0m\n", ##__VA_ARGS__)
#define LOG_COLOR_GREEN(format, ...) \
  printf("\033[0;32m" format "\033[0m\n", ##__VA_ARGS__)

#endif // LITTLE_HELP_H