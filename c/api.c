#include <stdio.h>
#include <stdlib.h>
#include <cJSON/cJSON.h>
int set() {
	printf("int set() called\n");

	char *char_json = "{\"habit\":\"lol\"}";
	//从缓冲区中解析出JSON结构
	cJSON *json = cJSON_Parse(char_json);
 	//将传入的JSON结构转化为字符串 并打印
	char *buf = NULL;
	printf("data:%s\n",buf = cJSON_Print(json));
	
	free(buf);
	cJSON_Delete(json);

	return 1;
}

