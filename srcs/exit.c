/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 11:17:10 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/16 13:30:43 by vfiszbin         ###   ########.fr       */
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

void	clean_prog( char ***env, t_list **bin, char *cmd_line)
{
	free (cmd_line);
	rl_clear_history();
	free_strs_array(*env);
	ft_garbage(bin);
}

void	exit_prog(char *msg, int exit_status)
{
	if (msg != NULL)
		ft_putendl_fd(msg, 2);
	exit(exit_status);
}

int	exit_too_many_args(t_token *command, char ***env, t_list **bin,
	char *cmd_line)
{
	if (ft_isnum(command->content) == 1)
	{
		ft_putendl_fd("exit\nminishell: exit: too many arguments", 2);
		return (1);
	}
	clean_prog(env, bin, cmd_line);
	exit_prog("exit\nminishell: exit: too many arguments", 2);
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
int	exit_builtin(t_token *command, char ***env, t_list **bin, char *cmd_line)
{
	int	exit_status;

	if (command != NULL)
		command = command->next;
	if (command == NULL)
	{
		clean_prog(env, bin, cmd_line);
		exit_prog("exit", 0);
	}
	if (command != NULL && command->next != NULL)
	{
		if (exit_too_many_args(command, env, bin, cmd_line) == 1)
			return (1);
	}
	if (ft_isnum(command->content) == 0)
	{
		clean_prog(env, bin, cmd_line);
		exit_prog("exit\nminishell: exit: numeric argument required", 2);
	}
	exit_status = ft_atoi(command->content);
	clean_prog(env, bin, cmd_line);
	exit_prog("exit", exit_status);
	return (0);
}
