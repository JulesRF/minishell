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
int	ft_closed_quotes(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (str[i] != '\'')
				return (printf("minishell: syntax error unclosed quotes\n"),
					1);
		}
		if (str[i] == '\"')
		{
			i++;
			while (str[i] && str[i] != '\"')
				i++;
			if (str[i] != '\"')
				return (printf("minishell: syntax error unclosed quotes\n"),
					1);
		}
		i++;
	}
	return (0);
}

int	ft_syntax(char *str, t_list **bin)
{
	(void)bin;
	if (ft_closed_quotes(str, 0))
		return (1);
	return (0);
}

void	ft_prompt(t_token **token, t_list **bin, t_data *data, char *tester_cmd)
{
	t_vars vars;
	
	change_signals(2);

	rl_outstream = stderr; //RETIRER ?

	data->cmd_line = readline("\033[95mminishell$\033[0m ");
	(void)tester_cmd;
	// str = tester_cmd;
	
	while (data->cmd_line != NULL && ft_strcmp(data->cmd_line, "exit"))
	{
		if (data->cmd_line[0] != '\0')
			add_history(data->cmd_line);   // gere l'historique des commandes, sauf si la commande est un \n
		if (!ft_syntax(data->cmd_line, bin))
		{
			ft_token(token, bin, data); //parsing pur et dur (division des elements en tokens)
			// printf("salut a tous\n");
			// ft_print(*token);
			if (!ft_simplify(token, bin, data)) //simplification des tokens
			{
				// ft_print(*token);			// print simplement la liste de token pour voir le resultat du parsing
				vars.cmd = token;
				vars.env = data->env;
				vars.bin = bin;
				vars.cmd_line = data->cmd_line;
				g_exit_status = redir_and_exec(&vars);
				// exit(g_exit_status);
			}
		}
		ft_garbage(bin);
		ft_clean_token(token);
		free (data->cmd_line);
		data->cmd_line = readline("\033[95mminishell$\033[0m ");
	}
	free (data->cmd_line);
	rl_clear_history();
	if (data->cmd_line == NULL)
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
