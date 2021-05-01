/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

int populate_server_fd(server_t *server)
{
    int maxFd = server->socketFd;

    FD_ZERO(&server->fd);
    FD_SET(server->socketFd, &server->fd);
    for (int i = 0; i < 10; i++) {
        if (server->clients[i] && maxFd < server->clients[i]->ctrlSock)
            maxFd = server->clients[i]->ctrlSock;
        if (server->clients[i] && server->dataTransfer && maxFd < server->clients[i]->dataSock)
            maxFd = server->clients[i]->dataSock;
        if (server->clients[i])
            FD_SET(server->clients[i]->ctrlSock, &server->fd);
        if (server->dataTransfer && server->clients[i])
            FD_SET(server->clients[i]->dataSock, &server->fd);
    }
    return maxFd;
}

void print_help(void)
{
    printf("USAGE: ./myftp port path\n");
    printf("\tport is the port number on which the server socket listens\n");
    printf("\tpath is the path to the home directory for the Anonymous user\n");
}

bool manage_sockets(server_t *server, int maxFd)
{
    for (int i = server->socketFd; i < maxFd + 1; i++) {
        if (i == server->socketFd && FD_ISSET(i, &server->fd)) {
            accept_connection(server);
        } else if (FD_ISSET(i, &server->fd)) {
            receive_message(client_by_socket(server, i), i);
            if (check_command(client_by_socket(server, i), server))
                remove_client_by_socket(server, i);
        }
    }
    return true;
}