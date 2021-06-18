#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libchttpd.h"
#include "api.h"
#include <pthread.h>

int int_param = 100;
char* str_param = NULL;
int bool_param = 1;

void* watcher(void* data) {
	while(1) {
		printf("%d, %s, %d\n", int_param, str_param, bool_param);
		sleep(1);
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

	pthread_t tid;
	pthread_create(&tid, NULL, watcher, NULL);

	while(1);
	printf("Quit...\n");
	return 0;
}

