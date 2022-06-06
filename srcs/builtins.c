/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 11:17:10 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/06 12:05:08 by vfiszbin         ###   ########.fr       */
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
int check_n_option(char *s, int *n_option)
{
	int i;
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
int echo(t_token *command)
{
	int n_option;

	n_option = 0;
	if (command != NULL) //necessaire ?
		command = command->next;
	while (command && (command->type == 4 || check_n_option(command->content, &n_option))) //check for -n options
		command = command->next;
	while (command && (command->type == 2 || command->type == 4)) //space ou word
	{
		ft_putstr_fd(command->content, 1);
		command = command->next;
	}
	if (n_option == 0)
		ft_putchar_fd('\n', 1);
	return (0);
}

/**
 * @brief Print the name of the current/working directory
 * 
 * @return int 1 in case of failure, 0 otherwise
 */
int pwd()
{
	char buffer[BUFFER_SIZE];
	char *ret;
	
	ret = getcwd(buffer, BUFFER_SIZE);
	if (ret == NULL)
		return handle_error("getcwd failed", 1);
	ft_putendl_fd(ret, 1);
	return (0);
}

char *cmd_to_str(t_token *command)
{
	char *args_str;
	char *tmp;
	
	args_str = malloc(1);
	if (!args_str)
		return (NULL);
	args_str[0] = '\0';
	while (command && (command->type == 2 || command->type == 4)) //space ou word
	{
		tmp = args_str;
		args_str = ft_strjoin(args_str, command->content);
		free(tmp);
		if (!args_str)
			return (NULL);
		command = command->next;
	}
	return (args_str);
}


/**
 * @brief 
 * 
 * @param command 
 * @return int 0 on success, 1 on error
 */
int cd(t_token *command)
{
	//too many arguments
	int ret;
	
	if (command != NULL) //necessaire ?
		command = command->next;
	while (command && (command->type == 4)) //jump all spaces
		command = command->next;

	if (command == NULL)
		return handle_error("cd: no argument", 1);
	else if (command->next != NULL && command->next->next != NULL)
		return handle_error("cd: too many arguments", 1);
	
	ret = chdir(command->content);
	if (ret == -1)
	{
		perror(command->content);
		return 1;
	}
	return 0;
}