#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <time.h>
#include "cJSON.h"
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
char address_channel_list[200] = "/Users/Dariush/Desktop/SUT/مبانی برنامه نویسی/Project/phase2/Resources/channels/";
char address_user_list[200] = "/Users/Dariush/Desktop/SUT/مبانی برنامه نویسی/Project/phase2/Resources/users/";
cJSON* online_user;
cJSON* online_token;
cJSON* token_get_user;
cJSON* token_get_channel;
cJSON* token_get_num_read_message;
int num_online_user = 0;
char AuthToken[32];
int server_socket, client_socket;

void make_socket()
{
    struct sockaddr_in server;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    if ((bind(server_socket, (SA*)&server, sizeof(server))) != 0) {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");
    
    // Now server is ready to listen and verify
    
}
void acceptance(){
    struct sockaddr_in client;
    if ((listen(server_socket, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // Accept the data packet from client and verify
    socklen_t len = sizeof(client);
    client_socket = accept(server_socket, (SA*)&client, &len);
    if (client_socket < 0) {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("Server acccepted the client..\n");
    
}
void Register(char* buffer);

void Login(char* buffer);

void Create_Channel(char* buffer);

void Join_Channel(char* buffer);

void Logout(char* buffer);

void Send_Message(char* buffer);

void Refresh(char* buffer);

void Channel_Members(char* buffer);

void Leave_channel(char* buffer);

enum alphabet {a,b,c,d,e,f,g};
void product_token()
{
    bzero(AuthToken, sizeof(AuthToken));
    srand((int)time(0));
    char element = 'z';
    for (int i = 0; i < 31; i++) {
        element = rand()%12+76;
        AuthToken[i] = element;
    }
}

int main() {
    online_user = cJSON_CreateArray();
    online_token = cJSON_CreateArray();
    token_get_user = cJSON_CreateObject();
    token_get_channel = cJSON_CreateObject();
    token_get_num_read_message = cJSON_CreateObject();
    char buffer[1000];
    char check[100];
    make_socket();
    while (1) {
        acceptance();
        bzero(buffer, sizeof(buffer));
        bzero(check, sizeof(check));
        recv(client_socket, buffer, sizeof(buffer), 0);
        sscanf(buffer, "%s", check);
        if (strcmp(check, "register") == 0) {
            Register(buffer+9);
            continue;
        }
        else if(strcmp(check, "login") == 0){
            Login(buffer+6);
            continue;
        }
        else if(strcmp(check, "create") == 0){
            Create_Channel(buffer+15);
            continue;
        }
        else if(strcmp(check, "join") == 0){
            Join_Channel(buffer+13);
            continue;
        }
        else if(strcmp(check, "logout") == 0){
            Logout(buffer+7);
            continue;
        }
        else if(strcmp(check, "send") == 0){
            Send_Message(buffer+5);
            continue;
        }
        else if(strcmp(check, "refresh") == 0){
            Refresh(buffer+8);
            continue;
        }
        else if(strcmp(check, "channel") == 0){
            Channel_Members(buffer+16);
            continue;
        }
        else if(strcmp(check, "leave") == 0){
            Leave_channel(buffer+6);
            continue;
        }
        shutdown(client_socket, SHUT_RDWR);
    }
    
    return 0;
}
void Register(char* buffer)
{
    cJSON* message_send = cJSON_CreateObject();
    char* message_send_str;
    char username[100];
    char password[100];
    sscanf(buffer, "%s %s", username, password);
    username[strlen(username)-1] = username[strlen(username)];
    char address[150];
    strcpy(address, address_user_list);
    strcat(address, username);
    strcat(address, ".json");
    FILE* fp1 = fopen(address, "r");
    if (fp1 == NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString(""));
        message_send_str = cJSON_PrintUnformatted(message_send);
        cJSON* store_user_pass = cJSON_CreateObject();
        char* store_user_pass_str;
        cJSON_AddItemToObject(store_user_pass, "username", cJSON_CreateString(username));
        cJSON_AddItemToObject(store_user_pass, "password", cJSON_CreateString(password));
        store_user_pass_str = cJSON_PrintUnformatted(store_user_pass);
        fclose(fp1);
        char address[150];
        strcpy(address, address_user_list);
        strcat(address, username);
        strcat(address, ".json");
        fp1 = fopen(address, "w");
        fprintf(fp1, "%s", store_user_pass_str);
        fclose(fp1);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
    }
    else{
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("this username is not available"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
    }
}

void Login(char* buffer)
{
    char content[200];
    char username[100];
    char password[100];
    sscanf(buffer, "%s %s", username, password);
    char* message_send_str;
    cJSON* message_send = cJSON_CreateObject();
    username[strlen(username)-1] = username[strlen(username)];
    char address[150];
    strcpy(address, address_user_list);
    strcat(address, username);
    strcat(address, ".json");
    FILE* fp1;
    fp1 = fopen(address, "r");
    if (fp1 == NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("it doesn't have existed"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        fclose(fp1);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return ;
    }
    fscanf(fp1, "%s", content);
    fclose(fp1);
    cJSON* json_content = cJSON_Parse(content);
    cJSON* json_password = cJSON_GetObjectItem(json_content, "password");
    char* check_log_user_arr_str;
    char check_log_user_str[100] = "\"";
    strcat(check_log_user_str, username);
    strcat(check_log_user_str, "\"");
    cJSON* check_log_user_arr = cJSON_CreateObject();
    for (int i = 0; i < num_online_user; i++) {
        check_log_user_arr = cJSON_GetArrayItem(online_user, i);
        check_log_user_arr_str = cJSON_PrintUnformatted(check_log_user_arr);
        if (strcmp(check_log_user_arr_str, check_log_user_str) == 0) {
            cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("it was loged in"));
            message_send_str = cJSON_PrintUnformatted(message_send);
            char tom_ellis[200];
            strcpy(tom_ellis, message_send_str);
            strcat(tom_ellis, "\0");
            send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
            message_send_str = "";
            bzero(tom_ellis, sizeof(tom_ellis));
            return;
        }
    }
    if (strcmp(json_password->valuestring, password) != 0) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("wrong password"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    product_token();
    cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("AuthToken"));
    cJSON_AddItemToObject(message_send, "content", cJSON_CreateString(AuthToken));
    message_send_str = cJSON_PrintUnformatted(message_send);
    char tom_ellis[200];
    strcpy(tom_ellis, message_send_str);
    strcat(tom_ellis, "\0");
    send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
    message_send_str = "";
    bzero(tom_ellis, sizeof(tom_ellis));
    cJSON_AddItemToArray(online_user, cJSON_CreateString(username));
    cJSON_AddItemToArray(online_token, cJSON_CreateString(AuthToken));
    cJSON_AddItemToObject(token_get_user, AuthToken, cJSON_CreateString(username));
    num_online_user++;
}

void Create_Channel(char* buffer)
{
    char channelname[100];
    char authtoken[100];
    sscanf(buffer, "%s %s", channelname, authtoken);
    channelname[strlen(channelname)-1] = channelname[strlen(channelname)];
    char address[150];
    strcpy(address, address_channel_list);
    strcat(address, channelname);
    strcat(address, ".json");
    char* message_send_str;
    cJSON* message_send = cJSON_CreateObject();
    char* check_AuthToken_arr_str;
    char check_AuthToken_str[100] = "\"";
    strcat(check_AuthToken_str, authtoken);
    strcat(check_AuthToken_str, "\"");
    cJSON* check_AuthToken_arr = cJSON_CreateObject();
    int flag = 0;
    for (int i = 0; i < num_online_user; i++) {
        check_AuthToken_arr = cJSON_GetArrayItem(online_token, i);
        check_AuthToken_arr_str = cJSON_PrintUnformatted(check_AuthToken_arr);
        if (strcmp(check_AuthToken_arr_str, check_AuthToken_str) == 0) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("Wrong AuthToken"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    FILE* fp1;
    fp1 = fopen(address, "r");
    if (fp1 != NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("channel name is not available"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        fclose(fp1);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(message_send, "content", cJSON_CreateString(""));
    message_send_str = cJSON_PrintUnformatted(message_send);
    cJSON_AddItemToObject(token_get_channel, authtoken, cJSON_CreateString(channelname));
    char tom_ellis[200];
    strcpy(tom_ellis, message_send_str);
    strcat(tom_ellis, "\0");
    send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
    message_send_str = "";
    bzero(tom_ellis, sizeof(tom_ellis));
    cJSON *recog_user_by_auth = cJSON_GetObjectItem(token_get_user, authtoken);
    char* recog_user_by_auth_str = cJSON_Print(recog_user_by_auth);
    char recog_user_by_auth_char[100];
    strcpy(recog_user_by_auth_char, recog_user_by_auth_str);
    for (int i = 0; i < strlen(recog_user_by_auth_char); i++) {
        recog_user_by_auth_char[i] = recog_user_by_auth_char[i+1];
    }
    recog_user_by_auth_char[strlen(recog_user_by_auth_char)-1] = recog_user_by_auth_char[strlen(recog_user_by_auth_char)];
    cJSON* content_channel = cJSON_CreateObject();
    char* content_channel_str;
    cJSON_AddItemToObject(content_channel, "sender", cJSON_CreateString("server"));
    char channel_message[200];
    sprintf(channel_message, "%s created %s.", recog_user_by_auth_char, channelname);
    cJSON_AddItemToObject(content_channel, "content", cJSON_CreateString(channel_message));
    content_channel_str = cJSON_PrintUnformatted(content_channel);
    fp1 = fopen(address, "w");
    fprintf(fp1, "%s\n", content_channel_str);
    cJSON_DeleteItemFromObject(content_channel, "content");
    bzero(channel_message, sizeof(channel_message));
    sprintf(channel_message, "%s joined.", recog_user_by_auth_char);
    cJSON_AddItemToObject(content_channel, "content", cJSON_CreateString(channel_message));
    content_channel_str = cJSON_PrintUnformatted(content_channel);
    fprintf(fp1,"%s\n", content_channel_str);
    fclose(fp1);
    cJSON_AddItemToObject(token_get_num_read_message, authtoken, cJSON_CreateNumber(0));
}

void Join_Channel(char* buffer)
{
    char channelname[100];
    char authtoken[100];
    sscanf(buffer, "%s %s", channelname, authtoken);
    channelname[strlen(channelname)-1] = channelname[strlen(channelname)];
    char* message_send_str;
    cJSON* message_send = cJSON_CreateObject();
    char* check_AuthToken_arr_str;
    char check_AuthToken_str[100] = "\"";
    strcat(check_AuthToken_str, authtoken);
    strcat(check_AuthToken_str, "\"");
    cJSON* check_AuthToken_arr = cJSON_CreateObject();
    int flag = 0;
    for (int i = 0; i < num_online_user; i++) {
        check_AuthToken_arr = cJSON_GetArrayItem(online_token, i);
        check_AuthToken_arr_str = cJSON_PrintUnformatted(check_AuthToken_arr);
        if (strcmp(check_AuthToken_arr_str, check_AuthToken_str) == 0) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("Wrong AuthToken"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    char address[150];
    strcpy(address, address_channel_list);
    strcat(address, channelname);
    strcat(address, ".json");
    FILE* fp1;
    fp1 = fopen(address, "r");
    if (fp1 == NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("channel not found."));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        fclose(fp1);
        return;
    }
    cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(message_send, "content", cJSON_CreateString(""));
    message_send_str = cJSON_PrintUnformatted(message_send);
    cJSON_AddItemToObject(token_get_channel, authtoken, cJSON_CreateString(channelname));
    char tom_ellis[200];
    strcpy(tom_ellis, message_send_str);
    strcat(tom_ellis, "\0");
    send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
    message_send_str = "";
    bzero(tom_ellis, sizeof(tom_ellis));
    fp1 = fopen(address, "a");
    cJSON *recog_user_by_auth = cJSON_GetObjectItem(token_get_user, authtoken);
    char* recog_user_by_auth_str = cJSON_Print(recog_user_by_auth);
    char recog_user_by_auth_char[100];
    strcpy(recog_user_by_auth_char, recog_user_by_auth_str);
    for (int i = 0; i < strlen(recog_user_by_auth_char); i++) {
        recog_user_by_auth_char[i] = recog_user_by_auth_char[i+1];
    }
    recog_user_by_auth_char[strlen(recog_user_by_auth_char)-1] = recog_user_by_auth_char[strlen(recog_user_by_auth_char)];
    cJSON* content_channel = cJSON_CreateObject();
    char* content_channel_str;
    cJSON_AddItemToObject(content_channel, "sender", cJSON_CreateString("server"));
    char channel_message[200];
    sprintf(channel_message, "%s joined.", recog_user_by_auth_char);
    cJSON_AddItemToObject(content_channel, "content", cJSON_CreateString(channel_message));
    content_channel_str = cJSON_PrintUnformatted(content_channel);
    fprintf(fp1,"%s\n", content_channel_str);
    fclose(fp1);
    cJSON_AddItemToObject(token_get_num_read_message, authtoken, cJSON_CreateNumber(0));
}

void Logout(char* buffer)
{
    char authtoken[100];
    sscanf(buffer,"%s", authtoken);
    cJSON* message_send = cJSON_CreateObject();
    char* message_send_str;
    char* check_AuthToken_arr_str;
    char check_AuthToken_str[100] = "\"";
    strcat(check_AuthToken_str, authtoken);
    strcat(check_AuthToken_str, "\"");
    cJSON* check_AuthToken_arr = cJSON_CreateObject();
    int flag = 0;
    for (int i = 0; i < num_online_user; i++) {
        check_AuthToken_arr = cJSON_GetArrayItem(online_token, i);
        check_AuthToken_arr_str = cJSON_PrintUnformatted(check_AuthToken_arr);
        if (strcmp(check_AuthToken_arr_str, check_AuthToken_str) == 0) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("Wrong AuthToken"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON *recog_user_by_auth = cJSON_GetObjectItem(token_get_user, authtoken);
    char* recog_user_by_auth_str = cJSON_PrintUnformatted(recog_user_by_auth);
    cJSON* check_log_user_arr;
    char* check_log_user_arr_str;
    int n = -2;
    for (int i = 0; i < num_online_user; i++) {
        check_log_user_arr = cJSON_GetArrayItem(online_user, i);
        check_log_user_arr_str = cJSON_PrintUnformatted(check_log_user_arr);
        if (strcmp(check_log_user_arr_str, recog_user_by_auth_str) == 0) {
            n = i;
            break;
        }
    }
        if (n < 0) {
            return;
        }
        cJSON_DeleteItemFromArray(online_user, n);
        cJSON_DeleteItemFromArray(online_token, n);
        cJSON_DeleteItemFromObject(token_get_user, authtoken);
        cJSON_DeleteItemFromObject(token_get_channel, authtoken);
        num_online_user --;
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString(""));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
}

void Send_Message(char* buffer)
{
    int a;
    cJSON* message_send = cJSON_CreateObject();
    char* message_send_str;
    char message[5000];
    char authtoken[100]={};
    int how_many = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ',') {
            how_many++;
        }
    }
    int k = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if ( (buffer[i] == ',') && (how_many > 1) ) {
            how_many--;
            message[i] = buffer[i];
        }
        else if ( (buffer[i] == ',') && (how_many == 1) ) {
            message[i] = '\0';
            for (int j = i+2; j < strlen(buffer); j++) {
                authtoken[k++] = buffer[j];
            }
            authtoken[k-1] = '\0';
            a = k;
            break;
        }
        else
        message[i] = buffer[i];
    }
    char* check_AuthToken_arr_str;
    char check_AuthToken_str[100] = {};
    strcpy(check_AuthToken_str, "\"");
    strcat(check_AuthToken_str, authtoken);
    strcat(check_AuthToken_str, "\"");
    cJSON* check_AuthToken_arr = cJSON_CreateObject();
    int flag = 0;
    for (int i = 0; i < num_online_user; i++) {
        check_AuthToken_arr = cJSON_GetArrayItem(online_token, i);
        check_AuthToken_arr_str = cJSON_PrintUnformatted(check_AuthToken_arr);
        if (strcmp(check_AuthToken_arr_str, check_AuthToken_str) == 0) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("Wrong AuthToken"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON *recog_user_by_auth = cJSON_GetObjectItem(token_get_user, authtoken);
    char* recog_user_by_auth_str = cJSON_PrintUnformatted(recog_user_by_auth);
    char recog_user_by_auth_char[100];
    strcpy(recog_user_by_auth_char, recog_user_by_auth_str);
    for (int i = 0; i < strlen(recog_user_by_auth_char); i++) {
        recog_user_by_auth_char[i] = recog_user_by_auth_char[i+1];
    }
    recog_user_by_auth_char[strlen(recog_user_by_auth_char)-1] = recog_user_by_auth_char[strlen(recog_user_by_auth_char)];
    cJSON *recog_channel_by_auth = cJSON_GetObjectItem(token_get_channel, authtoken);
    char* recog_channel_by_auth_str = cJSON_PrintUnformatted(recog_channel_by_auth);
    char recog_channel_by_auth_char[100];
    strcpy(recog_channel_by_auth_char, recog_channel_by_auth_str);
    for (int i = 0; i < strlen(recog_channel_by_auth_char); i++) {
        recog_channel_by_auth_char[i] = recog_channel_by_auth_char[i+1];
    }
    recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)-1] = recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)];
    char address[150];
    strcpy(address, address_channel_list);
    strcat(address, recog_channel_by_auth_char);
    strcat(address, ".json");
    FILE* fp1 = fopen(address, "a");
    if (fp1 == NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("this user isn't jonined in this channel"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        fclose(fp1);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(message_send, "content", cJSON_CreateString(""));
    message_send_str = cJSON_PrintUnformatted(message_send);
    char tom_ellis[200];
    strcpy(tom_ellis, message_send_str);
    strcat(tom_ellis, "\0");
    send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
    message_send_str = "";
    bzero(tom_ellis, sizeof(tom_ellis));
    cJSON* channel_content = cJSON_CreateObject();
    char* channel_content_str;
    cJSON_AddItemToObject(channel_content, "sender", cJSON_CreateString(recog_user_by_auth_char));
    cJSON_AddItemToObject(channel_content, "content", cJSON_CreateString(message));
    channel_content_str = cJSON_PrintUnformatted(channel_content);
    fp1 = fopen(address, "a");
    fprintf(fp1, "%s\n", channel_content_str);
    fclose(fp1);
}

void Refresh(char* buffer)
{
    char authtoken[100];
    sscanf(buffer,"%s", authtoken);
    cJSON* message_send = cJSON_CreateObject();
    char* message_send_str;
    char* check_AuthToken_arr_str;
    char check_AuthToken_str[100] = "\"";
    strcat(check_AuthToken_str, authtoken);
    strcat(check_AuthToken_str, "\"");
    cJSON* check_AuthToken_arr = cJSON_CreateObject();
    int flag = 0;
    for (int i = 0; i < num_online_user; i++) {
        check_AuthToken_arr = cJSON_GetArrayItem(online_token, i);
        check_AuthToken_arr_str = cJSON_PrintUnformatted(check_AuthToken_arr);
        if (strcmp(check_AuthToken_arr_str, check_AuthToken_str) == 0) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("Wrong AuthToken"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON* num_read_messsage = cJSON_GetObjectItem(token_get_num_read_message, authtoken);
    cJSON* recog_channel_by_auth = cJSON_GetObjectItem(token_get_channel, authtoken);
    char* recog_channel_by_auth_str = cJSON_PrintUnformatted(recog_channel_by_auth);
    char recog_channel_by_auth_char[100];
    strcpy(recog_channel_by_auth_char, recog_channel_by_auth_str);
    for (int i = 0; i < strlen(recog_channel_by_auth_char); i++) {
        recog_channel_by_auth_char[i] = recog_channel_by_auth_char[i+1];
    }
    recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)-1] = recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)];
    char address[150];
    strcpy(address, address_channel_list);
    strcat(address, recog_channel_by_auth_char);
    strcat(address, ".json");
    FILE* fp1 = fopen(address, "a");
    if (fp1 == NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("this user isn't jonined in this channel"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        fclose(fp1);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    int num_read = num_read_messsage->valueint;
    int count;
    count = 0;
    bzero(address, sizeof(address));
    strcpy(address, address_channel_list);
    strcat(address, recog_channel_by_auth->valuestring);
    strcat(address, ".json");
    fp1 = fopen(address, "r");
    char transfer[10000];
    cJSON* transfer_arr = cJSON_CreateArray();
    while (!feof(fp1)) {
        fgets(transfer, 9999, fp1);
        if (count >= num_read) {
            cJSON* transfer_json = cJSON_Parse(transfer);
            cJSON_AddItemToArray(transfer_arr, transfer_json);
        }
        count++;
    }
    count--;
    cJSON_DeleteItemFromArray(transfer_arr, (count-num_read));
    num_read = count;
    cJSON_DeleteItemFromObject(token_get_num_read_message, authtoken);
    cJSON_AddItemToObject(token_get_num_read_message, authtoken, cJSON_CreateNumber(count));
    cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("List"));
    cJSON_AddItemToObject(message_send, "content", transfer_arr);
    message_send_str = cJSON_PrintUnformatted(message_send);
    fclose(fp1);
    char tom_ellis[10000];
    strcpy(tom_ellis, message_send_str);
    strcat(tom_ellis, "\0");
    send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
    message_send_str = "";
    bzero(tom_ellis, sizeof(tom_ellis));
}

void Channel_Members(char* buffer)
{
    cJSON* channel_member_arr = cJSON_CreateArray();
    char authtoken[100];
    sscanf(buffer,"%s", authtoken);
    cJSON* message_send = cJSON_CreateObject();
    char* message_send_str;
    char* check_AuthToken_arr_str;
    char check_AuthToken_str[100] = "\"";
    strcat(check_AuthToken_str, authtoken);
    strcat(check_AuthToken_str, "\"");
    cJSON* check_AuthToken_arr = cJSON_CreateObject();
    int flag = 0;
    for (int i = 0; i < num_online_user; i++) {
        check_AuthToken_arr = cJSON_GetArrayItem(online_token, i);
        check_AuthToken_arr_str = cJSON_PrintUnformatted(check_AuthToken_arr);
        if (strcmp(check_AuthToken_arr_str, check_AuthToken_str) == 0) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("Wrong AuthToken"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON *recog_channel_by_auth = cJSON_GetObjectItem(token_get_channel, authtoken);
    char* recog_channel_by_auth_str = cJSON_PrintUnformatted(recog_channel_by_auth);
    char recog_channel_by_auth_char[100];
    strcpy(recog_channel_by_auth_char, recog_channel_by_auth_str);
    for (int i = 0; i < strlen(recog_channel_by_auth_char); i++) {
        recog_channel_by_auth_char[i] = recog_channel_by_auth_char[i+1];
    }
    recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)-1] = recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)];
    char address[150];
    strcpy(address, address_channel_list);
    strcat(address, recog_channel_by_auth_char);
    strcat(address, ".json");
    FILE* fp1 = fopen(address, "a");
    if (fp1 == NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("this user isn't jonined in this channel"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        fclose(fp1);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON* get_online_token_arr;
    char* get_online_token_arr_str;
    for (int i = 0; i < num_online_user; i++) {
        get_online_token_arr = cJSON_GetArrayItem(online_token, i);
        get_online_token_arr_str = cJSON_PrintUnformatted(get_online_token_arr);
    char get_online_token_arr_char[100];
    strcpy(get_online_token_arr_char, get_online_token_arr_str);
    for (int i = 0; i < strlen(get_online_token_arr_char); i++) {
        get_online_token_arr_char[i] = get_online_token_arr_char[i+1];
    }
    get_online_token_arr_char[strlen(get_online_token_arr_char)-1] = get_online_token_arr_char[strlen(get_online_token_arr_char)];
    cJSON* recog_is_join_in_channel_by_auth = cJSON_GetObjectItem(token_get_channel, get_online_token_arr_char);
        char* recog_is_join_in_channel_by_auth_str;
        if (recog_is_join_in_channel_by_auth == NULL) {
            continue;
        }
        recog_is_join_in_channel_by_auth_str = cJSON_PrintUnformatted(recog_is_join_in_channel_by_auth);
        if (strcmp(recog_channel_by_auth_str, recog_is_join_in_channel_by_auth_str) == 0) {
            cJSON *recog_user_by_auth = cJSON_GetObjectItem(token_get_user, get_online_token_arr_char);
            char* recog_user_by_auth_str = cJSON_PrintUnformatted(recog_user_by_auth);
            char recog_user_by_auth_char[100];
            strcpy(recog_user_by_auth_char, recog_user_by_auth_str);
            for (int i = 0; i < strlen(recog_user_by_auth_char); i++) {
                recog_user_by_auth_char[i] = recog_user_by_auth_char[i+1];
            }
            recog_user_by_auth_char[strlen(recog_user_by_auth_char)-1] = recog_user_by_auth_char[strlen(recog_user_by_auth_char)];
            cJSON_AddItemToArray(channel_member_arr, cJSON_CreateString(recog_user_by_auth_char));
        }
    }
    cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("List"));
    cJSON_AddItemToObject(message_send, "content", channel_member_arr);
    message_send_str = cJSON_PrintUnformatted(message_send);
    char tom_ellis[200];
    strcpy(tom_ellis, message_send_str);
    strcat(tom_ellis, "\0");
    send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
    message_send_str = "";
    bzero(tom_ellis, sizeof(tom_ellis));
}

void Leave_channel(char* buffer)
{
    char authtoken[100];
    sscanf(buffer,"%s", authtoken);
    cJSON* message_send = cJSON_CreateObject();
    char* message_send_str;
    char* check_AuthToken_arr_str;
    char check_AuthToken_str[100] = "\"";
    strcat(check_AuthToken_str, authtoken);
    strcat(check_AuthToken_str, "\"");
    cJSON* check_AuthToken_arr = cJSON_CreateObject();
    int flag = 0;
    for (int i = 0; i < num_online_user; i++) {
        check_AuthToken_arr = cJSON_GetArrayItem(online_token, i);
        check_AuthToken_arr_str = cJSON_PrintUnformatted(check_AuthToken_arr);
        if (strcmp(check_AuthToken_arr_str, check_AuthToken_str) == 0) {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("Wrong AuthToken"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON *recog_channel_by_auth = cJSON_GetObjectItem(token_get_channel, authtoken);
    char* recog_channel_by_auth_str = cJSON_PrintUnformatted(recog_channel_by_auth);
    char recog_channel_by_auth_char[100];
    strcpy(recog_channel_by_auth_char, recog_channel_by_auth_str);
    for (int i = 0; i < strlen(recog_channel_by_auth_char); i++) {
        recog_channel_by_auth_char[i] = recog_channel_by_auth_char[i+1];
    }
    recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)-1] = recog_channel_by_auth_char[strlen(recog_channel_by_auth_char)];
    char address[150];
    strcpy(address, address_channel_list);
    strcat(address, recog_channel_by_auth_char);
    strcat(address, ".json");
    FILE* fp1 = fopen(address, "a");
    if (fp1 == NULL) {
        cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Error"));
        cJSON_AddItemToObject(message_send, "content", cJSON_CreateString("this user isn't jonined in this channel"));
        message_send_str = cJSON_PrintUnformatted(message_send);
        fclose(fp1);
        char tom_ellis[200];
        strcpy(tom_ellis, message_send_str);
        strcat(tom_ellis, "\0");
        send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
        message_send_str = "";
        bzero(tom_ellis, sizeof(tom_ellis));
        return;
    }
    cJSON *recog_user_by_auth = cJSON_GetObjectItem(token_get_user, authtoken);
    char* recog_user_by_auth_str = cJSON_PrintUnformatted(recog_user_by_auth);
    char recog_user_by_auth_char[100];
    strcpy(recog_user_by_auth_char, recog_user_by_auth_str);
    for (int i = 0; i < strlen(recog_user_by_auth_char); i++) {
        recog_user_by_auth_char[i] = recog_user_by_auth_char[i+1];
    }
    recog_user_by_auth_char[strlen(recog_user_by_auth_char)-1] = recog_user_by_auth_char[strlen(recog_user_by_auth_char)];
    cJSON* content_channel = cJSON_CreateObject();
    char* content_channel_str;
    cJSON_AddItemToObject(content_channel, "sender", cJSON_CreateString("server"));
    char channel_message[200];
    sprintf(channel_message, "%s left.", recog_user_by_auth_char);
    cJSON_AddItemToObject(content_channel, "content", cJSON_CreateString(channel_message));
    content_channel_str = cJSON_PrintUnformatted(content_channel);
    fp1 = fopen(address, "a");
    fprintf(fp1, "%s\n", content_channel_str);
    fclose(fp1);
    cJSON_DeleteItemFromObject(token_get_channel, authtoken);
    cJSON_AddItemToObject(message_send, "type", cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(message_send, "content", cJSON_CreateString(""));
    message_send_str = cJSON_PrintUnformatted(message_send);
    char tom_ellis[200];
    strcpy(tom_ellis, message_send_str);
    strcat(tom_ellis, "\0");
    send(client_socket, tom_ellis, sizeof(tom_ellis), 0);
    message_send_str = "";
    bzero(tom_ellis, sizeof(tom_ellis));
}
