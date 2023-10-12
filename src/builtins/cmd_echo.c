/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_echo.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ssemanco <ssemanco@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/03 11:06:15 by ssemanco      #+#    #+#                 */
/*   Updated: 2023/10/12 21:30:34 by eunbi         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int cmd_echo(char **cmd)
{
    int printNewline = 1;
    int i;
	int count;

	i = 1;
	count = 0;
	while (cmd[count]!= NULL)
		count++;
    if (count > 1 && strcmp(cmd[1], "-n") == 0) //if option -n exist
    {
        printNewline = 0;
        i = 2;
    }
    while (i < count) 
    {
        if (ft_strncmp(cmd[i], "", 1) == 0)
            i++;
        else
            printf("%s", cmd[i]);
		if (i < count - 1)
            printf(" ");
        i++;
    } 
    if (printNewline)
        printf("\n");
    return 0;
}