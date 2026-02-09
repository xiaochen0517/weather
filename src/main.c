#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "jansson.h"
#include "jwt.h"
#include "output.h"
#include "parser.h"
#include "utils.h"

#include <string.h>

char *get_token(void) {
  char *kid_str = getenv("W_API_KID");
  if (!kid_str) {
    fprintf(stderr, "Please set API_KID environment variable.\n");
    exit(1);
  }
  char *sub_str = getenv("W_API_SUB");
  if (!sub_str) {
    fprintf(stderr, "Please set API_SUB environment variable.\n");
    exit(1);
  }

  const int64_t expiration_ms = 60 * 60; // 1 hour
  char *jwt_token = get_jwt_token(kid_str, sub_str, expiration_ms,
                                  "/home/masonlee/.ssh/ed25519-private.pem");
  w_log("Token: %s\n", jwt_token);

  return jwt_token;
}

void get_location(const CommandArgs *command_args) {
  printf("Loading location data...\n");
  char *jwt_token = get_token();
  char *location_json = get_geoid_by_location(command_args->location_name, jwt_token);
  free(jwt_token);

  Locations *locations = malloc(sizeof(Locations));
  parse_location_json(locations, location_json );
  free(location_json );
  for (int i = 0; i < locations->count; i++) {
    const Location loc = locations->locations[i];
    w_log(
        "Name: %s, ID: %s, Lat: %s, Adm1: %s, Adm2: %s, Country: %s, Tz: %s\n",
        loc.name, loc.id, loc.lat, loc.adm1, loc.adm2, loc.country, loc.tz);
  }

  free_locations(locations);
  free(locations);
}

void days_weather(const CommandArgs *command_args) {
  // 获取天气数据
  printf("Loading weather data...\n");
  char *jwt_token = get_token();
  char *days_json = get_weather_json_by_days(command_args->day_size, command_args->location_id, jwt_token);
  free(jwt_token);
  // 开始解析
  DayWeathers *day_weathers = malloc(sizeof(DayWeathers));
  parse_weather_days_json(day_weathers, days_json);
  free(days_json);
  // 测试输出
  for (int i = 0; i < day_weathers->count; i++) {
    const DayWeather day = day_weathers->days[i];
    w_log("Date: %s, Max Temp: %s, Min Temp: %s, Day Weather: %s, Night "
          "Weather: %s\n",
          day.fxDate, day.tempMax, day.tempMin, day.textDay, day.textNight);
  }

  print_weather_days(day_weathers);

  // 释放资源
  free_day_weathers(day_weathers);
  free(day_weathers);
}

int main(const int argc, char *argv[]) {
  // 初始化配置
  w_config_init();

  CommandArgs *command_args = malloc(sizeof(CommandArgs));
  parse_command_args(command_args, argc, argv);

  if (strcmp(command_args->command, "location") == 0) {
    get_location(command_args);
  } else if (strcmp(command_args->command, "days") == 0) {
    days_weather(command_args);
  } else {
    fprintf(stderr, "Unknown command: %s\n", command_args->command);
    exit(1);
  }

  free_command_args(command_args);

  return 0;
}
