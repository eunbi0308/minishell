#include "../includes/minishell.h"
#include "../includes/error.h"

int	open_infile(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		file_err_msg(ERROR_INFILE);
	else if (fd == -1 && dup2(fd, STDIN_FILENO) == -1)
		file_err_msg(ERROR_PIPE_IN);
	else
		close(fd);
	return (EXIT_SUCCESS);
}

int	create_outfile(t_cmd *redir)
{
	int	fd;

	if (redir->redir_type = APPEND)
		fd = open(redir->file_name, O_CREAT | O_RDWR | O_APPEND, 0000644);
	else
		fd = open(redir->file_name, O_CREAT | O_RDWR | O_TRUNC, 0000644);
	if (fd == -1)
		file_err_msg(ERROR_OUTFILE);
	else if (fd == -1 && dup2(fd, STDOUT_FILENO) == -1)
		file_err_msg(ERROR_PIPE_IN)
	else
		close(fd);
	return (EXIT_SUCCESS);
}

int	redirection(t_cmd *cmd)
{
	t_redir	*tmp;

	tmp = cmd->redir;
	while (cmd->redir != NULL)
	{
		if (cmd->redir->redir_type == GREATER || cmd->redir->redir_type == APPEND)
		{
			if (create_outfile(cmd->redir))
				return (EXIT_FAILURE);
		}
		else if (cmd->redir->redir_type == LESSER || cmd->redir->redir_type == HERE_DOC)
		{
			if (open_infile(cmd->redir->file_name))
				return (EXIT_FAILURE);
		}
		cmd->redir = cmd->redir->next;
	}
	cmd->redir = tmp;
	return (EXIT_SUCCESS);
}
