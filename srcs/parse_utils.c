/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 14:51:13 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 14:51:13 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		i;

	i = 0;
	while (s1[i])
		i++;
	dest = malloc(sizeof(char) * i + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (1);
	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

void	ft_preparse(int argc, char **argv, char **env)
{
	(void)argv;
	(void)env;
	if (argc != 1)
	{
		printf("no need to put arguments, simply type: ./minishell\n");
		exit(1);
	}
	return ;
}

void	ft_print(t_token *token)
{
	int	i;

	printf("printing token list...\n");
	i = 1;
	while (token)
	{
		printf("---------------\n");
		printf("numero: %d\ntype: %d\ncontent:\e[38;5;196m->\033[0m%s\e[38;5;196m<-\033[0m\n", i, token->type, (char *)token->content);
		printf("---------------\n");
		i++;
		token = token->next;
	}
}
