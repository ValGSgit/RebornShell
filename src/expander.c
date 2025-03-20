/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:57:26 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/20 14:15:39 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

void	ft_free(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*get_env_value(char *name, char **env)
{
	int		i;
	char	*key;

	i = 0;
	if (!env || !name)
		return (NULL);
	while (env[i])
	{
		key = ft_substr(env[i], 0, (char *)ft_strchr(env[i], '=') - env[i]);
		if (ft_strncmp(key, name, ft_strlen(key)) == 0)
		{
			free(key);
			return (ft_strchr(env[i], '=') + 1);
		}
		free(key);
		i++;
	}
	return (NULL);
}

char	*resolve_path(char *cmd, char **env)
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**paths;

	i = 0;
	paths = ft_split(get_env_value("PATH", env), ':');
	if (!paths)
		return (cmd);
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free(paths);
	return (cmd);
}

static char	*expand_variable(char *arg, t_shell *shell)
{
    char	*value;

    if (ft_strcmp(arg, "$?") == 0)
        return (ft_itoa(shell->exit_status)); // Last exit status
    if (ft_strcmp(arg, "$$") == 0)
        return (ft_itoa(getpid())); // Current process ID
    if (ft_strcmp(arg, "$-") == 0)
        return (ft_strdup("himBHs"));
    if (arg[1] == '\0') // Handle "$" with no variable name
        return (ft_strdup("$"));
    value = get_env_value(arg + 1, shell->env); // Lookup environment variable
    if (value)
        return (ft_strdup(value));
    return (ft_strdup("")); // Undefined variables expand to an empty string
}

void	expander(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*value;

	i = 0;
	if (!cmd->args)
		return ;
	if (ft_strncmp(cmd->args[0], "./", 2) != 0 && ft_strncmp(cmd->args[0],
			"../", 3) != 0)
		cmd->args[0] = resolve_path(cmd->args[0], shell->env);
	while (cmd->args[++i])
	{
		if (ft_strchr(cmd->args[i], '$'))
		{
			value = expand_variable(cmd->args[i], shell);
			if (value)
			{
				free(cmd->args[i]);
				cmd->args[i] = value;
			}
		}
	}
}
