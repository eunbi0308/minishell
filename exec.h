#ifndef EXEC_H
# define EXEC_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "libft/libft.h"

typedef struct s_env
{
    char* key;
    char* value;
    struct s_env *next;
}               t_env;

t_env* createLinkedList(char** envp);
int cmd_echo(int argc, char **argv);
void cmd_cd(int argc, char **path);
void cmd_pwd();

#endif