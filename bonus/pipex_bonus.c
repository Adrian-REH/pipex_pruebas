
#include <stdio.h>
#include <unistd.h>

#include "../include/pipex_bonus.h"

void close_all_fds(t_pipex *pipex)
{
	printf("Entra en close_all_fds --\n");
	// Comprobar si el pipe del padre esta cerrado
	if (pipex->pipe_father[0] != -1)
		close_fd(&pipex->pipe_father[0], "pipex->pipe_father[0]");

	// Comprobar si el pipe del padre esta cerrado
	if (pipex->pipe_father[1] != -1)
		close_fd(&pipex->pipe_father[1], "pipex->pipe_father[1]");
}

int main(int argc, char *argv[], char **envp)
{
	t_pipex pipex;
	int status;

	status = 0;
	initialize_pipex(&pipex, envp, argc);
	command(&pipex, argv, argc);
	close_all_fds(&pipex);
	while (pipex.index > 0)
	{
		waitpid(-1, NULL, 0);
		pipex.index--;
	}
	if (WEXITSTATUS(status) == 127)
		exit(127);
	return (0);
}