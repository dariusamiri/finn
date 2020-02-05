typedef enum {allone, object, array}cJSONkind;

typedef struct cJSON
{
    struct cJSON *left, *right, *down, *up;
    char type[200], content[200];
    cJSONkind kind;
    
}cJSON;

void emtying_cJSON(cJSON* fp);

cJSON* cJSON_CreateString(char* content_en);

cJSON* cJSON_CreateObject(void);

cJSON* cJSON_CreateArray(void);

void cJSON_AddItemToObject(cJSON* ent_object, char* type_en, cJSON* item_en);

void cJSON_AddItemToArray(cJSON* ent_array, cJSON* content_en);

cJSON* cJSON_Parse_first_step(char* fp, int* n);

cJSON* cJSON_Parse(char* fp);

void cJSON_PrintUnformatted_first_step(cJSON* fp);

char* cJSON_PrintUnformatted(cJSON*fp);

cJSON* cJSON_GetObjectItem(cJSON* fp, char* type_en);

int cJSON_GetArraySize(cJSON* array_en);

cJSON* cJSON_GetArrayItem(cJSON* array_en, int element);











