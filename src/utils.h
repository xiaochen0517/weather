//
// Created by masonlee on 2026/2/5.
//

#ifndef WEATHER_UTILS_H
#define WEATHER_UTILS_H

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <string.h>

char *base64_decode(const char *input, int *length);
char *base64_encode(const char *input, int length);

#endif // WEATHER_UTILS_H
