#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

//typedef enum {allone, object, array}cJSONkind;

//typedef struct cJSON
//{
//    struct cJSON *left, *right, *down, *up;
//    char type[200], valuestring[200];
//    cJSONkind kind;
//
//}cJSON;


static int cJSON_strcasecmp(const char *s1,const char *s2)
{
    if (!s1){
        if (s1==s2) {
            return 0;
        }
        else
            return 1;
    }
    if (!s2)
        return 1;
    for(; tolower(*s1) == tolower(*s2); ++s1, ++s2){
        if(*s1 == 0)
            return 0;
    }
    return tolower(*(const unsigned char *)s1) - tolower(*(const unsigned char *)s2);
}





void emtying_cJSON(cJSON* fp)
{
    fp->up = NULL;
    fp->down = NULL;
    fp->left = NULL;
    fp->right = NULL;
    strcpy(fp->type, "");
    strcpy(fp->valuestring, "");
}
cJSON* cJSON_CreateString(char* content_en)
{
    cJSON* fp = (cJSON*)malloc(sizeof(cJSON));
    emtying_cJSON(fp);
    strcpy(fp->valuestring, content_en);
    fp->kind = allone;
    return fp;
}

cJSON* cJSON_CreateNumber(double num)
{
    cJSON* fp = (cJSON*)malloc(sizeof(cJSON));
    emtying_cJSON(fp);
    fp->valueint = num;
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
        printf("\"%s\"", fp->valuestring);
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
        strcat(final, fp->valuestring);
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

static void (*cJSON_free)(void *ptr) = free;

void cJSON_Delete(cJSON *c)
{
    cJSON *next;
    while (c)
    {
        next=c->right;
        if (!(c->valueint&256) && c->down) cJSON_Delete(c->down);
        if (!(c->valueint&256) && c->str) cJSON_free(c->str);
        if (!(c->valueint&512) && c->str) cJSON_free(c->str);
        cJSON_free(c);
        c=next;
    }
}


//void cJSON_Delete(cJSON* fp)
//{
//    if (fp->down != NULL) {
//        cJSON_Delete(fp->down);
//    }
//    if (fp->right != NULL) {
//        cJSON_Delete(fp->right);
//    }
//    free(fp);
//}

cJSON* cJSON_DeleteItemFromArray_first_step(cJSON* array_en, int element)
{
    cJSON* tmp = array_en->down;
    while (tmp && element>0) {
        (void)(tmp = tmp->right);
        element--;
    }
    if (!tmp) {
        return 0;
    }
    if (tmp->left) {
       (tmp->left)->right = tmp->right;
    }
    if (tmp->right) {
        (tmp->right)->left = tmp->left;
    }
    if (tmp == array_en->down) {
        array_en->down = tmp->right;
    }
    tmp->left = 0;
    tmp->right = 0;
    return tmp;
}

void cJSON_DeleteItemFromArray(cJSON* array_en, int element)
{
    cJSON_Delete(cJSON_DeleteItemFromArray_first_step(array_en, element));
}

cJSON* cJSON_DeleteItemFromObject_first_step(cJSON* object_en, char* str)
{
    int n = 0;
    cJSON* fp = object_en->down;
    while (fp && cJSON_strcasecmp(fp->str, str)) {
        (n++);
        fp = fp->right;
    }
    if (fp) {
        return cJSON_DeleteItemFromArray_first_step(object_en, n);
    }
    return 0;
}

void cJSON_DeleteItemFromObject(cJSON* object_en, char* str)
{
    cJSON_Delete(cJSON_DeleteItemFromObject_first_step(object_en, str));
}
