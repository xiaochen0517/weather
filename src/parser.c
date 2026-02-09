//
// Created by masonlee on 2026/2/6.
//

#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "utils.h"

#include <string.h>

void parse_command_args(CommandArgs *command_args, const int argc,
                        char *argv[]) {
  for (int i = 0; i < argc; i++) {
    // 这里可以添加对命令行参数的处理逻辑
    w_log("Command line argument %d: %s\n", i, argv[i]);
  }
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <location|days> <args>\n", argv[0]);
    exit(1);
  }
  const char *command = argv[1];
  command_args->command = str_duplicate(command);
  if (strcmp(command, "location") == 0) {
    w_log("Parsing location command.\n");
    // 处理 location 命令的逻辑
    if (argc < 3) {
      fprintf(stderr, "Usage: %s location <location_name>\n", argv[0]);
      exit(1);
    }
    command_args->location_name = str_duplicate(argv[2]);
  } else if (strcmp(command, "days") == 0) {
    w_log("Parsing days command.\n");
    // 处理 days 命令的逻辑
    if (argc < 4) {
      fprintf(stderr, "Usage: %s days <day_size> <location_id>\n", argv[0]);
      exit(1);
    }
    const int day_size = str_to_int(argv[2]);
    command_args->day_size = day_size;
    command_args->location_id = str_duplicate(argv[3]);
  } else {
    fprintf(stderr, "Unknown command: %s\n", command);
    exit(1);
  }
}

void free_command_args(const CommandArgs *command_args) {
  if (command_args == NULL) {
    return;
  }
  if (command_args->command != NULL) {
    free(command_args->command);
  }
  if (command_args->location_name != NULL) {
    free(command_args->location_name);
  }
  if (command_args->location_id != NULL) {
    free(command_args->location_id);
  }
}

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

int parse_location_json(Locations *locations, const char *json_str) {
  json_error_t json_error;
  json_t *data = json_loads(json_str, 0, &json_error);

  if (!data) {
    fprintf(stderr, "Error parsing JSON: %s\n", json_error.text);
    exit(1);
  }

  json_t *location_array = json_object_get(data, "location");
  // 检查 location_array 是否为数组
  if (!json_is_array(location_array)) {
    fprintf(stderr, "Error: 'location' is not an array.\n");
    json_decref(data);
    exit(1);
  }

  // 获取数组大小
  size_t array_size = json_array_size(location_array);
  locations->locations = malloc(sizeof(Location) * array_size);
  // 遍历数组
  for (size_t i = 0; i < array_size; i++) {
    json_t *loc_json = json_array_get(location_array, i);
    Location location = {0};

    location.name =
        str_duplicate(json_string_value(json_object_get(loc_json, "name")));
    location.id =
        str_duplicate(json_string_value(json_object_get(loc_json, "id")));
    location.lat =
        str_duplicate(json_string_value(json_object_get(loc_json, "lat")));
    location.adm1 =
        str_duplicate(json_string_value(json_object_get(loc_json, "adm1")));
    location.adm2 =
        str_duplicate(json_string_value(json_object_get(loc_json, "adm2")));
    location.country =
        str_duplicate(json_string_value(json_object_get(loc_json, "country")));
    location.tz =
        str_duplicate(json_string_value(json_object_get(loc_json, "tz")));
    location.utcOffset = str_duplicate(
        json_string_value(json_object_get(loc_json, "utcOffset")));
    location.isDest =
        str_duplicate(json_string_value(json_object_get(loc_json, "isDst")));
    location.type =
        str_duplicate(json_string_value(json_object_get(loc_json, "type")));
    location.rank =
        str_duplicate(json_string_value(json_object_get(loc_json, "rank")));
    location.fxLink =
        str_duplicate(json_string_value(json_object_get(loc_json, "fxLink")));

    // 扩展 locations 数组
    locations->locations[locations->count] = location;
    locations->count += 1;
  }

  json_decref(data);

  return (int)array_size;
}

void free_location(Location *location) {
  if (location == NULL) {
    return;
  }
  free(location->name);
  free(location->id);
  free(location->lat);
  free(location->adm1);
  free(location->adm2);
  free(location->country);
  free(location->tz);
  free(location->utcOffset);
  free(location->isDest);
  free(location->type);
  free(location->rank);
  free(location->fxLink);
}

void free_locations(Locations *locations) {
  if (locations == NULL) {
    return;
  }
  for (int i = 0; i < locations->count; i++) {
    free_location(&locations->locations[i]);
  }
  free(locations->locations);
}
