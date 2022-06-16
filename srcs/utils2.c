/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 16:18:27 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/16 15:34:28 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free an array of strings
 *
 * @param strs Array of strings that must end with NULL pointer
 */
void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

/**
 * @brief Return the command and its arguments as an array of strings
 * 
 * @param command Linked list of command (path) and arguments
 * @return char** Array of strings of the command and its arguments,
 * NULL in case of error
 */
char	**cmd_to_strs(t_token *command)
{
	char	**args;
	t_token	*cur;
	int		i;

	cur = command;
	i = 0;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
		return (NULL);
	cur = command;
	i = 0;
	while (cur)
	{
		args[i] = cur->content;
		i++;
		cur = cur->next;
	}
	args[i] = NULL;
	return (args);
}

t_token	**split_commands(t_token *commands, int nb_cmd, int i)
{
	t_token	*tmp;
	t_token	**cmd_table;

	cmd_table = malloc(sizeof(t_token *) * (nb_cmd + 1));
	if (!cmd_table)
		return (NULL);
	i = -1;
	while (++i < nb_cmd)
		cmd_table[i] = NULL;
	i = 0;
	while (commands)
	{
		if (commands->type == 1 && (ft_strcmp(commands->content, "|") == 0))
			i++;
		if (commands->type == 2 || commands->type == 5)
		{
			tmp = commands;
			commands = commands->next;
			tmp->next = NULL;
			ft_lstadd_back_token(&cmd_table[i], tmp);
		}
		else
			commands = commands->next;
	}
	return (cmd_table);
}

int	get_nb_cmd_and_heredocs(t_token *commands, int *nb_heredocs)
{
	int	count;

	*nb_heredocs = 0;
	count = 1;
	while (commands)
	{
		if (commands->type == 1 && (ft_strcmp(commands->content, "|") == 0))
			count++;
		else if (commands->type == 5 && ft_strcmp(commands->content, "<<") == 0)
			*nb_heredocs = *nb_heredocs + 1;
		commands = commands->next;
	}
	return (count);
}
