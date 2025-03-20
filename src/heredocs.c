/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:31 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/18 13:43:30 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

int	my_mkstemp(char *template)
{
	int		fd;
	int		i;
	char	*chars;

	if (!template || ft_strlen(template) < 6
		|| ft_strcmp(&template[ft_strlen(template) - 6], "XXXXXX"))
	{
		errno = EINVAL;
		return (-1);
	}
	chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	while (1)
	{
		i = -1;
		while (++i < 6)
			template[ft_strlen(template) - 6 + i] = chars[rand() % 62];
		fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd != -1 || errno != EEXIST)
			return (fd);
	}
}

static void	heredoc_prompt(char *eof, int temp_fd)
{
	char	*line;

	while (1)
	{
		line = readline("heredoc> "); // Prompt the user
		if (!line)                    // Handle Ctrl+D (EOF)
		{
			write(2, "warning: heredoc delimited by EOF\n", 34);
			break ;
		}
		if (ft_strcmp(line, eof) == 0) // Stop if the delimiter is entered
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line)); // Write input to the temp file
		write(temp_fd, "\n", 1);               // Add a newline
		free(line);
	}
}

static char	*create_temp_file(void)
{
	char	*template;
	int		temp_fd;

	template = malloc(ft_strlen(HERE_TEMP) + 1);
	if (!template)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(template, HERE_TEMP, ft_strlen(HERE_TEMP) + 1);
	temp_fd = my_mkstemp(template); // Create a unique temporary file
	if (temp_fd == -1)
	{
		perror("mkstemp");
		free(template);
		return (NULL);
	}
	close(temp_fd); // Close the file descriptor (it will be reopened later)
	return (template);
}

void	handle_heredoc(t_cmd *cmd, char *eof)
{
	char	*temp_file;
	int		temp_fd;

	temp_file = create_temp_file();
	if (!temp_file)
		return ;
	temp_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd == -1)
	{
		perror("open");
		free(temp_file);
		return ;
	}
	heredoc_prompt(eof, temp_fd);
	close(temp_fd);
	cmd->in_file = temp_file;
}

void	redirect_input(t_cmd *cmd)
{
	int	fd;

	if (cmd->in_file)
	{
		fd = open(cmd->in_file, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return ;
		}
		dup2(fd, STDIN_FILENO); // Redirect stdin to the temporary file
		close(fd);
	}
}

void	cleanup_heredoc(t_cmd *cmd)
{
	if (cmd->in_file)
	{
		unlink(cmd->in_file); // Delete the temporary file
		free(cmd->in_file);
		cmd->in_file = NULL;
	}
}
