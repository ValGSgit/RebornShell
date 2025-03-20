/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:56:16 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/20 13:41:52 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/planer.h"

static char	*remove_quotes(char *start, char *end)
{
	char	*result;
	char	*dst;
	char	quote_char;
	bool	in_quote;

	result = ft_calloc(end - start + 1, sizeof(char));
	if (!result)
		return (NULL);
	dst = result;
	in_quote = false;
	while (start < end)
	{
		if (!in_quote && (*start == '\'' || *start == '\"'))
		{
			in_quote = true;
			quote_char = *start++;
		}
		else if (in_quote && *start == quote_char && start++)
			in_quote = false;
		else
			*dst++ = *start++;
	}
	*dst = '\0';
	return (result);
}

int	add_token(char **tokens, int *count, char *start, char *end)
{
	char	*cleaned_token;

	if (*count >= MAX_TOKENS - 1)
		return (-1);
	cleaned_token = remove_quotes(start, end);
	if (!cleaned_token)
		return (-1);
	tokens[*count] = cleaned_token;
	(*count)++;
	return (0);
}

static int	process_special_char(char **tokens, int *count, char **input)
{
	if (is_special_char(**input))
	{
		if (add_token(tokens, count, *input, *input + 1) < 0)
			return (-1);
		(*input)++;
		return (1);
	}
	return (0);
}

static int	handle_quotes(char **input, bool *in_quote, char *quote_char)
{
	if (!*in_quote && (**input == '\'' || **input == '\"'))
	{
		*in_quote = true;
		*quote_char = **input;
		(*input)++;
		return (1);
	}
	else if (*in_quote && **input == *quote_char)
	{
		*in_quote = false;
		*quote_char = '\0';
		(*input)++;
		return (1);
	}
	return (0);
}

char	**lexer(char *input)
{
	char **tokens;
	t_lexer lx;

	lx = (t_lexer){0};
	tokens = ft_calloc(MAX_TOKENS, sizeof(char *));
	if (!tokens || !input)
		return (free(tokens), NULL);
	lx.start = input;
	while (*input && lx.tokcount < MAX_TOKENS - 1)
	{
		if (handle_quotes(&input, &lx.in_quote, &lx.quote_char))
			continue ;
		if (!lx.in_quote && (ft_isspace(*input) || is_special_char(*input)))
		{
			if (lx.start != input && add_token(tokens, &lx.tokcount, lx.start,
					input) < 0)
				return (free_tokens(tokens), NULL);
			if (process_special_char(tokens, &lx.tokcount, &input) < 0)
				return (free_tokens(tokens), NULL);
			lx.start = input + 1;
		}
		input++;
	}
	return (end_check(lx, input, tokens));
}
