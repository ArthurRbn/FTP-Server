/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

server_t *init_server_2(server_t *server)
{
    if (bind(server->socketFd, (struct sockaddr *)server->config, sizeof(struct sockaddr_in)) < 0 || !(server->buff = calloc(BUFF_SIZE, 1))) {
        perror("Error during server initialization");
        exit(84);
    }
    FD_ZERO(&(server->fd));
    FD_SET(server->socketFd, &(server->fd));
    if (listen(server->socketFd, 10) < 0) {
        perror("Error on server initialization: listen");
        exit(84);
    }
    server->clients = malloc(sizeof(client_t *) * 10);
    if (!server->clients) {
        perror("Can't initialize clients.");
        exit(84);
    }
    for (int i = 0; i < 10; i++)
        server->clients[i] = NULL;
    return server;
}

server_t *init_server(char **av)
{
    server_t *server = malloc(sizeof(server_t));
    int opt = 1;
    long port = strtol(av[1], NULL, 10);

    if (port == 0 || !is_dir_valid(av[2]) || !server || !(server->config = calloc(1, sizeof(struct sockaddr_in)))) {
        perror("Error during server initialization");
        exit(84);
    }
    server->path = get_absolute_path(av[2]);
    server->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socketFd < 0 || setsockopt(server->socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        exit(84);
    server->config->sin_family = AF_INET;
    server->config->sin_addr.s_addr = htonl(INADDR_ANY);
    server->config->sin_port = htons(port);
    server->dataTransfer = false;
    server->active = false;
    return init_server_2(server);
}

int accept_connection(server_t *server)
{
    struct sockaddr_in ip;
    socklen_t socklen = sizeof(ip);
    int freeSlot = 0;
    int new = connect_to_client(server);

    if (new < 0 && (new = accept(server->socketFd, (struct sockaddr *)&ip, &socklen)) < 0) {
        return -1;
    } else if (server->dataTransfer) {
        server->clients[0]->dataSock = new;
        send_message("220 FTP server ready for data transfer !\n", new);
        return new;
    }
    for (; freeSlot < 10 && server->clients[freeSlot]; freeSlot++);
    if (freeSlot == 10)
        return -1;
    server->clients[freeSlot] = create_client(new, server->path,
    ntohs(ip.sin_port), inet_ntoa(ip.sin_addr));
    send_message("220 FTP server ready !\n", new);
    return new;
}

void receive_message(client_t *client, int socket)
{
    if (!client)
        return;
    read(socket, client->buff, BUFF_SIZE);
    if (!strlen(client->buff))
        client->buff[0] = 0;
}

void send_message(char *msg, int socket)
{
    int maxFd = socket;
    fd_set fd;
    int state = 0;

    FD_ZERO(&fd);
    FD_SET(socket, &fd);
    state = select(maxFd + 1, NULL, &fd, NULL, NULL);
    if (state < 1)
        return;
    else if (FD_ISSET(socket, &fd))
        write(socket, msg, strlen(msg));
}