/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

client_t *create_client(int socket, char *path, int port, char *ip)
{
    client_t *client = malloc(sizeof(client_t));

    if (!client)
        exit(84);
    client->ctrlSock = socket;
    client->dataSock = -1;
    client->port = port;
    client->cli_port = -1;
    client->ip = ip;
    client->cli_ip = NULL;
    client->passwd = NULL;
    client->username = NULL;
    client->buff = calloc(BUFF_SIZE, 1);
    client->path = strdup(path);
    client->logged_in = false;
    if (!client->buff) {
        perror("malloc failed\n");
        exit(84);
    }
    return client;
}

client_t *client_by_socket(server_t *server, int socket)
{
    for (int i = 0; i < 10; i++) {
        if (server->clients[i] && (server->clients[i]->ctrlSock == socket || server->clients[i]->dataSock == socket))
            return server->clients[i];
    }
    return NULL;
}

void remove_client_by_socket(server_t *server, int socket)
{
    for (int i = 0; i < 10; i++) {
        if (server->clients[i] && server->clients[i]->ctrlSock == socket) {
            close(server->clients[i]->ctrlSock);
            if (server->clients[i]->dataSock != -1)
                close(server->clients[i]->dataSock);
            free(server->clients[i]->buff);
            free(server->clients[i]->passwd);
            free(server->clients[i]->username);
            free(server->clients[i]);
            server->clients[i] = NULL;
            return;
        }
    }
}