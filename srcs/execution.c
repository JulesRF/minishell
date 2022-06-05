/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 11:17:41 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/05 17:04:21 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int cmd_not_found(t_token *command)
{
	char *cmd_name;
	
	cmd_name = command->content; 
	printf("%s : command not found\n", cmd_name);
	return 127;
}

int handle_error(char *error_msg, int ret_value)
{
	ft_putendl_fd(error_msg, 1);
	return (ret_value);
}

/**
 * @brief Return the command and its arguments as an array of strings
 * 
 * @param command Linked list of command (path) and arguments
 * @return char** Array of strings of the command and its arguments,
 * NULL in case of error
 */
char **cmd_to_strs(t_token *command)
{
	char *args_str;
	char *tmp;
	char **args;
	
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
	args = ft_split(args_str, ' '); //probleme : supprime aussi les espaces volontaires ex: ./a.out "je    suis"
	free(args_str);
	return (args);
}

/**
 * @brief Execute the executable file named after command in a child process.
 * The parent process waits for the child to die
 * 
 * @param command Linked list of command (path) and arguments
 * @param env Environment variables
 * @return int -1 if execution fails
 */
int exec_cmd(t_token *command, char **env)
{
	pid_t	pid;
	int status;
	char **args;
	
	args = cmd_to_strs(command);
	if (!args)
		return handle_error("cmd_to_str failed", -1);
	pid = fork();
	if (pid == -1) //fork failed
		return handle_error("fork failed", -1);
	else if (pid == 0) //child process
	{
		execve(args[0], args, env); //check fail ?
		ft_putendl_fd("execve failed", 1);
	}
	else //parent process
	{
		wait(&status); //recup valeur retour ?
		//free ?
	}
	return (0); //?
}


/**
 * @brief Check if the command name is a builtin function
 * 
 * @param command Linked list of command (name) and arguments
 * @return int -1 if the search or execution fails
 */
int check_builtin(t_token *command)
{
	char *cmd_name;
	
	cmd_name = command->content; 
	if (ft_strcmp(cmd_name, "echo") == 0)
		return echo(command);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return cd(command);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return pwd();
	if (ft_strcmp(cmd_name, "export") == 0)
		return 1;
	if (ft_strcmp(cmd_name, "unset") == 0)
		return 1;
	if (ft_strcmp(cmd_name, "env") == 0)
		return 1;
	if (ft_strcmp(cmd_name, "exit") == 0)
		return 1;
	return -1;
}

/**
 * @brief Free an array of strings
 *
 * @param strs Array of strings that must end with NULL pointer
 */
void free_strs(char **strs)
{
	int i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

/**
 * @brief Check if an executable whose name is that of command exists in
 * one the PATH directories. If an executable is found, it is executed
 * @param command Linked list of command (name) and arguments
 * @param env Environment variables
 * @return int -1 if the search or execution fails
 */
int check_path(t_token *command, char **env)
{
	char *cmd_name;
	char *path;
	char *path_to_cmd;
	char **paths;
	struct stat sb;
	int i;
	
	cmd_name = ft_strjoin("/", command->content);
	if (!cmd_name)
		return (-1);
	(void)cmd_name;
	path = getenv("PATH"); //protect ?
	paths = ft_split(path, ':');
	if (!paths)
		return (-1); // and free path ?
	i = 0;
	while (paths[i])
	{
		path_to_cmd = ft_strjoin(paths[i], cmd_name);
		if ((stat(path_to_cmd, &sb) == 0 && sb.st_mode & S_IXUSR)) //check if executable
		{
			// free(command->content);
			command->content = path_to_cmd; //ATTENTION AUX FREE/BIN ICI
			free_strs(paths);
			return exec_cmd(command, env);
		}
		i++;
	}
	free_strs(paths);
	return (-1);
}


/**
 * @brief Search the command executable and try execute it
 * 
 * @param command Linked list of command (path or name) and arguments
 * @param env Environment variables
 * @return int -1 if the search or execution fails
 */
int search_cmd(t_token *command, char **env)
{
	char *cmd_name;
	int ret;

	cmd_name = command->content; 
	if (ft_strchr(cmd_name, '/') == NULL) //check if cmd_name == NULL ?
	{
		ret = check_builtin(command);
		if (ret != -1)
			return ret;
		ret = check_path(command, env);
		if (ret != -1)
			return ret;
		return cmd_not_found(command); //apparait même quand il ne fautdrait pas
	}
	else
	{
		exec_cmd(command, env);
	}
	
	return -1;
}