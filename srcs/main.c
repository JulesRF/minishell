/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:10:11 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/05/04 16:30:20 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# include <readline/readline.h>
# include <readline/history.h>

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

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
		printf("no need to put arguments, simply type <./minishell>\n");
		exit(1);
	}
	return ;
}

int	main(int argc, char **argv, char **env)
{
	char	*str;

	ft_preparse(argc, argv, env);
	str = readline("\033[95mminishell$\033[33m ");
	while (ft_strcmp(str, "exit") && str != NULL)
	{
		printf("%s\n", str);
		add_history(str);
		free (str);
		str = readline("\033[95mminishell$\033[33m ");
	}
	free (str);
	exit(0);
}