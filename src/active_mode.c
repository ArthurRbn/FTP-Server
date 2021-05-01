/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

int connect_to_client(server_t *server)
{
    struct sockaddr_in config;
    socklen_t socklen = sizeof(struct sockaddr_in);
    client_t *client = NULL;

    if (!server->dataTransfer || !server->active)
        return -1;
    client = server->clients[0];
    if ((client->dataSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    config.sin_family = AF_INET;
    config.sin_port = htons(client->cli_port);
    if (inet_pton(AF_INET, client->cli_ip, (void*)&(config.sin_addr)) == 0)
        return -1;
    if (connect(client->dataSock, (struct sockaddr *)&config, socklen) < 0)
        return -1;
    return client->dataSock;
}

static void parse_port(char *adress, char **ip, client_t *client)
{
    int count = 0;
    char *port1 = NULL;
    char *port2 = NULL;

    for (int i = 0; adress[i]; ++i) {
        count += adress[i] == ',' ? 1 : 0;
        if (count == 4 && !(*ip)) {
            *ip = strdup(adress);
            (*ip)[i] = 0;
            port1 = strdup(&(adress[i + 1]));
        } else if (count == 5 && !port2) {
            port2 = strdup(&((*ip)[i + 1]));
            port2[strcspn(port2, "\r\n")] = 0;
        }
    }
    client->cli_port = strtol(port1, NULL, 10) * 256 + strtol(port2, NULL, 10);
    free(port2);
    free(port1);
}

void port_cmd(client_t *client, server_t *server)
{
    int count = 0;
    char *adress = index(client->buff, ' ');
    char *ip = NULL;

    for (int i = 0; adress && adress[i]; i++)
        count += adress[i] == ',' ? 1 : 0;
    if (!adress || strcspn(++adress, ",0123456789") || count != 5) {
        send_message("501 Syntax error in parameters.\n", client->ctrlSock);
        return;
    }
    parse_port(adress, &ip, client);
    for (int i = 0; ip && ip[i]; i++)
        if (ip[i] == ',')
            ip[i] = '.';
    client->cli_ip = ip;
    server->dataTransfer = true;
    server->active = true;
}