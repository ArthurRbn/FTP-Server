/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#ifndef B_NWP_400_STG_4_1_MYFTP_ARTHUR_ROBINE_CLIENT_H
#define B_NWP_400_STG_4_1_MYFTP_ARTHUR_ROBINE_CLIENT_H

#include <stdbool.h>

typedef struct server_s server_t;

typedef struct client_s
{
    char *username;
    char *passwd;
    int ctrlSock;
    int dataSock;
    int port;
    long cli_port;
    char *ip;
    char *cli_ip;
    char *buff;
    char *path;
    bool logged_in;
} client_t;

client_t *create_client(int socket, char *path, int port, char *ip);
client_t *client_by_socket(server_t *server, int socket);
void remove_client_by_socket(server_t *server, int socket);

#endif //B_NWP_400_STG_4_1_MYFTP_ARTHUR_ROBINE_CLIENT_H