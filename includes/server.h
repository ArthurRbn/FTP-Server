/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#ifndef B_NWP_400_STG_4_1_MYFTP_ARTHUR_ROBINE_SERVER_H
#define B_NWP_400_STG_4_1_MYFTP_ARTHUR_ROBINE_SERVER_H

#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "client.h"

#define BUFF_SIZE 1000

typedef struct server_s
{
    char *buff;
    int socketFd;
    struct sockaddr_in *config;
    fd_set fd;
    client_t **clients;
    char *path;
    bool dataTransfer;
    bool active;
} server_t;

server_t *init_server(char **av);
int accept_connection(server_t *server);
void receive_message(client_t *client, int socket);
void send_message(char *msg, int socket);
void server_loop(server_t *server);


// COMMANDS
void user_cmd(client_t *client);
void password_cmd(client_t *client);
void cwd_cmd(client_t *client);
void noop_cmd(client_t *client);
void pwd_cmd(client_t *client);
void cdup_cmd(client_t *client);
void help_cmd(client_t *client);
void dele_cmd(client_t *client);
void quit_cmd(client_t *client);
void pasv_cmd(client_t *client, server_t *server);
void retr_cmd(client_t *client, server_t *server);
void list_cmd(client_t *client, server_t *server);
void store_cmd(client_t *cli, server_t *server);
void port_cmd(client_t *client, server_t *server);

bool check_command(client_t *client, server_t *server);
int data_commands(client_t *client, server_t *server);


// UTILS
bool is_dir_valid(char *path);
bool is_file_valid(char *path);
char *get_absolute_path(char *relative);
int populate_server_fd(server_t *server);
void print_help(void);
bool manage_sockets(server_t *server, int maxFd);
int connect_to_client(server_t *server);

#endif //B_NWP_400_STG_4_1_MYFTP_ARTHUR_ROBINE_SERVER_H