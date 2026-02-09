//
// Created by masonlee on 2026/2/5.
//

#ifndef WEATHER_UTILS_H
#define WEATHER_UTILS_H

int w_config_init(void);

char *base64_decode(const char *input, int *length);
char *base64_encode(const char *input, int length);

char *str_duplicate(const char *src);

void w_log(const char *msg, ...);
void w_log_error(const char *msg, ...);

int str_to_int(const char *str);

#endif // WEATHER_UTILS_H
