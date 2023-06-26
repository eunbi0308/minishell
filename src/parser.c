#include "../includes/parser.h"

// t_cmd	generate_simple_cmd(t_lexer_utils *lexer)
// {
// 	t_cmd		*cmd;
// 	t_tokens	*current;

// 	current = lexer->token_list;
// 	cmd = (t_node *)malloc(sizeof(t_node));
// 	cmd->str = ft_strdup("");
// 	while (current != NULL)
// 	{
// 		while (current->token == -1)
// 		{
// 			args->str = ft_strjoin(args->str, current->data);
// 			args->str = ft_strjoin(args->str, " ");
// 		}
// 		ft_strtrim(args->str, " ");
// 		current = current->next;
// 		free(current);
// 		args = args->next;
// 	}
// 	return (*args);
// }

// void	init_parser_utils(t_lexer_utils *lexer, t_parser_utils *parser)
// {
// 	t_redir_utils	redir;

// 	redir.lexer_lst = lexer->token_list;
// 	redir.redir = NULL;
// 	redir.num_redir = 0;
// 	redir.utils = parser;
// 	return (redir);
// }


t_cmd	*create_cmd_node(void)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	new->redir = (t_redir *)malloc(sizeof(t_redir));
	new->data = NULL;
	new->redir->file_name = NULL;
	new->redir->redir_type = DEFAULT;
	new->redir->next = NULL;
	new->prev = NULL;
	new->next = NULL;
	return (new); 
}

void	generate_cmd(t_lexer_utils *lexer)
{
	t_cmd		cmd;
	int			arg_num;
	t_tokens	*current;
	t_tokens	*tmp;
	int			i;

	current = lexer->token_list;
	tmp = current;
	create_cmd_node();
	printf("parser\n");
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->token == PIPE)
			break ;
		arg_num++;
		tmp = tmp->next;
	}
	cmd.data = malloc(arg_num * sizeof(char *));
	i = 0;
	while (i < arg_num)
	{
		cmd.data[i] = malloc((ft_strlen(current->data) + 1) * sizeof(char));
		ft_strlcpy(cmd.data[i], current->data, ft_strlen(current->data));
		if (current->token != DEFAULT)
		{
			cmd.redir->redir_type = current->token;
			if (current->next)
				cmd.redir->file_name = ft_strdup(current->next->data);
			cmd.redir = cmd.redir->next;
		}
		i++;
		current = current->next;
	}
}

int	main(void)
{
	t_lexer_utils lexer;
	char	*str;

	// str = "    grep 'Hello World'";
	str = "    grep 'Hello World' | cat -e    ";
	lexer.arg = ft_strtrim(str, " ");
	if (match_quotes(lexer.arg) == FALSE)
		return (-1);
	lexer.pipe_num = 0;
	lexical_analyzer(&lexer);
	//Print lexer
	// printf("\n");
	// printf("INPUT : %s\n\n", str);
	// printf("NUMBER OF PIPES : %i\n", lexer.pipe_num);

	// t_tokens *current = lexer.token_list;
	// int i = 1;
	// while (current != NULL)
	// {
	// 	printf("%d. current->data: %s\n", i, current->data);
	// 	printf("%d. current->token: %d\n", i, current->token);
	// 	current = current->next;
	// 	i++;
	// 	printf("\n");
	// }

	generate_cmd(&lexer);
	
	return (0);
}
