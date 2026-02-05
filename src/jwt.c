//
// Created by masonlee on 2026/2/5.
//

#include <jansson.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <stdio.h>

#include "jwt.h"
#include "utils.h"

#include <openssl/err.h>

static void remove_last_equals(char *str) {
  const size_t len = strlen(str);
  for (size_t i = len; i > 0; i--) {
    if (str[i - 1] == '=') {
      str[i - 1] = '\0';
    } else {
      break;
    }
  }
}

static char *get_header_payload(const char *kid, const char *sub,
                                int64_t expiration_ms) {
  // 创建 Header
  json_t *header = json_object();
  json_object_set_new(header, "alg", json_string("EdDSA"));
  json_object_set_new(header, "kid", json_string(kid));
  // 转换为 json 字符串
  char *header_json = json_dumps(header, JSON_COMPACT);
  // 使用 base64 进行编码
  char *header_b64 = base64_encode(header_json, (int)strlen(header_json));

  // 创建 Payload
  json_t *payload = json_object();
  time_t now = time(NULL);

  json_object_set_new(payload, "sub", json_string(sub));
  json_object_set_new(payload, "iat", json_integer(now - 30));
  json_object_set_new(payload, "exp", json_integer(now + expiration_ms));
  // 转换为 json 字符串
  char *payload_json = json_dumps(payload, JSON_COMPACT);
  char *payload_b64 = base64_encode(payload_json, (int)strlen(payload_json));

  // 创建签名内容
  unsigned long signing_input_length =
      sizeof(char) * (strlen(header_b64) + strlen(payload_b64) + 2);
  char *header_payload = malloc(signing_input_length);
  sprintf(header_payload, "%s.%s", header_b64, payload_b64);
  header_payload[signing_input_length - 1] = '\0';

  // 释放资源
  json_decref(header);
  json_decref(payload);
  free(header_json);
  free(header_b64);
  free(payload_json);
  free(payload_b64);

  return header_payload;
}

static EVP_PKEY *read_ed25519_key(const char *key_path) {
  FILE *key_file = fopen(key_path, "r");
  if (!key_file) {
    fprintf(stderr, "Can't open key file: %s\n", key_path);
    return NULL;
  }

  EVP_PKEY *pkey = PEM_read_PrivateKey(key_file, NULL, NULL, NULL);
  fclose(key_file);
  if (!pkey) {
    fprintf(stderr, "Error reading private key from file: %s\n", key_path);
    return NULL;
  }

  return pkey;
}

static EVP_PKEY *read_ed25519_public_key(const char *key_path) {
  FILE *key_file = fopen(key_path, "r");
  if (!key_file) {
    fprintf(stderr, "Can't open key file: %s\n", key_path);
    exit(1);
  }

  EVP_PKEY *pkey = PEM_read_PUBKEY(key_file, NULL, NULL, NULL);
  fclose(key_file);
  if (!pkey) {
    fprintf(stderr, "Error reading public key from file: %s\n", key_path);
    exit(1);
  }

  return pkey;
}

static char *signature(const char *str, EVP_PKEY *private_key) {
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  unsigned char sig_buf[256];
  size_t sig_len = sizeof(sig_buf);

  if (EVP_DigestSignInit(mdctx, NULL, NULL, NULL, private_key) <= 0) {
    fprintf(stderr, "EVP_DigestSignInit failed.\n");
    ERR_print_errors_fp(stderr);
    EVP_MD_CTX_free(mdctx);
    exit(1);
  }

  if (EVP_DigestSign(mdctx, sig_buf, &sig_len, (unsigned char *)str,
                     strlen(str)) <= 0) {
    fprintf(stderr, "EVP_DigestSign failed.\n");
    ERR_print_errors_fp(stderr);
    EVP_MD_CTX_free(mdctx);
    exit(1);
  }

  EVP_MD_CTX_free(mdctx);

  printf("Signature length: %zu\n", sig_len);
  char *sig_b64 = base64_encode((char *)sig_buf, (int)sig_len);
  return sig_b64;
}

char *get_jwt_token(const char *kid, const char *sub, int64_t expiration_ms,
                    const char *signature_key_path) {
  char *header_payload = get_header_payload(kid, sub, expiration_ms);
  printf("Header.Payload: %s\n", header_payload);

  EVP_PKEY *private_key = read_ed25519_key(signature_key_path);
  char *sig_str = signature(header_payload, private_key);
  printf("Signature: %s\n", sig_str);

  const size_t jwt_length =
      sizeof(char) * (strlen(header_payload) + strlen(sig_str) + 2);
  char *jwt_token = malloc(jwt_length);
  sprintf(jwt_token, "%s.%s", header_payload, sig_str);
  jwt_token[jwt_length - 1] = '\0';

  free(header_payload);
  return jwt_token;
}
