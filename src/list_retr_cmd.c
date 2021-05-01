/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

static void process_list_cmd(char *path, client_t *client, char *buff)
{
    FILE *file = NULL;

    if (!is_dir_valid(path)) {
        send_message("550 Requested action not taken.\n", client->ctrlSock);
        return;
    }
    sprintf(buff, "ls -l %s", path);
    file = popen(buff, "r");
    while (fgets(buff, BUFF_SIZE, file)) {
        send_message(buff, client->dataSock);
    }
    send_message("226 Requested file action successful.\n", client->ctrlSock);
    fclose(file);
    shutdown(client->dataSock, 2);
    close(client->dataSock);
    free(buff);
    client->dataSock = -1;
}

void list_cmd(client_t *client, server_t *server)
{
    char *buff = calloc(BUFF_SIZE, 1);
    char *path = NULL;

    if (!server->dataTransfer) {
        send_message("425 Select a data transfer mode.\n", client->ctrlSock);
        return;
    }
    send_message("150 About to open connection.\n", client->ctrlSock);
    if (accept_connection(server) == -1) {
        send_message("500 Can't accept connection.\n", client->ctrlSock);
        return;
    }
    if (index(client->buff, ' ') && (path = strdup(index(client->buff, ' ') + 1)) && strlen(path) < 2) {
        path[strcspn(path, "\r\n")] = 0;
    } else
        path = client->path;
    process_list_cmd(path, client, buff);
}

static void process_retr(char *path, char *buff, client_t *cli)
{
    FILE *fd;

    path[strcspn(path, "\r\n")] = 0;
    if ((fd = fopen(path, "r"))) {
        while (fread(buff, 1, BUFF_SIZE, fd)) {
            send_message(buff, cli->dataSock);
        }
        send_message("226 Requested file action successful.\n", cli->ctrlSock);
        fclose(fd);
    }
    shutdown(cli->dataSock, 2);
    close(cli->dataSock);
    cli->dataSock = -1;
    free(buff);
}

void retr_cmd(client_t *cli, server_t *server)
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
    } else if (chdir(cli->path) != 0 || !buff || !is_file_valid(path)) {
        send_message("550 Requested action not taken.\n", cli->ctrlSock);
        return;
    }
    send_message("150 About to open connection.\n", cli->ctrlSock);
    if (accept_connection(server) == -1) {
        send_message("500 Can't accept connection.\n", cli->ctrlSock);
    } else
        process_retr(path, buff, cli);
}