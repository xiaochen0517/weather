//
// Created by masonlee on 2026/2/5.
//

#ifndef WEATHER_JWT_H
#define WEATHER_JWT_H

#include <stdint.h>

char *get_jwt_token(const char *kid, const char *sub, int64_t expiration_ms,
                    const char *signature_key_path);

#endif // WEATHER_JWT_H
