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

int set_vars(char * json_str) {
	int ret = 0;
	printf("%s() called\n", __func__);
	cJSON *json_obj = cJSON_Parse(json_str);
	cJSON *item = NULL;
	cJSON_ArrayForEach(item, json_obj)
	{
		cJSON* p_fn = cJSON_GetObjectItem(item, "src_filename");
		if(p_fn && cJSON_IsString(p_fn)) {
			//printf("%s\n", p_fn->valuestring);
		} else {
			goto end;
		}

		cJSON* p_name = cJSON_GetObjectItem(item, "var_name");
		if(p_name && cJSON_IsString(p_name)) {
			//printf("%s\n", p_name->valuestring);
		} else {
			goto end;
		}

		// search by src_filename and var_name
		var_registation_entry *p = NULL;
		for(int i = 0; i < MAX_REGISTRY_ENTRY; i++) {
			if(!strcmp(VAR_REGISTRY[i].src_filename, p_fn->valuestring) &&
				!strcmp(VAR_REGISTRY[i].var_name, p_name->valuestring)) {
				p = VAR_REGISTRY + i;
				break;
			}
		}

		if(!p) goto end;
		
		cJSON* p_type = cJSON_GetObjectItem(item, "var_type");
		if(p_type && cJSON_IsString(p_type)) {
			//printf("%s\n", p_type->valuestring);
		} else {
			goto end;
		}

		cJSON* p_var = cJSON_GetObjectItem(item, "var");
		if(p_var) {
			if(!strcmp(p_type->valuestring, "int"))
			{
				if(cJSON_IsNumber(p_var)) {
					*((int*)p->var) = p_var->valueint;
					//printf("%d\n", p_var->valueint);
				} else {
					printf("'type' and 'value' do not match\n");
					goto end;
				}
			}
			else if (!strcmp(p_type->valuestring, "string"))
			{
				if(cJSON_IsString(p_var)) {
					free(*(char**)(p->var));
					*(char**)(p->var) = (char*)malloc(strlen(p_var->valuestring) + 1);
					memcpy(*(char**)(p->var), p_var->valuestring, strlen(p_var->valuestring) + 1);
					//printf("%s\n", p_var->valuestring);
				} else {
					printf("'type' and 'value' do not match\n");
					goto end;
				}
			}
			else if (!strcmp(p_type->valuestring, "float"))
			{
				if(cJSON_IsNumber(p_var)) {
					*((float*)p->var) = p_var->valuedouble;
					//printf("%lf\n", p_var->valuedouble);
				}
				else {
					printf("'type' and 'value' do not match\n");
					goto end;
				}
			}
			else if (!strcmp(p_type->valuestring, "bool"))
			{
				if(cJSON_IsBool(p_var)) {
					*((int*)p->var) = p_var->valueint;
					//printf("%d\n", p_var->valueint);
				}
				else {
					printf("'type' and 'value' do not match\n");
					goto end;
				}
			}
			
		} else {
			goto end;
		}
	}

	ret = 1;
end:
	cJSON_Delete(json_obj);
	return ret;
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

			if(!strcmp(VAR_REGISTRY[i].var_type, "int"))
			{
				cJSON* var = cJSON_CreateNumber(*((int*)VAR_REGISTRY[i].var));
				cJSON_AddItemToObject(var_item, "var", var);
			}
			else if(!strcmp(VAR_REGISTRY[i].var_type, "float"))
			{
				cJSON* var = cJSON_CreateNumber(*((float*)VAR_REGISTRY[i].var));
				cJSON_AddItemToObject(var_item, "var", var);
			}
			else if(!strcmp(VAR_REGISTRY[i].var_type, "string"))
			{
				cJSON* var = cJSON_CreateString(*(char**)(VAR_REGISTRY[i].var));
				cJSON_AddItemToObject(var_item, "var", var);
			}
			else if(!strcmp(VAR_REGISTRY[i].var_type, "bool"))
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