//
// Created by masonlee on 2026/2/6.
//

#include <stdio.h>
#include <string.h>

#include "output.h"

#define RAIN_ICON "\U0001F327"
#define SUN_ICON "\u2600\uFE0F"
#define DAY_ICON "\U0001F31E"
#define NIGHT_ICON "\U0001F319"
#define THERMOMETER_ICON "\U0001F321"

void print_weather_days(const DayWeathers *day_weathers) {
  for (int i = 0; i < day_weathers->count; i++) {
    const DayWeather day_weather = day_weathers->days[i];
    printf("%s  ", day_weather.fxDate);
    printf("%s:%3s℃ ~ %3s℃ ", THERMOMETER_ICON, day_weather.tempMax,
           day_weather.tempMin);
    printf("%s:%s %s:%s ", DAY_ICON, day_weather.sunrise, NIGHT_ICON,
           day_weather.sunset);
    if (strcmp(day_weather.precip, "0.0") == 0) {
      printf("%s 无雨", SUN_ICON);
    } else {
      printf("%s %smm ", RAIN_ICON, day_weather.precip);
    }
    printf("\n");
  }
}
