//
// Created by masonlee on 2026/2/5.
//

#include "utils.h"

char *base64_decode(const char *input, int *length) {
  char *buffer = malloc(strlen(input));

  BIO *bio = BIO_new_mem_buf((void *)input, (int)strlen(input));
  BIO *b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  *length = BIO_read(bio, buffer, (int)strlen(input));

  BIO_free_all(bio);
  return buffer;
}

static void base64_to_base64url(char *str) {
  for (char *p = str; *p; p++) {
    if (*p == '+') {
      *p = '-';
    } else if (*p == '/') {
      *p = '_';
    }
  }

  // Remove padding '='
  char *end = str + strlen(str) - 1;
  while (end >= str && *end == '=') {
    *end = '\0';
    end--;
  }
}

char *base64_encode(const char *input, const int length) {
  BUF_MEM *buffer_ptr;

  BIO *b64 = BIO_new(BIO_f_base64());
  BIO *bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(bio, input, length);
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &buffer_ptr);

  char *buf = malloc(buffer_ptr->length + 1);
  memcpy(buf, buffer_ptr->data, buffer_ptr->length);
  buf[buffer_ptr->length] = '\0';
  base64_to_base64url(buf);

  BIO_free_all(bio);
  return buf;
}
