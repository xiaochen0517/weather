//
// Created by masonlee on 2026/2/4.
//

#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <stdio.h>

typedef struct Weather {
  char *name;
} Weather;

typedef struct MemoryStruct {
  char *memory;
  size_t size;
} MemoryStruct;

void get_weather_by_day(Weather *weather, const char *token);

#endif // WEATHER_API_H
