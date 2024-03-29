/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: eucho <eucho@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/02 16:13:05 by eucho         #+#    #+#                 */
/*   Updated: 2023/10/29 16:40:43 by eucho         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error.h"
#include <sys/types.h>
#include <sys/stat.h>

static int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (-1);
	if (S_ISDIR(path_stat.st_mode))
		return (1);
	else
		return (0);
}

char	**get_cmd_dirs(t_env *envp)
{
	char	*tmp;
	char	**dirs;
	t_env	*head;

	head = envp;
	dirs = NULL;
	while (head)
	{
		if (ft_strncmp(head->key, "PATH", 4) == 0)
		{
			tmp = ft_strdup(head->value + 1);
			dirs = ft_split(tmp, ':');
			free(tmp);
			return (dirs);
		}
		head = head->next;
	}
	return (NULL);
}

static char	*command_check(char **path, char *cmd)
{
	char	*command;
	char	*tmp;

	if (path == NULL)
		return (NULL);
	if ((access(cmd, X_OK) == 0))
		return (cmd);
	while (*path)
	{
		tmp = ft_strjoin(*path, "/");
		command = ft_strjoin(tmp, cmd);
		if (command == NULL)
			exit(EXIT_FAILURE);
		if (access(command, F_OK) == 0)
		{
			free(tmp);
			return (command);
		}
		path++;
		free(tmp);
		free(command);
	}
	return (NULL);
}

static int	generate_command(t_parser *parser)
{
	t_cmd	*cmd;

	cmd = parser->cmd_list;
	if (is_directory(*cmd->data) == 1)
		return (cmd_error(*cmd->data, ERROR_DIR, EXIT_DIR));
	if (cmd->data[0] == NULL)
		parser->command = NULL;
	else
		parser->command = command_check(parser->cmd_dirs, *cmd->data);
	if (cmd->data[0] \
		&& access(*cmd->data, F_OK) == 0 && access(*cmd->data, X_OK) == -1)
		return (cmd_error(*cmd->data, ERROR_PERMISSION, EXIT_DIR));
	else if (parser->cmd_dirs == NULL && access(*cmd->data, X_OK) == 0)
		parser->command = *cmd->data;
	else if (parser->command == NULL)
		return (cmd_error(*cmd->data, ERROR_CMD, EXIT_CMD));
	return (EXIT_SUCCESS);
}

int	execute_command(t_parser *parser)
{
	execute_redir(parser, parser->cmd_list->redir, 0);
	if (parser->cmd_list->data)
	{
		if (is_builtin(parser) == 0)
		{
			if (execute_builtin(parser) != 0)
				return (1);
		}
		else
		{
			parser->cmd_exit_code = generate_command(parser);
			if (parser->cmd_exit_code != 0)
				return (parser->cmd_exit_code);
			else
			{
				if (execve(parser->command, parser->cmd_list->data, \
					parser->envp) < 0)
					perror_exit(ERROR_EXECVE);
			}
		}
	}
	return (EXIT_SUCCESS);
}
