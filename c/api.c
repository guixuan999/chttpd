#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON/cJSON.h>
#include <api.h>

#define MAX_REGISTRY_ENTRY 100

typedef struct {
    const char* src_filename;
    const char* var_name;
    const char* var_type;
    void* var;
} var_registation_entry;

static var_registation_entry VAR_REGISTRY[MAX_REGISTRY_ENTRY];

static void registry_init() {
	static int initilized = 0;
	if(!initilized) {
		memset(VAR_REGISTRY, 0, MAX_REGISTRY_ENTRY * sizeof(var_registation_entry));
		initilized = 1;
	}
}

int set() {
	printf("%s() called\n", __func__);
	return 101;
}

/* 
	var_type: valid in {"int", "float", "string", "bool"}
 */
void do_register_var(const char* src_filename, const char* var_name, const char* var_type, void* var) {
	printf("%s() called\n", __func__);
	registry_init();

	for(int i = 0; i < MAX_REGISTRY_ENTRY; i++) {
		if(VAR_REGISTRY[i].var == NULL) {
			char* p_fn = (char*)malloc(strlen(src_filename) + 1);
			memcpy(p_fn, src_filename, strlen(src_filename) + 1);
			VAR_REGISTRY[i].src_filename = p_fn;

			char* p_name = (char*)malloc(strlen(var_name) + 1);
			memcpy(p_name, var_name, strlen(var_name) + 1);
			VAR_REGISTRY[i].var_name = p_name;

			char* p_type = (char*)malloc(strlen(var_type) + 1);
			memcpy(p_type, var_type, strlen(var_type) + 1);
			VAR_REGISTRY[i].var_type = p_type;

			VAR_REGISTRY[i].var = var;

			break;
		}
	}
}

char* get_vars() {
	printf("%s() called\n", __func__);
	cJSON* outermost = cJSON_CreateArray();
	for(int i = 0; i < MAX_REGISTRY_ENTRY; i++) {
		if(VAR_REGISTRY[i].var != NULL) {
			cJSON* var_item = cJSON_CreateObject();
			cJSON_AddItemToArray(outermost, var_item);

			cJSON* src_filename = cJSON_CreateString(VAR_REGISTRY[i].src_filename);
			cJSON_AddItemToObject(var_item, "src_filename", src_filename);

			cJSON* var_name = cJSON_CreateString(VAR_REGISTRY[i].var_name);
			cJSON_AddItemToObject(var_item, "var_name", var_name);
			
			cJSON* var_type = cJSON_CreateString(VAR_REGISTRY[i].var_type);
			cJSON_AddItemToObject(var_item, "var_type", var_type);

			if((strlen(VAR_REGISTRY[i].var_type) == strlen("int")) && 
				!memcmp(VAR_REGISTRY[i].var_type, "int", strlen("int")))
			{
				cJSON* var = cJSON_CreateNumber(*((int*)VAR_REGISTRY[i].var));
				cJSON_AddItemToObject(var_item, "var", var);
			}
			else if((strlen(VAR_REGISTRY[i].var_type) == strlen("float")) && 
				!memcmp(VAR_REGISTRY[i].var_type, "float", strlen("float")))
			{
				cJSON* var = cJSON_CreateNumber(*((float*)VAR_REGISTRY[i].var));
				cJSON_AddItemToObject(var_item, "var", var);
			}
			else if((strlen(VAR_REGISTRY[i].var_type) == strlen("string")) && 
				!memcmp(VAR_REGISTRY[i].var_type, "string", strlen("string")))
			{
				cJSON* var = cJSON_CreateString(VAR_REGISTRY[i].var);
				cJSON_AddItemToObject(var_item, "var", var);
			}
			else if((strlen(VAR_REGISTRY[i].var_type) == strlen("bool")) && 
				!memcmp(VAR_REGISTRY[i].var_type, "bool", strlen("bool")))
			{
				cJSON* var = cJSON_CreateBool(*((int*)VAR_REGISTRY[i].var));
				cJSON_AddItemToObject(var_item, "var", var);
			}
		}
	}

	char* json_string = cJSON_Print(outermost);
	cJSON_Delete(outermost);

	return json_string;
}