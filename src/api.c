//
// Created by masonlee on 2026/2/4.
//

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

// 回调函数：处理curl返回的数据
static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
  printf("Write callback called, size: %d\n", size);
  const size_t realsize = size * nmemb;
  MemoryStruct *mem = (MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (ptr == NULL) {
    // 内存分配失败
    printf("Not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0; // Null-terminate the string

  return realsize;
}

void get_weather_by_day(Weather *weather, const char *token) {
  printf("Start curl request.\n");
  CURL *curl = curl_easy_init();
  if (curl == NULL) {
    perror("CURL init error.");
    exit(1);
  }

  MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  struct curl_slist *headers = NULL;
  char auth_header[256];
  snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", token);
  headers = curl_slist_append(headers, auth_header);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(
      curl, CURLOPT_URL,
      "https://jw564k2gn9.re.qweatherapi.com/v7/weather/3d?location=101010100");
  curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
  } else {
    printf("CURL request succeeded.\n");
    printf("Response: %s\n", chunk.memory);
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  free(chunk.memory);
}
