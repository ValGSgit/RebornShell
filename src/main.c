/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:35:07 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/20 13:15:05 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

int		g_signal_received = 0;

void	print_parsed_command(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		printf("Command: %s\n", current->args[0]);
		if (current->args)
		{
			printf("Arguments:\n");
			for (int i = 1; current->args[i]; i++)
				printf("  - %s\n", current->args[i]);
		}
		current = current->next;
	}
}

void	minishell_loop(t_shell *shell)
{
	char	*input;
	char	**tokens;
	t_cmd	*cmd;
	char	*prompt;

	prompt = "Minishell-> ";
	cmd = NULL;
	while (1)
	{
		input = readline(prompt);
		if (!input)
			break ;
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		if (*input)
			add_history(input);
		tokens = lexer(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		cmd = parser(tokens);
		if (!cmd)
		{
			free(input);
			free_tokens(tokens);
			continue ;
		}
		expander(cmd, shell);
		execute_command(cmd, shell);
		free_cmd(cmd);
		free_tokens(tokens);
		free(input);
	}
	rl_clear_history();
}

char	**copy_env(char **envp)
{
	int		i;
	char	**env;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	if (*envp)
		shell.env = copy_env(envp);
	if (!shell.env)
		return (write(2, "environment copy failed\n", 25), EXIT_FAILURE);
	shell.exit_status = 0;
	shell.is_interactive = isatty(STDIN_FILENO);
	shell.signal_status = 0;
	setup_signals();
	minishell_loop(&shell);
	rl_clear_history();
	free_env(shell.env);
	return (shell.exit_status);
}
