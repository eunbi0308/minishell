NAME		= minishell
CC			= gcc
ifdef DEBUG
CFLAGS		= -Wall -Wextra -Werror -fsanitize=address -g
READLINE	= -lreadline
else
CFLAGS		= -Wall -Wextra -Werror
READLINE	= -lreadline
endif
LIBFT		= libft
OBJ_DIR		= obj/
SRC_DIR		= src/

HEADER_DIR	= includes/
HEADER_SRC	= minishell.h lexer.h parser.h executor.h error.h
HEADERS		= $(addprefix $(HEADER_DIR), $(HEADER_SRC))

# READLINE_FLAGS  = -lreadline -L${HOME}/.brew/opt/readline/lib
# OBJ_FLAGS       = -I${HOME}/.brew/opt/readline/include

READLINE_FLAGS  = -lreadline -L/usr/local/opt/readline/lib
OBJ_FLAGS       = -I/usr/local/opt/readline/include

SRC_DIR		= src/
SRC_FILE	= main.c lexer.c node_utils.c parser.c executor.c heredoc.c error.c env.c executor_utils.c cmd_echo.c cmd_pwd.c cmd_exit.c redirection.c expand.c signals.c cmd_cd.c cmd_export.c cmd_unset.c

OBJ			=	$(addprefix $(OBJ_DIR), $(SRC_FILE:.c=.o))

OBJF =	.cache_exists

CYAN_B		=	\033[1;96m
CYAN		=	\033[0;96m

all: $(NAME)

$(NAME): $(OBJ) $(OBJF)
			@make -C $(LIBFT)
			@$(CC) $(CFLAGS) $(READLINE_FLAGS) $(OBJ) libft/libft.a -o $(NAME)
			@echo "$(CYAN_B)- Lexer is compiled -"

$(OBJ_DIR)%.o:$(SRC_DIR)%.c $(HEADER)| $(OBJF)
			@$(CC) $(CFLAGS) $(OBJ_FLAGS) -c $< -o $@

$(OBJF):
		@mkdir -p $(OBJ_DIR)
		@mkdir -p $(OBJ_DIR)$(SRC_DIR)
		@touch $(OBJF)

clean:
		@rm -rf $(OBJ_DIR)
		@make clean -C $(LIBFT)
		@rm -f $(OBJF)
		@echo "$(CYAN)- Object files are cleaned -"

fclean: clean
		@rm -f $(NAME)
		@make fclean -C $(LIBFT)
		@echo "$(CYAN)- Executable files are cleaned -"

re:	fclean all

.PHONY: all clean fclean re