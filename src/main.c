#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Hello, World!\n");
    // 创建 JSON 对象
    cJSON *root = cJSON_CreateObject();

    // 添加字段
    cJSON_AddStringToObject(root, "name", "John");
    cJSON_AddNumberToObject(root, "age", 30);
    cJSON_AddBoolToObject(root, "active", 1);

    // 转换为字符串
    char *json_str = cJSON_Print(root);
    printf("生成的 JSON:\n%s\n", json_str);

    // 解析 JSON
    cJSON *name = cJSON_GetObjectItem(root, "name");
    if (name && name->type == cJSON_String) {
        printf("name 字段: %s\n", name->valuestring);
    }

    // 释放内存
    free(json_str);
    cJSON_Delete(root);
    return 0;
}
