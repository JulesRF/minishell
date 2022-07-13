/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 09:57:45 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/07/13 17:28:32 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the string is a -n option.
 * If yes, set the n_option bool to 1
 * @param s String to check
 * @param n_option Boolean: is one of the arguments a -n option ?
 * @return int 1 if true, 0 if false
 */
int	check_n_option(char *s, int *n_option)
{
	int	i;

	if (s[0] != '-')
		return (0);
	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	*n_option = 1;
	return (1);
}

/**
 * @brief Write command args to stdout.
 * -n option : do not output the trailing newline
 * @param command Linked list of command (name) and arguments
 * @return int 0 when done
 */
int	echo(t_token *command)
{
	int	n_option;

	n_option = 0;
	if (command != NULL)
		command = command->next;
	while (command && (command->type == 4
			|| check_n_option(command->content, &n_option)))
		command = command->next;
	signal(SIGPIPE, handle_sigpipe);
	while (command && (command->type == 2))
	{
		ft_putstr_fd(command->content, 1);
		if (command->next != NULL)
			write(1, " ", 1);
		command = command->next;
	}
	if (n_option == 0)
		ft_putchar_fd('\n', 1);
	signal(SIGPIPE, SIG_DFL);
	return (0);
}
