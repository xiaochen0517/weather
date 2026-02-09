//
// Created by masonlee on 2026/2/4.
//

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"
#include "utils.h"

// 回调函数：处理curl返回的数据
static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
  w_log("Write callback called, size: %d\n", size);
  const size_t realsize = size * nmemb;
  MemoryStruct *mem = (MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (ptr == NULL) {
    // 内存分配失败
    w_log("Not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0; // Null-terminate the string

  return realsize;
}

static char *http_api_request(const char *url, const char *token, CURL *curl) {
  w_log("Start curl request.\n");
  if (curl == NULL) {
    curl = curl_easy_init();
  }

  MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  struct curl_slist *headers = NULL;
  char auth_header[256];
  snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);
  headers = curl_slist_append(headers, auth_header);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
  } else {
    w_log("CURL request succeeded.\n");
    w_log("Response: %s\n", chunk.memory);
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  return chunk.memory;
}

char *get_geoid_by_location(const char *location, const char *token) {
  w_log("Start get geoid request.\n");
  CURL *curl = curl_easy_init();
  // 使用 curl_easy_escape 进行编码
  char *encoded = curl_easy_escape(curl, location, (int)strlen(location));
  char url[512];
  snprintf(
      url, sizeof(url),
      "https://jw564k2gn9.re.qweatherapi.com/geo/v2/city/lookup?location=%s",
      encoded);
  return http_api_request(url, token, curl);
}

char *get_weather_json_by_days(const int day_size, const char *location_id,
                               const char *token) {
  w_log("Start get weather request.\n");
  char url[512];
  snprintf(url, sizeof(url),
           "https://jw564k2gn9.re.qweatherapi.com/v7/weather/%dd?location=%s",
           day_size, location_id);
  return http_api_request(url, token, NULL);
}
