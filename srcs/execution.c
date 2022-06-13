/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 11:17:41 by vfiszbin          #+#    #+#             */
/*   Updated: 2022/06/13 10:08:55 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int g_exit_status;

/**
 * @brief Get the env value for key
 * 
 * @param key key to search
 * @param env environment variables
 * @return char* NULL if not found, the allocated value otherwise
 */
char *get_env_value(char *key, char **env)
{
	int i;
	int j;
	int key_len;
	char *value;

	key_len = ft_strlen(key);
	i = 0;
	while (env && env[i])
	{
		j = 0;
		while (env[i][j] && key[j] && env[i][j] == key[j]) //peut remplacer ca par ft_strncmp ?
			j++;

		if (j == key_len && (env[i][j] == '=' || env[i][j] == '\0')) //we found the matching key in env
		{
			value = ft_strdup(&env[i][j + 1]);
			if (!value)
			{
				ft_putendl_fd("Memory allocation error", 2);
				return (NULL);
				
			}
			return value;
		}
		i++;
	}
	return (NULL);
}

int cmd_not_found(t_token *command)
{
	char *cmd_name;
	
	cmd_name = command->content; 
	ft_putstr_fd(cmd_name, 2);
	ft_putendl_fd(": command not found", 2);
	return 127;
}

int handle_error(char *error_msg, int ret_value)
{
	ft_putendl_fd(error_msg, 2);
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
	char **args;
	t_token *cur;
	int i;
	
	cur = command;
	i = 0;
	while(cur)
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


int			find_error_status(char *path)
{
	DIR	*folder;
	int	f;
	int	ret;

	f = open(path, O_WRONLY);
	folder = opendir(path);
	if (f == -1 && folder == NULL)
		ret = 127;
	else
		ret = 126;
	if (folder)
		closedir(folder);
	close(f);
	return (ret);
}

/**
 * @brief Execute the executable file named after command in a child process.
 * The parent process waits for the child to die
 * 
 * @param command Linked list of command (path) and arguments
 * @param env Environment variables
 * @return int 1 if execution fails, 0 if no error
 */
int exec_cmd(t_token *command, char **env, pid_t pid)
{
	int status;
	int ret;
	char **args;

	ret = 0;
	args = cmd_to_strs(command);
	if (!args)
		return 1;
	if (!args)
		return handle_error("cmd_to_str failed", 1);

	
	if (pid == -1) //if there is no pipe
	{
		pid = fork();
		if (pid == -1) //fork failed
		{
			free(args);
			return handle_errno("fork failed", 1, NULL);
		}
		else if (pid == 0) //child process
		{
			execve(args[0], args, env);
			handle_errno(args[0], 1, NULL);
			ret = find_error_status(args[0]);
			exit(ret);
		}
		else //parent process
		{
			signal(SIGINT, handle_sigint_no_prompt);
			if (waitpid(pid, &status, 0) == -1)
				return handle_errno("waitpid", 1, NULL);
			if (WIFEXITED(status))
				ret = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
			{
				ret = WTERMSIG(status);
				ret += 128;
			}
			signal(SIGINT, handle_sigint);
		}
	}
	else
	{
		execve(args[0], args, env);
		handle_errno(args[0], 1, NULL);
		ret = find_error_status(args[0]);
		exit(ret);
	}

	free(args); // check leak
	return (ret);
}


/**
 * @brief Check if the command name is a builtin function
 * 
 * @param command Linked list of command (name) and arguments
 * @return int -1 if the search fails, 1 if execution fails, 0 if no error
 */
int check_builtin(t_token *command, char ***env, t_list **bin)
{
	char *cmd_name;
	
	cmd_name = command->content; 
	if (ft_strcmp(cmd_name, "echo") == 0)
		return echo(command);
	if (ft_strcmp(cmd_name, "cd") == 0)
		return cd(command, *env);
	if (ft_strcmp(cmd_name, "pwd") == 0)
		return pwd();
	if (ft_strcmp(cmd_name, "export") == 0)
		return (export(command, env));
	if (ft_strcmp(cmd_name, "unset") == 0)
		return unset(command, env);
	if (ft_strcmp(cmd_name, "env") == 0)
		return (env_builtin(*env));
	if (ft_strcmp(cmd_name, "exit") == 0)
		return (exit_builtin(command, env, bin));
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
int check_path(t_token *command, char **env, t_list **bin, pid_t pid)
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
	path = get_env_value("PATH", env);
	if (!path)
	{
		free(cmd_name);
		return (-1);
	}
	paths = ft_split(path, ':');
	free(path);
	if (!paths)
	{
		free(cmd_name);
		return (-1);
	}
	i = 0;
	while (paths[i])
	{
		path_to_cmd = ft_strjoin(paths[i], cmd_name);
		if ((stat(path_to_cmd, &sb) == 0 && sb.st_mode & S_IXUSR)) //check if executable
		{
			ft_lstadd_back(bin, ft_lstnew(path_to_cmd));//PROTECT
			command->content = path_to_cmd;
			free(cmd_name);
			free_strs(paths);
			return exec_cmd(command, env, pid);
		}
		i++;
		free(path_to_cmd);
	}
	free(cmd_name);
	free_strs(paths);
	return (-1);
}


/**
 * @brief Search the command executable and try execute it
 * 
 * @param command Linked list of command (path or name) and arguments
 * @param env Environment variables
 * @return int -1 if the search fails, 1 if execution fails, 0 if no error
 */
int search_cmd(t_token *command, char ***env, t_list **bin, pid_t pid)
{
	char *cmd_name;
	int ret;
	
	ret = 0;
	if (command == NULL)
		return (-1);
	cmd_name = command->content; 
	if (ft_strchr(cmd_name, '/') == NULL) //check if cmd_name == NULL ?
	{
		ret = check_builtin(command, env, bin);
		if (ret != -1)
			return ret;
		ret = check_path(command, *env, bin, pid);
		if (ret != -1)
			return ret;
		return cmd_not_found(command);
	}
	else
	{
		ret = exec_cmd(command, *env, pid);
	}
	
	return ret;
}


