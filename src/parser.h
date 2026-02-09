//
// Created by masonlee on 2026/2/6.
//

#ifndef WEATHER_PARSER_H
#define WEATHER_PARSER_H

typedef struct CommandArgs {
  char *command;
  char *location_name;
  int day_size;
  char *location_id;
} CommandArgs;

void parse_command_args(CommandArgs *command_args, int argc, char *argv[]);

void free_command_args(const CommandArgs *command_args);

typedef struct DayWeather {
  // 日期
  char *fxDate;
  // 日出时间
  char *sunrise;
  // 日落时间
  char *sunset;
  // 最高温度
  char *tempMax;
  // 最低温度
  char *tempMin;
  // 白天天气状况
  char *textDay;
  // 夜间天气状况
  char *textNight;
  // 月亮状况
  char *moonPhase;
  // 白天风向
  char *windDirDay;
  // 白天风速
  char *windSpeedDay;
  // 夜间风向
  char *windDirNight;
  // 夜间风速
  char *windSpeedNight;
  // 降雨量
  char *precip;
  // 相对湿度
  char *humidity;
  // 能见度
  char *vis;
} DayWeather;

typedef struct DayWeathers {
  DayWeather *days;
  int count;
} DayWeathers;

int parse_weather_days_json(DayWeathers *day_weathers, const char *json_str);

void free_day_weather(DayWeather *day_weather);
void free_day_weathers(DayWeathers *day_weathers);

typedef struct Location {
  char *name;
  char *id;
  char *lat;
  char *adm1;
  char *adm2;
  char *country;
  char *tz;
  char *utcOffset;
  char *isDest;
  char *type;
  char *rank;
  char *fxLink;
} Location;

typedef struct Locations {
  Location *locations;
  int count;
} Locations;

int parse_location_json(Locations *locations, const char *json_str);

void free_location(Location *location);
void free_locations(Locations *locations);

#endif // WEATHER_PARSER_H
