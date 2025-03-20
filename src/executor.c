/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:34:42 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/20 13:21:59 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

// Removed my attempt at execution to improve lexing/parsing

void	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		handle_dup(cmd);
		//shell->exit_status = execve(cmd->args[0], cmd->args, shell->env);
		execve(cmd->args[0], cmd->args, shell->env);
		perror("execve");
		exit(1);
	}
	else
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else
			shell->signal_status = 1;
		restore_redirections(cmd);
	}
}

void	execute_pipeline(t_cmd *cmd, char **env)
{
	int		pipe_fd[2];
	int		prev_pipe_in;
	pid_t	pid;

	prev_pipe_in = 0;
	while (cmd)
	{
		if (cmd->next)
		{
			pipe(pipe_fd);
			cmd->out_fd = pipe_fd[1];
		}
		pid = fork();
		if (pid == 0)
		{
			if (prev_pipe_in != 0)
			{
				dup2(prev_pipe_in, 0);
				close(prev_pipe_in);
			}
			if (cmd->out_fd != 1)
			{
				dup2(cmd->out_fd, 1);
				close(cmd->out_fd);
			}
			handle_dup(cmd);
			execve(cmd->args[0], cmd->args, env);
			perror("execve");
			exit(1);
		}
		else
		{
			if (prev_pipe_in != 0)
				close(prev_pipe_in);
			if (cmd->out_fd != 1)
				close(cmd->out_fd);
			prev_pipe_in = pipe_fd[0];
			cmd = cmd->next;
		}
	}
	waitpid(pid, NULL, 0);
}

void	handle_dup(t_cmd *cmd)
{
	if (cmd->in_fd != 0)
		dup2(cmd->in_fd, 0);
	if (cmd->out_fd != 1)
		dup2(cmd->out_fd, 1);
}

void	restore_redirections(t_cmd *cmd)
{
	if (cmd->in_fd != 0)
		close(cmd->in_fd);
	if (cmd->out_fd != 1)
		close(cmd->out_fd);
}