#include <unistd.h>
#include "../includes/error.h"

void	err_msg(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
	return(EXIT_FAILURE);
}
