//
// Created by masonlee on 2026/2/4.
//

#ifndef WEATHER_API_H
#define WEATHER_API_H

typedef struct MemoryStruct {
  char *memory;
  size_t size;
} MemoryStruct;

char *get_weather_json_by_days(const char *token);

#endif // WEATHER_API_H
