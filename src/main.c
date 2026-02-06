#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "jwt.h"
#include "output.h"
#include "parser.h"
#include "utils.h"

char *get_weather(void) {
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

  char *days_json = get_weather_json_by_days(jwt_token);

  free(jwt_token);
  return days_json;
}

int main(void) {
  // 初始化配置
  w_config_init();
  // 获取天气数据
  printf("Loading weather data...\n");
  char *json_str = get_weather();
  // 开始解析
  DayWeathers *day_weathers = malloc(sizeof(DayWeathers));
  parse_weather_days_json(day_weathers, json_str);
  free(json_str);
  // 测试输出
  for (int i = 0; i < day_weathers->count; i++) {
    const DayWeather day = day_weathers->days[i];
    w_log("Date: %s, Max Temp: %s, Min Temp: %s, Day Weather: %s, Night Weather: %s\n",
           day.fxDate, day.tempMax, day.tempMin, day.textDay, day.textNight);
  }

  print_weather_days(day_weathers);

  // 释放资源
  free_day_weathers(day_weathers);
  free(day_weathers);
  return 0;
}
