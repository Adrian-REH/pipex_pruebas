
#include "../include/pipex_bonus.h"

void print_error(char *error_message, char *cmd)
{
	char *str;

	if (cmd == NULL)
		return;
	str = ft_strjoin(error_message, cmd);
	ft_putendl_fd(str, 2);
	free(str);
}

void print_error_no_cmd(char *error_message, int code)
{
	ft_putendl_fd(error_message, 2);
	exit(code);
}

char *search_path(t_pipex *pipex, char **cmd_splitted)
{
	char *line;
	char *path_slash;
	int i;

	i = 0;
	if (pipex->path == NULL)
	{
		ft_putendl_fd("Error: PATH not found", 2);
		return (NULL);
	}
	while (pipex->path[i] != NULL)
	{
		path_slash = ft_strjoin(pipex->path[i], "/");
		line = ft_strjoin(path_slash, cmd_splitted[0]);
		//line = ft_strjoin(line, "\0");
		free(path_slash);
		if (access(line, F_OK | X_OK) == 0)
			return (line);
		free(line);
		i++;
	}
	return (cmd_splitted[0]);
}

char **get_path(char **envp)
{
	int i;
	char **str;

	i = 0;
	str = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			str = ft_split(envp[i] + 5, ':');
			return (str);
		}
		i++;
	}
	return (NULL);
}

void close_fd(int *fd, char *name)
{
	char *str;

	if (close(*fd) == -1)
	{
		str = ft_strjoin(RED "Error\n" END "Error closing ", name);
		printf("%s\n", str);
		ft_putendl_fd(str, 2);
		free(str);
		*fd = -1;
	}
}