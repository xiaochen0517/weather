#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "jwt.h"
#include "utils.h"

int main(void) {
  char *kid_str = getenv("API_KID");
  if (!kid_str) {
    fprintf(stderr, "Please set API_KID environment variable.\n");
    return 1;
  }
  char *sub_str = getenv("API_SUB");
  if (!sub_str) {
    fprintf(stderr, "Please set API_SUB environment variable.\n");
    return 1;
  }

  const int64_t expiration_ms = 60 * 60; // 1 hour
  char *jwt_token = get_jwt_token(kid_str, sub_str, expiration_ms,
                                  "/home/masonlee/.ssh/ed25519-private.pem");
  printf("Token: %s\n", jwt_token);

  Weather weather = {0};
  get_weather_by_day(&weather, jwt_token);

  free(jwt_token);
  return 0;
}
