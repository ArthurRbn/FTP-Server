/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

void user_cmd(client_t *client)
{
    char *username = index(client->buff, ' ');

    if (!username || strcspn(++username, "\r\n") == 0) {
        send_message("501 Syntax error in parameters or arguments.\n",
        client->ctrlSock);
        return;
    }
    username[strcspn(username, "\r\n")] = 0;
    if (strcmp(username, "Ano") == 0) {
        client->username = strdup(username);
        send_message("220 Service ready for new user.\n", client->ctrlSock);
    } else if (strlen(username)) {
        client->username = strdup(username);
        send_message("331 Username OK.\n", client->ctrlSock);
    }
}

void password_cmd(client_t *client)
{
    char *password = index(client->buff, ' ');

    if (!password || *(++password) != 13) {
        send_message("530 Not logged in.\n", client->ctrlSock);
        return;
    } else if (!client->username) {
        send_message("332 Need account for login.\n", client->ctrlSock);
        return;
    }
    client->passwd = strdup(password);
    client->logged_in = true;
    send_message("230 Password OK logged in\n", client->ctrlSock);
}

void noop_cmd(client_t *client)
{
    send_message("200 Requested file action okay, completed.\n",
    client->ctrlSock);
}

void help_cmd(client_t *client)
{
    send_message("214 Available commands: USER, PASS, CWD, CDUP, QUIT, DELE,\
    PWD, PASV, PORT, HELP, NOOP, RETR, STOR, LIST\n", client->ctrlSock);
}

void quit_cmd(client_t *client)
{
    send_message("221 Service closing control connection.\n", client->ctrlSock);
}