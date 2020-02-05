#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "cJSON.h"
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
int client_socket;

void make_socket()
{
    struct sockaddr_in servaddr;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to the server failed...\n");
        exit(0);
    }
    else
        printf("Successfully connected to the server..\n");

}

void account_menu(void);

void Register(void);

void Login(void);

void main_menu(void);

void Create_Channel(void);

void Join_Channel(void);

void Logout(void);

void chat_menu(void);

void Send_Message(void);

void Refresh(void);

void Channel_Members(void);

void Leave_channel(void);

char AuthToken[1000] = {};

int main()
{
    account_menu();
    
    return 0;
}

void account_menu()
{
    printf("account menu:\n1: Register\n2: Login\n3: exit\n");
    int choose;
       scanf("%d", &choose);
    switch (choose) {
        case 1:
            Register();
            break;
        case 2:
            Login();
            break;
        case 3:
            exit(0);
    }
}

void Register()
{
    char buffer[100];
    char username[50];
    char password[50];
    printf("create your username\n");
    scanf("%s", username);
    printf("create your password\n");
    scanf("%s", password);
    strcpy(buffer, "register ");
    strcat(buffer, username);
    strcat(buffer, ", ");
    strcat(buffer, password);
    strcat(buffer, "\n");
    make_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    bzero(buffer, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    shutdown(client_socket, SHUT_RDWR);
    cJSON* json_buffer = cJSON_Parse(buffer);
    cJSON* type_json = cJSON_GetObjectItem(json_buffer, "type");
    char type[1000] = {};
    strcpy(type, type_json->valuestring);
    if (strcmp(type, "Error") == 0) {
        printf("Error\n");
        account_menu();
    }
    else{
        printf("please login:\n");
        Login();
    }
}

void Login()
{
    char buffer[100];
    char username[50];
    char password[50];
    printf("enter your username:\n");
    scanf("%s", username);
    printf("enter your password:\n");
    scanf("%s", password);
    strcpy(buffer, "login ");
    strcat(buffer, username);
    strcat(buffer, ", ");
    strcat(buffer, password);
    strcat(buffer, "\n");
    make_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
      bzero(buffer, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    shutdown(client_socket, SHUT_RDWR);
    cJSON* json_buffer = cJSON_Parse(buffer);
       cJSON* type_json = cJSON_GetObjectItem(json_buffer, "type");
       cJSON* content_json = cJSON_GetObjectItem(json_buffer, "content");
       char type[1000] = {};
       char content[1000] = {};
       strcpy(type, type_json->valuestring);
       strcpy(content, content_json->valuestring);
       strcpy(AuthToken, content);
       bzero(buffer, sizeof(buffer));
    if (strcmp(type, "Error") == 0) {
        printf("Error\n");
        account_menu();
    }
      else
          main_menu();
}

void main_menu(){
    printf("1: Create Channel\n2: Join Channel\n3: Logout\n");
     int choose;
       scanf("%d", &choose);
       switch (choose) {
           case 1:
               Create_Channel();
               break;
           case 2:
               Join_Channel();
               break;
           case 3:
               Logout();
               break;
       }
}

void Create_Channel()
{
       char buffer[350];
       char channel_name[100];
    bzero(channel_name, sizeof(channel_name));
       printf("please enter your channel name:\n");
       scanf("\n%[^\n]", channel_name);
       strcpy(buffer, "create channel ");
       strcat(buffer, channel_name);
       strcat(buffer, ", ");
       strcat(buffer, AuthToken);
       strcat(buffer, "\n");
       make_socket();
       send(client_socket, buffer, sizeof(buffer), 0);
       bzero(buffer, sizeof(buffer));
       recv(client_socket, buffer, sizeof(buffer), 0);
    shutdown(client_socket, SHUT_RDWR);
    cJSON* json_buffer = cJSON_Parse(buffer);
    cJSON* type_json = cJSON_GetObjectItem(json_buffer, "type");
    char type[1000] = {};
    strcpy(type, type_json->valuestring);
    if (strcmp(type, "Error") == 0) {
        printf("Error!\n");
        main_menu();
    }
    else;
        chat_menu();
}

void Join_Channel()
{
    char buffer[350];
    char channel_name[MAX];
    printf("enter the channel name:\n");
    scanf("\n%[^\n]", channel_name);
    strcpy(buffer, "join channel ");
    strcat(buffer, channel_name);
    strcat(buffer, ", ");
    strcat(buffer, AuthToken);
    strcat(buffer, "\n");
    make_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    bzero(buffer, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    shutdown(client_socket, SHUT_RDWR);
    cJSON* json_buffer = cJSON_Parse(buffer);
    cJSON* type_json = cJSON_GetObjectItem(json_buffer, "type");
    char type[1000] = {};
    strcpy(type, type_json->valuestring);
    if (strcmp(type, "Error") == 0) {
        printf("Error\n");
        main_menu();
    }
    else
        chat_menu();
}

void Logout()
{
    char buffer[210];
    strcpy(buffer, "logout ");
    strcat(buffer, AuthToken);
    strcat(buffer, "\n");
    make_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    bzero(buffer, sizeof(buffer));
    shutdown(client_socket, SHUT_RDWR);
        account_menu();
}

void chat_menu()
{
    printf("1: Send Message:\n2: Refresh\n3: Channel Members\n4: Leave Channel\n");
    int choose;
    scanf("%d", &choose);
    switch (choose) {
        case 1:
            Send_Message();
            break;
        case 2:
            Refresh();
            break;
        case 3:
            Channel_Members();
        break;
        case 4:
            Leave_channel();
        break;
    }
}

void Send_Message()
{
          char buffer[7010];
          char Message[5000];
          bzero(Message, sizeof(Message));
          printf("enter Your message:\n");
          scanf("\n%[^\n]", Message);
          bzero(buffer, sizeof(buffer));
          strcpy(buffer, "send ");
          strcat(buffer, Message);
          strcat(buffer, ", ");
          strcat(buffer, AuthToken);
          strcat(buffer, "\n");
          make_socket();
          send(client_socket, buffer, sizeof(buffer), 0);
          bzero(buffer, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        shutdown(client_socket, SHUT_RDWR);
        cJSON* json_buffer = cJSON_Parse(buffer);
        cJSON* type_json = cJSON_GetObjectItem(json_buffer, "type");
        char type[100] = {};
        strcpy(type, type_json->valuestring);
        if (strcmp(type, "Error") == 0) {
        printf("your message wasn't send!!\n");
    }
    chat_menu();
}
void Refresh()
{
   char buffer[10000];
   strcpy(buffer, "refresh ");
   strcat(buffer, AuthToken);
   strcat(buffer, "\n");
   make_socket();
    char type[1000]={};
    char content[1000]={};
   send(client_socket, buffer, sizeof(buffer), 0);
   bzero(buffer, sizeof(buffer));
   recv(client_socket, buffer, sizeof(buffer), 0);
    shutdown(client_socket, SHUT_RDWR);
    cJSON* json_buffer = cJSON_Parse(buffer);
    cJSON* content_json = cJSON_GetObjectItem(json_buffer, "content");
    for (int i = 0; i < cJSON_GetArraySize(content_json); i++) {
        cJSON* content_array = cJSON_GetArrayItem(content_json, i);
        cJSON* content_cjson = cJSON_GetObjectItem(content_array, "content");
        cJSON* sender = cJSON_GetObjectItem(content_array, "sender");
        strcpy(type, sender->valuestring);
        strcpy(content, content_cjson->valuestring);
        printf("%s : %s\n", type, content);
        bzero(content, sizeof(content));
        bzero(type, sizeof(type));
    }
    bzero(buffer, sizeof(buffer));
    chat_menu();
}

void Channel_Members()
{
    char buffer[910];
    strcpy(buffer, "channel members ");
    strcat(buffer, AuthToken);
    strcat(buffer, "\n");
    make_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    bzero(buffer, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    shutdown(client_socket, SHUT_RDWR);
    char content[1000];
    cJSON* json_buffer = cJSON_Parse(buffer);
    cJSON* content_json = cJSON_GetObjectItem(json_buffer, "content");
    for (int i = 0; i < cJSON_GetArraySize(content_json); i++) {
        cJSON* content_array = cJSON_GetArrayItem(content_json, i);
        strcpy(content, content_array->valuestring);
        printf("%s,  \n", content);
    }
        bzero(buffer, sizeof(buffer));
        bzero(content, sizeof(content));
        chat_menu();
}

void Leave_channel()
{
    char buffer[210];
    strcpy(buffer, "leave ");
    strcat(buffer, AuthToken);
    strcat(buffer, "\n");
    make_socket();
    send(client_socket, buffer, sizeof(buffer), 0);
    bzero(buffer, sizeof(buffer));
    shutdown(client_socket, SHUT_RDWR);
           main_menu();
}


