#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "jansson.h"
#include "jwt.h"
#include "utils.h"


char *get_weather() {
  char *kid_str = getenv("API_KID");
  if (!kid_str) {
    fprintf(stderr, "Please set API_KID environment variable.\n");
    exit(1);
  }
  char *sub_str = getenv("API_SUB");
  if (!sub_str) {
    fprintf(stderr, "Please set API_SUB environment variable.\n");
    exit(1);
  }

  const int64_t expiration_ms = 60 * 60; // 1 hour
  char *jwt_token = get_jwt_token(kid_str, sub_str, expiration_ms,
                                  "/home/masonlee/.ssh/ed25519-private.pem");
  printf("Token: %s\n", jwt_token);

  Weather weather = {0};
  get_weather_by_day(&weather, jwt_token);

  free(jwt_token);
}

int main(void) {
  const char *json_str =
    "{\"code\":\"200\",\"updateTime\":\"2026-02-05T16:42+08:00\",\"fxLink\":"
    "\"https://www.qweather.com/weather/"
    "beijing-101010100.html\",\"daily\":[{\"fxDate\":\"2026-02-05\","
    "\"sunrise\":\"07:20\",\"sunset\":\"17:38\",\"moonrise\":\"21:26\","
    "\"moonset\":\"09:03\",\"moonPhase\":\"亏凸月\",\"moonPhaseIcon\":\"805\","
    "\"tempMax\":\"2\",\"tempMin\":\"-8\",\"iconDay\":\"101\",\"textDay\":"
    "\"多云\",\"iconNight\":\"104\",\"textNight\":\"阴\",\"wind360Day\":\"0\","
    "\"windDirDay\":\"北风\",\"windScaleDay\":\"1-3\",\"windSpeedDay\":\"16\","
    "\"wind360Night\":\"45\",\"windDirNight\":\"东北风\",\"windScaleNight\":"
    "\"1-3\",\"windSpeedNight\":\"3\",\"humidity\":\"19\",\"precip\":\"0.0\","
    "\"pressure\":\"1029\",\"vis\":\"25\",\"cloud\":\"0\",\"uvIndex\":\"1\"},{"
    "\"fxDate\":\"2026-02-06\",\"sunrise\":\"07:19\",\"sunset\":\"17:39\","
    "\"moonrise\":\"22:29\",\"moonset\":\"09:23\",\"moonPhase\":\"亏凸月\","
    "\"moonPhaseIcon\":\"805\",\"tempMax\":\"-2\",\"tempMin\":\"-9\","
    "\"iconDay\":\"101\",\"textDay\":\"多云\",\"iconNight\":\"151\","
    "\"textNight\":\"多云\",\"wind360Day\":\"45\",\"windDirDay\":\"东北风\","
    "\"windScaleDay\":\"1-3\",\"windSpeedDay\":\"3\",\"wind360Night\":\"45\","
    "\"windDirNight\":\"东北风\",\"windScaleNight\":\"1-3\",\"windSpeedNight\":"
    "\"3\",\"humidity\":\"27\",\"precip\":\"0.0\",\"pressure\":\"1026\","
    "\"vis\":\"25\",\"cloud\":\"0\",\"uvIndex\":\"1\"},{\"fxDate\":\"2026-02-"
    "07\",\"sunrise\":\"07:18\",\"sunset\":\"17:40\",\"moonrise\":\"23:32\","
    "\"moonset\":\"09:45\",\"moonPhase\":\"亏凸月\",\"moonPhaseIcon\":\"805\","
    "\"tempMax\":\"0\",\"tempMin\":\"-9\",\"iconDay\":\"101\",\"textDay\":"
    "\"多云\",\"iconNight\":\"151\",\"textNight\":\"多云\",\"wind360Day\":"
    "\"225\",\"windDirDay\":\"西南风\",\"windScaleDay\":\"1-3\","
    "\"windSpeedDay\":\"3\",\"wind360Night\":\"45\",\"windDirNight\":"
    "\"东北风\",\"windScaleNight\":\"1-3\",\"windSpeedNight\":\"3\","
    "\"humidity\":\"22\",\"precip\":\"0.0\",\"pressure\":\"1029\",\"vis\":"
    "\"25\",\"cloud\":\"0\",\"uvIndex\":\"3\"}],\"refer\":{\"sources\":["
    "\"QWeather\"],\"license\":[\"QWeather Developers License\"]}}";

  json_error_t json_error;
  json_t *data = json_loads(json_str, 0, &json_error);

  if (!data) {
    fprintf(stderr, "Error parsing JSON: %s\n", json_error.text);
    return 1;
  }

  // Print the entire JSON data
  char *pretty_json = json_dumps(data, JSON_INDENT(2));
  printf("Parsed JSON Data:\n%s\n", pretty_json);

  return 0;
}
