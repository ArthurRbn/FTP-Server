/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"


char *cmd[] = {
    "LIST",
    "PASV",
    "RETR",
    "STOR",
    "PORT",
    NULL
};

void (*actions[])(client_t *client, server_t *serv) = {
    list_cmd,
    pasv_cmd,
    retr_cmd,
    store_cmd,
    port_cmd
};

int data_commands(client_t *client, server_t *server)
{
    int idx = 0;
    for (idx = 0; cmd[idx] && !strstr(client->buff, cmd[idx]); idx++);
    if (cmd[idx]) {
        actions[idx](client, server);
        return 1;
    }
    return 0;
}

static void process_store(char *path, char *buff, client_t *cli)
{
    FILE *fd;

    path[strcspn(path, "\r\n")] = 0;
    if ((fd = fopen(path, "w+"))) {
        while (read(cli->dataSock, buff, BUFF_SIZE) != 0) {
            buff[strcspn(buff, "\r")] = 0;
            fwrite(buff, 1, strlen(buff), fd);
            memset(buff, 0, BUFF_SIZE);
        }
        send_message("226 Requested file action successful.\n", cli->ctrlSock);
        fclose(fd);
    }
    shutdown(cli->dataSock, 2);
    close(cli->dataSock);
    cli->dataSock = -1;
    free(buff);
}

void store_cmd(client_t *cli, server_t *server)
{
    char *path = NULL;
    char *buff = calloc(BUFF_SIZE, 1);

    if (!server->dataTransfer) {
        send_message("425 Select a data transfer mode.\n", cli->ctrlSock);
        return;
    }
    if (!index(cli->buff, ' ') || !(path = strdup(index(cli->buff, ' '))) || *(++path) == 13) {
        send_message("501 Syntax error in parameters.\n", cli->ctrlSock);
        return;
    } else if (chdir(cli->path) != 0 || !buff || is_dir_valid(path)) {
        send_message("550 Requested action not taken.\n", cli->ctrlSock);
        return;
    }
    send_message("150 About to open connection.\n", cli->ctrlSock);
    if (accept_connection(server) == -1) {
        send_message("500 Can't accept connection.\n", cli->ctrlSock);
    } else
        process_store(path, buff, cli);
}