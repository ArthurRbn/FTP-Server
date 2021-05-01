/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

void cdup_cmd(client_t *cli)
{
    long n;
    char *buff;

    if (chdir(cli->path) != 0) {
        send_message("550 Requested action not taken.\n", cli->ctrlSock);
        return;
    } else if ((n = pathconf(".", _PC_PATH_MAX)) == -1)
        n = 1024;
    if ((buff = calloc(n, sizeof(char))) && getcwd(buff, n) != NULL) {
        if (rindex(buff, '/') && rindex(buff, '/') != buff) {
            *(rindex(buff, '/')) = 0;
            free(cli->path);
            cli->path = strdup(buff);
            send_message("200 Command okay.\n", cli->ctrlSock);
        } else
            send_message("550 Requested action not taken.\n", cli->ctrlSock);
    } else
        send_message("550 Requested action not taken.\n", cli->ctrlSock);
    free(buff);
}

void cwd_cmd(client_t *cli)
{
    char *path = NULL;

    if (!index(cli->buff, ' ') || !(path = strdup(index(cli->buff, ' ') + 1)) || *path == 13) {
        send_message("550 Missing parameter.\n", cli->ctrlSock);
        return;
    }
    path[strlen(path) - 2] = 0;
    if (!is_dir_valid(path)) {
        send_message("550 Requested action not taken.\n", cli->ctrlSock);
        return;
    }
    if (cli->path)
        free(cli->path);
    cli->path = get_absolute_path(path);
    send_message("250 Requested file action okay, completed.\n", cli->ctrlSock);
}

void dele_cmd(client_t *client)
{
    char *path = NULL;

    if (!index(client->buff, ' ') || !(path = strdup(index(client->buff, ' '))) || *(++path) == 13) {
        send_message("501 Syntax error in parameters or arguments.\n", client->ctrlSock);
        return;
    }
    path[strlen(path) - 2] = 0;
    if (!is_file_valid(path)) {
        send_message("550 Requested action not taken.\n", client->ctrlSock);
        return;
    }
    if (remove(path) == 0) {
        send_message("250 Requested file action okay, completed.\n", client->ctrlSock);
    } else
        send_message("550 Requested action not taken.\n", client->ctrlSock);
}

void pwd_cmd(client_t *client)
{
    char *str = "257 \"";
    char *str2 = "\"\n";
    char *pwd = get_absolute_path(client->path);
    char *concat = NULL;

    if (!pwd) {
        send_message("550 Requested action not taken.\n", client->ctrlSock);
        return;
    } else {
        concat = malloc(strlen(str) + strlen(str2) + strlen(pwd) + 1);
        strcpy(concat, str);
        strcat(concat, pwd);
        strcat(concat, str2);
        send_message(concat, client->ctrlSock);
    }
    free(pwd);
}