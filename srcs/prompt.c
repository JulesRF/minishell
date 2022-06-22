/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 15:07:19 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/06/22 15:07:19 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	ft_closed_quotes(char *str, t_list **bin)
{
	int	i;

	(void)bin;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] != '\'')
				return (printf("SYNTAX ERROR\n"), 1);
		}
		if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
			if (str[i] != '\"')
				return (printf("SYNTAX ERROR\n"), 1);
		}
		i++;
	}
	return (0);
}

int	ft_syntax(char *str, t_list **bin)
{
	if (ft_closed_quotes(str, bin))
		return (1);
	return (0);
}

void	ft_prompt(t_token **token, t_list **bin, char ***env, char *tester_cmd)
{
	char *str;
	t_vars vars;
	
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, handle_sigint);

	rl_outstream = stderr; //RETIRER

	str = readline("\033[95mminishell$\033[0m ");

	(void)tester_cmd;
	// str = tester_cmd;
	
	while (str != NULL && ft_strcmp(str, "exit"))
	{
		if (str[0] != '\0')
			add_history(str);   // gere l'historique des commandes, sauf si la commande est un \n
		if (!ft_syntax(str, bin))
		{
			ft_token(token, bin, str); //parsing pur et dur (division des elements en tokens)
			// ft_print(*token);
			if (!ft_simplify(token, bin, *env)) //simplification des tokens
			{
				// ft_print(*token);			// print simplement la liste de token pour voir le resultat du parsing
				// envoie des infos a mon mate
				
				vars.cmd = token;
				vars.env = env;
				vars.bin = bin;
				vars.cmd_line = str;
				g_exit_status = redir_and_exec(&vars);
				// exit(g_exit_status);
			}
		}
		ft_garbage(bin);
		ft_clean_token(token);
		free (str);
		str = readline("\033[95mminishell$\033[0m ");
	}
	free (str);
	rl_clear_history();
	if (str == NULL)
		write(1, "\n", 1);
}

/**
 * @brief Duplicate the environment variables into an allocated array of strings
 * 
 * @param envp Environment variables
 * @return char** NULL if malloc fails, the duplicated array of strings otherwise
 */
char **dup_env(char **envp)
{
	int		i;
	int		j;
	char	**env;

	i = 0;
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
			j = 0;
			while (j < i)
			{
				free(env[j]);
				j++;
			}
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

/**
 * @brief Free an array of strings
 * 
 * @param strs Allocated array of strings
 */
void free_strs_array(char **strs)
{
	int i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs); //est ce qu'on ne devrait pas avoir char ***strs pour ici ?
}
