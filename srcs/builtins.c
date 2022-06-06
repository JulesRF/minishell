/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 11:17:10 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/06 18:46:07 by vfiszbin         ###   ########.fr       */
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


/**
 * @brief Check if the env var name is valid
 * env var name format : [a-zA-Z_]+[a-zA-Z0-9_]*
 * Also calculate the length of said var name
 * @param s arg string
 * @return int 1 if valid, 0 if not
 */
int var_name_is_valid(char *s, int *name_len)
{
	*name_len = 0;
	if (s && (ft_isalpha(s[0]) == 0 && s[0] != '_'))
	{
		ft_putstr_fd("export: ", 2);
		ft_putstr_fd(s, 2);
		ft_putendl_fd(": not a valid identifier1", 2);
		return 0;
	}
	*name_len = 1;
	while (s && s[*name_len] && s[*name_len] != '=')
	{
		if (ft_isalnum(s[*name_len]) == 0 && s[*name_len] != '_')
		{
			ft_putstr_fd("export: ", 2);
			ft_putstr_fd(s, 2);
			ft_putendl_fd(": not a valid identifier2", 2);
			return 0;
		}
		*name_len = *name_len + 1;
	}
	return 1;
}


/**
 * @brief Get the var name in the arg string
 * 
 * @param s arg string
 * @param name_len 
 * @return char* NULL if malloc fails, the malloc'ed var name string otherwise
 */
char *get_var_name(char *s, int name_len)
{
	char *var_name;

	var_name = malloc(sizeof(char) * (name_len + 1));
	if (!var_name)
		return NULL;
	ft_strlcpy(var_name, s, name_len); //size correcte ?
	return var_name;
}


void set_var_in_env(char *s, char *var_name, int name_len, char ***env)
{
	int i;
	int j;
	char **envv;

	envv = *env;
	i = 0;
	while (env && env[i])
	{
		j = 0;
		while (envv[i][j] && envv[i][j] != '=' && envv[i][j] == var_name[j])
			j++;
		if (j == name_len && envv[i][j] == '=') //we found the matching key in env
		{
			free(envv[i]);
			envv[i] = ft_strdup(s); //ATTENTION fuites memoires possibles ici !
			return ;
		}
		i++;
	}
}

int add_var_to_env(char *s, char ***env)
{
	int		i;
	int		j;
	char	**new_env;
	char	**tmp;

	i = 0;
	tmp = *env;
	while (tmp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (tmp[i])
	{
		new_env[i] = ft_strdup(tmp[i]);
		if (!new_env[i])
		{
			j = 0;
			while (j < i)
			{
				free(new_env[j]);
				j++;
			}
			free(new_env);
			return (1);
		}
		i++;
	}
	new_env[i] = ft_strdup(s);
	if (!new_env[i])
	{
		new_env[i] = NULL;
		free_strs_array(new_env);
	}
	i++;
	new_env[i] = NULL;
	*env = new_env;
	free_strs_array(tmp);
	return (0);
}

/**
 * @brief 
 * 
 * @param command 
 * @return int 
 */
int export(t_token *command, char ***env)
{
	int ret;
	int name_len;
	char *var_name;

	if (command != NULL) //necessaire ?
		command = command->next;
	//export with no argument
	ret = 0;
	while (command)
	{
		if (command->type == 2)
		{
			if (var_name_is_valid(command->content, &name_len) == 1)
			{
				if (command->content[name_len] != '=')
					return 0;
				var_name = get_var_name(command->content, name_len);
				if (!var_name)
					return 1;
				if (getenv(var_name) == NULL)
				{
					
					add_var_to_env(command->content, env);
				}
				else
				{
					set_var_in_env(command->content,var_name, name_len, env);
				}
			}
			else
				ret = 1;
		}
		command = command->next;
	}
	return ret;
}