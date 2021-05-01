/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include <netdb.h>
#include "../includes/server.h"

static char *format_port(int port)
{
    char *str = calloc(15, 1);

    sprintf(str, ",%d,%d)\n", port / 256, port % 256);
    return str;
}

static server_t *init_data_co2(server_t *server)
{
    FD_ZERO(&(server->fd));
    FD_SET(server->socketFd, &(server->fd));
    server->clients = malloc(sizeof(client_t *) * 10);
    if (!server->clients) {
        perror("Can't initialize clients.");
        exit(84);
    }
    for (int i = 0; i < 10; i++)
        server->clients[i] = NULL;
    if (listen(server->socketFd, 2) == -1)
        return NULL;
    return server;
}

static server_t *init_data_connection(char *path)
{
    socklen_t size = sizeof(struct sockaddr_in);
    server_t *serv = malloc(sizeof(server_t));
    int opt = 1;

    if (!serv || !(serv->config = calloc(1, sizeof(struct sockaddr_in))))
        return NULL;
    serv->path = strdup(path);
    serv->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    serv->config->sin_port = htons(0);
    serv->config->sin_family = AF_INET;
    serv->config->sin_addr.s_addr = INADDR_ANY;
    if (serv->socketFd < 0 || setsockopt(serv->socketFd, SOL_SOCKET,
    SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        exit(84);
    if (bind(serv->socketFd, (struct sockaddr *) serv->config, size) == -1 ||
    !(serv->buff = calloc(BUFF_SIZE, 1)))
        return NULL;
    serv->dataTransfer = true;
    serv->active = false;
    return init_data_co2(serv);
}

void pasv_cmd(client_t *cl, server_t *server)
{
    char *response = NULL;
    server_t *serv = NULL;
    socklen_t size = sizeof(struct sockaddr_in);
    struct sockaddr_in addr;

    if (fork() != 0 || !(response = calloc((45 + strlen(cl->ip)), 1)))
        return;
    close(server->socketFd);
    serv = init_data_connection(cl->path);
    serv->clients[0] = create_client(cl->ctrlSock, cl->path, cl->port, cl->ip);
    serv->clients[0]->logged_in = true;
    if (getsockname(serv->socketFd, (struct sockaddr *)&addr, &size) == -1)
        exit(0);
    sprintf(response, "227 Entering Passive Mode (%s%s",
    cl->ip, format_port(ntohs(addr.sin_port)));
    while (strchr(response, '.'))
        *(strchr(response, '.')) = ',';
    send_message(response, cl->ctrlSock);
    server_loop(serv);
    free(response);
    exit(0);
}