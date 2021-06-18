#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libchttpd.h"
#include "api.h"
#include <pthread.h>

int int_param = 100;
char* str_param = NULL;
int bool_param = 1;
float float_param = 123.39;

void* watcher(void* data) {
	while(1) {
		printf("%8d, %20s, %1d, %8f\n", int_param, str_param, bool_param, float_param);
		sleep(2);
	}
}

int main(int argc, char* argv[]) {
	GoString addr = {":8080", strlen(":8080")};
	start_httpd(addr);
	
	register_var("int_param", "int", &int_param);
	
	str_param = malloc(strlen("wang hongyan") + 1);
	memcpy(str_param, "wang hongyan", strlen("wang hongyan") + 1);
	register_var("str_param", "string", &str_param);

	register_var("bool_param", "bool", &bool_param);

	register_var("float_param", "float", &float_param);

	pthread_t tid;
	pthread_create(&tid, NULL, watcher, NULL);

	while(1);
	printf("Quit...\n");
	return 0;
}

