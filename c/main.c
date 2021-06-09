#include <stdio.h>
#include "../chttpd.h"

int main(int argc, char* argv[]) {
	GoString addr = {":8080", sizeof(":8080")};
	start_httpd(addr);
	
	while(1);
	printf("Quit...\n");
	return 0;
}

