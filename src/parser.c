//
// Created by masonlee on 2026/2/6.
//

#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "utils.h"

int parse_weather_days_json(DayWeathers *day_weathers, const char *json_str) {
  json_error_t json_error;
  json_t *data = json_loads(json_str, 0, &json_error);

  if (!data) {
    fprintf(stderr, "Error parsing JSON: %s\n", json_error.text);
    exit(1);
  }

  json_t *daily_array = json_object_get(data, "daily");
  // 检查 daily_array 是否为数组
  if (!json_is_array(daily_array)) {
    fprintf(stderr, "Error: 'daily' is not an array.\n");
    json_decref(data);
    exit(1);
  }

  // 获取数组大小
  size_t array_size = json_array_size(daily_array);
  day_weathers->days = malloc(sizeof(DayWeather) * array_size);
  // 遍历数组
  for (size_t i = 0; i < array_size; i++) {
    json_t *day_json = json_array_get(daily_array, i);
    DayWeather day_weather = {0};

    day_weather.fxDate =
        str_duplicate(json_string_value(json_object_get(day_json, "fxDate")));
    day_weather.sunrise =
        str_duplicate(json_string_value(json_object_get(day_json, "sunrise")));
    day_weather.sunset =
        str_duplicate(json_string_value(json_object_get(day_json, "sunset")));
    day_weather.tempMax =
        str_duplicate(json_string_value(json_object_get(day_json, "tempMax")));
    day_weather.tempMin =
        str_duplicate(json_string_value(json_object_get(day_json, "tempMin")));
    day_weather.textDay =
        str_duplicate(json_string_value(json_object_get(day_json, "textDay")));
    day_weather.textNight = str_duplicate(
        json_string_value(json_object_get(day_json, "textNight")));
    day_weather.moonPhase = str_duplicate(
        json_string_value(json_object_get(day_json, "moonPhase")));
    day_weather.windDirDay = str_duplicate(
        json_string_value(json_object_get(day_json, "windDirDay")));
    day_weather.windSpeedDay = str_duplicate(
        json_string_value(json_object_get(day_json, "windSpeedDay")));
    day_weather.windDirNight = str_duplicate(
        json_string_value(json_object_get(day_json, "windDirNight")));
    day_weather.windSpeedNight = str_duplicate(
        json_string_value(json_object_get(day_json, "windSpeedNight")));
    day_weather.precip =
        str_duplicate(json_string_value(json_object_get(day_json, "precip")));
    day_weather.humidity =
        str_duplicate(json_string_value(json_object_get(day_json, "humidity")));
    day_weather.vis =
        str_duplicate(json_string_value(json_object_get(day_json, "vis")));

    // 扩展 days 数组
    day_weathers->days[day_weathers->count] = day_weather;
    day_weathers->count += 1;
  }

  json_decref(data);

  return (int)array_size;
}

void free_day_weather(DayWeather *day_weather) {
    if (day_weather == NULL) {
        return;
    }
    free(day_weather->fxDate);
    free(day_weather->sunrise);
    free(day_weather->sunset);
    free(day_weather->tempMax);
    free(day_weather->tempMin);
    free(day_weather->textDay);
    free(day_weather->textNight);
    free(day_weather->moonPhase);
    free(day_weather->windDirDay);
    free(day_weather->windSpeedDay);
    free(day_weather->windDirNight);
    free(day_weather->windSpeedNight);
    free(day_weather->precip);
    free(day_weather->humidity);
    free(day_weather->vis);
}

void free_day_weathers(DayWeathers *day_weathers) {
    if (day_weathers == NULL) {
        return;
    }
    for (int i = 0; i < day_weathers->count; i++) {
        free_day_weather(&day_weathers->days[i]);
    }
    free(day_weathers->days);
}

