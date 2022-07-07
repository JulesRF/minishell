/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfiszbin <vfiszbin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:10:11 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/07/07 18:52:38 by vfiszbin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

void	shlvl_too_high(int *shlvl)
{
	ft_putstr_fd("minishell: warning: shell level (", 2);
	ft_putnbr_fd(*shlvl, 2);
	ft_putendl_fd(") too high, resetting to 1", 2);
	*shlvl = 1;
}

int	increment_shlvl(char ***env)
{
	char	*shlvl_str;
	char	*shlvl_var;
	int		shlvl;

	shlvl_str = get_env_value("SHLVL", *env);
	if (!shlvl_str)
		return (add_var_to_env("SHLVL=1", env));
	shlvl = ft_atoi(shlvl_str);
	free(shlvl_str);
	shlvl++;
	if (shlvl < 0)
		shlvl = 0;
	else if (shlvl >= 1000)
		shlvl_too_high(&shlvl);
	shlvl_str = ft_itoa(shlvl);
	if (!shlvl_str)
		return (1);
	shlvl_var = ft_strjoin("SHLVL=", shlvl_str);
	free(shlvl_str);
	if (!shlvl_var)
		return (1);
	shlvl = set_var_in_env(shlvl_var, "SHLVL", 5, env);
	free(shlvl_var);
	return (shlvl);
}

void	ft_initvars(t_token **token, t_data *data, t_list **bin)
{
	data->cmd = token;
	data->bin = bin;
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*bin;
	t_token	*token;
	t_data	data;
	char	**env;

	g_exit_status = 0;
	ft_preparse(argc, argv, envp);
	bin = NULL;
	token = NULL;
	env = dup_env(envp);
	if (!env)
		exit(1);
	if (increment_shlvl(&env) == 1)
	{
		free_strs_array(env);
		exit(1);
	}
	data.env = &env;
	ft_prompt(&token, &bin, &data);
	free_strs_array(env);
	ft_garbage(&bin);

	close(0);
	close(1);

	exit(g_exit_status);
}
