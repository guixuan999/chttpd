#include <stdio.h>
#include <string.h>
#include "../libchttpd.h"

int main(int argc, char* argv[]) {
	GoString addr = {":8080", strlen(":8080")};
	start_httpd(addr);
	
	while(1);
	printf("Quit...\n");
	return 0;
}

