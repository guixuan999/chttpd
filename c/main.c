#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../libchttpd.h"
#include "api.h"

int main(int argc, char* argv[]) {
	GoString addr = {":8080", strlen(":8080")};
	start_httpd(addr);
	
	int int_param = 100;
	do_register_var(__FILE__, "int_param", "int", &int_param);
	char * str_param = "guixuan";
	do_register_var(__FILE__, "str_param", "string", str_param);

	int bool_param = 1;
	do_register_var(__FILE__, "bool_param", "bool", &bool_param);

	while(1);
	printf("Quit...\n");
	return 0;
}

