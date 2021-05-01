/*
** EPITECH PROJECT, 2021
** B_NWP_400_STG_4_1_myftp_arthur_robine
** File description:
** Created by arthur,
*/

#include "../includes/server.h"

bool is_dir_valid(char *path)
{
    struct stat stats;

    if (!path || !strlen(path))
        return false;
    path[strcspn(path, "\r\n")] = 0;
    if (stat(path, &stats) < 0 || !S_ISDIR(stats.st_mode))
        return false;
    return true;
}

bool is_file_valid(char *path)
{
    struct stat stats;

    if (!path || !strlen(path))
        return false;
    path[strcspn(path, "\r\n")] = 0;
    if (stat(path, &stats) < 0 || !S_ISREG(stats.st_mode))
        return false;
    return true;
}

char *get_absolute_path(char *relative)
{
    long n;
    char *buff;
    char *new;

    if (chdir(relative) != 0) {
        return NULL;
    } else if ((n = pathconf(".", _PC_PATH_MAX)) == -1)
        n = 1024;
    if (!(buff = calloc(n, sizeof(char))) || getcwd(buff, n) == NULL) {
        return NULL;
    } else {
        new = strdup(buff);
        free(buff);
        return new;
    }
}