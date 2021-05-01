/*
** EPITECH PROJECT, 2021
** B-NWP-400-STG-4-1-myftp-arthur.robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

char *commands[] = {
    "HELP",
    "USER",
    "PASS",
    "QUIT",
    "CWD",
    "NOOP",
    "PWD",
    "CDUP",
    "DELE",
    NULL
};

void (*cmdAction[])(client_t *client) = {
    help_cmd,
    user_cmd,
    password_cmd,
    quit_cmd,
    cwd_cmd,
    noop_cmd,
    pwd_cmd,
    cdup_cmd,
    dele_cmd,
};

bool check_command(client_t *client, server_t *server)
{
    int idx;
    int dataState = 0;

    for (idx = 0; commands[idx] && !strstr(client->buff, commands[idx]); idx++);
    if (!client->logged_in && idx > 2) {
        send_message("530 Not logged in.\n", client->ctrlSock);
        return false;
    }
    dataState = data_commands(client, server);
    if (dataState == -1)
        send_message("530 Open data connection before.\n", client->ctrlSock);
    if (dataState == 0 && commands[idx])
        cmdAction[idx](client);
    else if (dataState != 1)
        send_message("500 Syntax error\n", client->ctrlSock);
    if (strstr(client->buff, "QUIT") || strstr(client->buff, "PASV"))
        return true;
    memset(client->buff, 0, BUFF_SIZE);
    return false;
}

void server_loop(server_t *server)
{
    int maxFd;
    int state;
    bool running = true;

    while (running) {
        maxFd = populate_server_fd(server);
        state = select(maxFd + 1, &(server->fd), NULL, NULL, NULL);
        if (state < 0)
            perror("select error");
        else if (state == 0)
            continue;
        else
            running = manage_sockets(server, maxFd);
    }
}

int main(int ac, char **av)
{
    server_t *server = NULL;

    if ((ac == 2 && !strcmp(av[1], "-help")) || ac != 3) {
        print_help();
        exit(84);
    }
    server = init_server(av);
    server_loop(server);
    close(server->socketFd);
    free(server->buff);
    free(server->config);
    free(server);
    (void)ac;
    (void)av;
}