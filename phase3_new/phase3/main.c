#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {allone, object, array}cJSONkind;

typedef struct cJSON
{
    struct cJSON *left, *right, *down, *up;
    char type[200], content[200];
    cJSONkind kind;
    
}cJSON;

void emtying_cJSON(cJSON* fp)
{
    fp->up = NULL;
    fp->down = NULL;
    fp->left = NULL;
    fp->right = NULL;
    strcpy(fp->type, "");
    strcpy(fp->content, "");
}
cJSON* cJSON_CreateString(char* content_en)
{
    cJSON* fp = (cJSON*)malloc(sizeof(cJSON));
    emtying_cJSON(fp);
    strcpy(fp->content, content_en);
    fp->kind = allone;
    return fp;
}

cJSON* cJSON_CreateObject()
{
    cJSON* fp = (cJSON*)malloc(sizeof(cJSON));
    emtying_cJSON(fp);
    fp->kind = object;
    return fp;
}

cJSON* cJSON_CreateArray()
{
    cJSON* fp = (cJSON*)malloc(sizeof(cJSON));
    emtying_cJSON(fp);
    fp->kind = array;
    return fp;
}

void cJSON_AddItemToObject(cJSON* ent_object, char* type_en, cJSON* item_en)
{
    cJSON* fp;
    strcpy(item_en->type, type_en);
    if (ent_object->down == NULL) {
        ent_object->down = item_en;
        item_en->up = ent_object;
    }
    else if( (fp = ent_object -> down)->right == NULL ){
        fp->right = item_en;
        item_en->left = fp;
    }
    else{
        while (fp->right != NULL) {
            fp = fp->right;
        }
            fp->right = item_en;
            item_en->left = fp;
    }
}

void cJSON_AddItemToArray(cJSON* ent_array, cJSON* content_en)
   {
       cJSON* fp;
       if (ent_array->down == NULL) {
           ent_array->down = content_en;
           content_en->up = ent_array;
       }
       else if ( (fp = ent_array->down)->right == NULL ) {
           fp->right = content_en;
       }
       else{
           while (fp->right != NULL) {
               fp = fp->right;
           }
               fp->right = content_en;
               content_en->left = fp;
       }
   }
cJSON* cJSON_Parse_first_step(char* fp, int* n)
{
    cJSON* final;
    if (fp[(*n)] == '"') {
        (*n)++;
        int k = 0;
        char charac[100];
        while ( (charac[k++] = fp[(*n)++]) != '"');
        charac[k-1] = 0;
        if (fp[(*n)] == ':') {
            (*n)++;
            final = cJSON_Parse_first_step(fp, n);
            strcpy(final->type, charac);
        }
        else{
            final = cJSON_CreateString(charac);
        }
    }
    else if (fp[(*n)] == '[') {
        final = cJSON_CreateArray();
        cJSON* tmp;
        if (fp[++(*n)] != ']') {
            tmp = cJSON_Parse_first_step(fp, n);
            final->down = tmp;
        }
        while (fp[(*n)] != ']') {
            (*n)++;
            tmp->right = cJSON_Parse_first_step(fp, n);
            tmp = tmp->right;
        }
        (*n)++;
    }
    else if (fp[(*n)] == '{') {
        final = cJSON_CreateObject();
        cJSON* tmp;
        if (fp[++(*n)] != '}') {
            tmp = cJSON_Parse_first_step(fp, n);
            final->down = tmp;
        }
        while (fp[(*n)] != '}') {
            (*n)++;
            tmp->right = cJSON_Parse_first_step(fp, n);
            tmp = tmp->right;
        }
        (*n)++;
    }
    return final;
}

cJSON* cJSON_Parse(char* fp)
{
    int* n= calloc(1, sizeof(int));
    *n = 0;
    cJSON* final = cJSON_Parse_first_step(fp, n);
    free(n);
    return final;
}

void cJSON_PrintUnformatted_first_step(cJSON* fp)
{
    if (fp->kind == allone) {
        if (strcmp(fp->type, "")) {
            printf("\"%s\":", fp->type);
        }
        printf("\"%s\"", fp->content);
        if (fp->right != NULL) {
            printf(",");
            cJSON_PrintUnformatted_first_step(fp->right);
        }
    }
    else if (fp->kind == object) {
        if (strcmp(fp->type, "")) {
            printf("\"%s\":", fp->type);
        }
        printf("{");
        if (fp->down != NULL) {
            cJSON_PrintUnformatted_first_step(fp->down);
        }
        printf("}");
        if (fp->right != NULL) {
            printf(",");
            cJSON_PrintUnformatted_first_step(fp->right);
        }
    }
    else if (fp->kind == array) {
        if (strcmp(fp->type, "")) {
            printf("\"%s\":", fp->type);
        }
        printf("[");
        if (fp->down != NULL) {
            cJSON_PrintUnformatted_first_step(fp->down);
        }
        printf("]");
        if (fp->right != NULL) {
            printf(",");
            cJSON_PrintUnformatted_first_step(fp->right);
        }
    }
}

char* cJSON_PrintUnformatted(cJSON*fp)
{
    char* final = calloc(1, 2500);
    if (fp->kind == allone) {
        if (strcmp(fp->type, "")) {
            strcat(final, "\"");
            strcat(final, fp->type);
            strcat(final, "\":");
        }
        strcat(final, "\"");
        strcat(final, fp->content);
        strcat(final, "\"");
        if (fp->right != NULL) {
            strcat(final, ",");
            strcat(final, cJSON_PrintUnformatted(fp->right));
        }
    }
    else if (fp->kind == object) {
        if (strcmp(fp->type, "")) {
            strcat(final, "\"");
            strcat(final, fp->type);
            strcat(final, "\":");
        }
        strcat(final, "{");
        if (fp->down != NULL) {
            strcat(final, cJSON_PrintUnformatted(fp->down));
        }
        strcat(final, "}");
        if (fp->right != NULL) {
            strcat(final, ",");
            strcat(final, cJSON_PrintUnformatted(fp->right));
        }
    }
    else if (fp->kind == array) {
        if (strcmp(fp->type, "")) {
            strcat(final, "\"");
            strcat(final, fp->type);
            strcat(final, "\":");
        }
        strcat(final, "[");
        if (fp->down != NULL) {
            strcat(final, cJSON_PrintUnformatted(fp->down));
        }
        strcat(final, "]");
        if (fp->right != NULL) {
            strcat(final, ",");
            strcat(final, cJSON_PrintUnformatted(fp->right));
        }
    }
    return final;
}

cJSON* cJSON_GetObjectItem(cJSON* fp, char* type_en)
{
    cJSON* tmp = fp->down;
    while (tmp != NULL) {
        if (!strcmp(type_en, tmp->type)) {
            return tmp;
        }
            tmp = tmp->right;
    }
    return NULL;
}

int cJSON_GetArraySize(cJSON* array_en)
{
    cJSON* tmp = array_en->down;
    int n = 0;
    while (tmp != NULL) {
        tmp = tmp->right;
        n++;
    }
    return n;
}

cJSON* cJSON_GetArrayItem(cJSON* array_en, int element)
{
    if (element >= cJSON_GetArraySize(array_en)) {
        return NULL;
    }
    cJSON* tmp = array_en->down;
    for (int i = 0; i < element; i++) {
        tmp = tmp->right;
    }
    return tmp;
}

int main() {

    return 0;
}
