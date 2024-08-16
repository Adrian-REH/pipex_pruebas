/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   duplicate_processes_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmendez- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 16:07:07 by pmendez-          #+#    #+#             */
/*   Updated: 2024/08/05 16:07:08 by pmendez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void first_iteration(t_pipex *pipex, char *argv[])
{

	pipex->fd[0] = open(argv[1], O_RDONLY);

	if (pipex->fd[0] == -1)
		print_error_no_cmd(RED "Error\n" END "Error opening files\n", 1);

	if (access(argv[1], R_OK) == -1)
		print_error_no_cmd(RED "Error\n" END "No read permissions for input file\n", 1);

	if (dup2(pipex->fd[0], STDIN_FILENO) == -1)
		print_error_no_cmd(RED "Error\n" END "dup2 failed in STDIN_FILENO\n", 1);

	close_fd(&pipex->fd[0], "pipex->fd[0]");

	if (dup2(pipex->pipe_father[1], STDOUT_FILENO) == -1)
		print_error_no_cmd(RED "Error\n" END "dup2 failed in STDOUT_FILENO\n", 1);

	close_fd(&pipex->pipe_father[1], "pipex->pipe_father[1]");

	close_fd(&pipex->pipe_father[0], "pipex->pipe_father[0]");

	execute(pipex, argv[2]);
}

static void last_iteration(t_pipex *pipex, char *argv[], int argc)
{
	pipex->fd[1] = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	printf("fd[1] %d | %s \n", pipex->fd[1], argv[argc - 1]);
	if (pipex->fd[1] == -1)
		print_error_no_cmd(RED "Error\n" END "Error opening files\n", 1);

	if (access(argv[argc - 1], W_OK) == -1)
		ft_putstr_fd(RED "Error\n" END "No write permissions for output file\n", 1);

	if (dup2(pipex->fd[1], STDOUT_FILENO) == -1)
		print_error_no_cmd(RED "Error\n" END "dup2 failed in STDOUT_FILENO\n", 1);

	close_fd(&pipex->fd[1], "pipex->fd[1]");

	if (dup2(pipex->pipe_father[0], STDIN_FILENO) == -1)
		print_error_no_cmd(RED "Error\n" END "dup2 failed in STDIN_FILENO\n", 1);

	close_fd(&pipex->pipe_father[0], "pipex->pipe_father[0]");
	close_fd(&pipex->pipe_father[1], "pipex->pipe_father[1]");

	execute(pipex, argv[argc - 2]);
}

void process_duplicates(t_pipex *pipex, char *argv[], int argc)
{
	// cmd = argv[2 + pipex->index]

	if (pipex->index == 0)
		first_iteration(pipex, argv);
	else if (pipex->index == pipex->num_cmds - 1)
	{
		last_iteration(pipex, argv, argc);
	}
}

void command(t_pipex *pipex, char **argv, int argc)
{

	if (pipe(pipex->pipe_father) == -1)
	{
		print_error_no_cmd(RED "Error\n" END "Error creating pipe\n", 1);
		exit(1);
	}
	while (pipex->index < pipex->num_cmds)
	{
		pipex->pid = fork();

		if (pipex->pid == -1)
			print_error_no_cmd(RED "Error\n" END "Error creating child process\n", 1);
		else if (pipex->pid == 0)
		{
			process_duplicates(pipex, argv, argc);
			// exit(0);
		}
		else
		{
			// printf("Entra aqui, INDEX %d\n", pipex->index);
			waitpid(pipex->pid, NULL, 0);
			pipex->index++;
		}
	}
}
