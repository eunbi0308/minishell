#include "../includes/executor.h"
#include "../includes/parser.h"
#include "../includes/error.h"
#include "../includes/minishell.h"

void	create_pipes(int pipe_num, int fds[])
{
	int	i;

	i = 0;
	while (i < pipe_num)
	{
		if (pipe(fds + (i * 2)) == -1)
		{
			perror("pipe error");
			printf("%d", errno);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	close_ends(int pipe_num, int fds[])
{
	int	i;
	i = 0;
	while (i < pipe_num * 2)
	{
		close(fds[i]);
		i++;
	}
}

int	wait_pipes(pid_t *pid, int pipe_num)
{
	int	i;
	int	n;
	int	status;

	i = 0;
	n = 0;
	while (i < pipe_num + 1)
	{
		waitpid(pid[n], &status, 0);
		if (WIFEXITED(status) == 0)
		{
			g_global.exit_stat = WEXITSTATUS(status);
			return (0);
		}
		i++;
		n++;
	}
	return (1);
}

int	executor(t_parser_utils *cmd, t_lexer_utils *lexer, char **envp)
{
	int		fds[lexer->pipe_num * 2];
	int		pipe_num;
	pid_t	*pid;
	int		i;
	int		n;

	i = 0;
	n = 0;
	pipe_num = lexer->pipe_num;
	pid = (pid_t *)malloc(sizeof(pid_t) * (pipe_num + 1));
    if (pid == NULL)
	{
        perror("malloc error");
        exit(EXIT_FAILURE);
    }
	create_pipes(pipe_num, fds);
	while (cmd->cmd_list != NULL)
	{
		pid[n] = fork();
		if (pid[n] == -1)
			err_msg(ERROR_CHILD);
		else if (pid[n] == 0)
		{
			if (cmd->cmd_list->redir != NULL && cmd->cmd_list->redir->redir_type == HERE_DOC)
				here_document(cmd->cmd_list, lexer);
			if (cmd->cmd_list->redir != NULL)
				redirection(cmd->cmd_list);
			if (cmd->cmd_list->next)
			{
				if (dup2(fds[i + 1], 1) == -1)
					perror_exit(ERROR_DUP2_OUT);
			}
			if (i > 0)
			{
				if (dup2(fds[i - 2], 0) == -1)
					perror_exit(ERROR_DUP2_IN);
			}
			close_ends(pipe_num, fds);
			cmd->command = command_check(cmd->cmd_dirs, *cmd->cmd_list->data);
			if (execve(cmd->command, cmd->cmd_list->data, envp) < 0)
			{
				perror("execve error");
				exit(1);
			}
		}
		cmd->cmd_list = cmd->cmd_list->next;
		i += 2;
		n++;
	}
	close_ends(pipe_num, fds);
	return (wait_pipes(pid, pipe_num));
}
