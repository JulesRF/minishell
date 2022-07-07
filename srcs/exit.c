/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 11:17:10 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/07 19:14:10 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isnum(char *s)
{
	int	i;

	if (s == NULL)
		return (0);
	i = 0;
	if (s[0] == '-')
		i++;
	while (s[i])
	{
		if (ft_isdigit(s[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

void	clean_prog(t_data *data, t_redir *redir)
{
	free (data->cmd_line);
	rl_clear_history();
	free_strs_array(*(data->env));
	ft_garbage(data->bin);
	if (redir && redir->cmd_table)
		free(redir->cmd_table);
}

void	exit_prog(char *msg, int exit_status, t_redir *redir)
{
	if (msg != NULL)
		ft_putendl_fd(msg, 2);
	close(0);
	close(1);
	if (redir)
	{
		close(redir->tmpin);
		close(redir->tmpout);
	}
	exit(exit_status);
}

int	exit_too_many_args(t_token *command, t_data *data, t_redir *redir)
{
	if (ft_isnum(command->content) == 1)
	{
		ft_putendl_fd("exit\nminishell: exit: too many arguments", 2);
		return (1);
	}
	clean_prog(data, redir);
	exit_prog("exit\nminishell: exit: too many arguments", 2, redir);
	return (0);
}

/**
 * @brief Exit the program with given exit status
 * 
 * @param command Linked list of command (name) and arguments
 * @param env environment variables
 * @param bin linked list of allocated memory to free
 * @param cmd_line string from readline
 * @return int 1 if error, 0 otherwise (but should not return)
 */
int	exit_builtin(t_token *command, t_data *data, t_redir *redir)
{
	int	exit_status;

	if (command != NULL)
		command = command->next;
	if (command == NULL)
	{
		clean_prog(data, redir);
		exit_prog("exit", 0, redir);
	}
	if (command != NULL && command->next != NULL)
	{
		if (exit_too_many_args(command, data, redir) == 1)
			return (1);
	}
	if (ft_isnum(command->content) == 0)
	{
		clean_prog(data, redir);
		exit_prog("exit\nminishell: exit: numeric argument required", 2, redir);
	}
	exit_status = ft_atoi(command->content);
	clean_prog(data, redir);
	exit_prog("exit", exit_status, redir);
	return (0);
}
