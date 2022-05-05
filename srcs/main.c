/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jroux-fo <jroux-fo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 14:10:11 by jroux-fo          #+#    #+#             */
/*   Updated: 2022/05/04 16:30:20 by jroux-fo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_garbage(t_list **bin)
{
	t_list	*to_suppr;

	while (*bin)
	{
		to_suppr = *bin;
		*bin = to_suppr->next;
		free(to_suppr->content);
		free(to_suppr);
	}
	*bin = NULL;
}

t_list	*ft_lstnew(void *content)
{
	t_list	*newcell;

	newcell = malloc(sizeof(t_list));
	if (!newcell)
		return (NULL);
	newcell->content = content;
	newcell->next = NULL;
	return (newcell);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*temp;

	if (!*alst)
	{
		*alst = new;
		return ;
	}
	temp = ft_lstlast(*alst);
	temp->next = new;
}

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		i;

	i = 0;
	while (s1[i])
		i++;
	dest = malloc(sizeof(char) * i + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

void	ft_preparse(int argc, char **argv, char **env)
{
	(void)argv;
	(void)env;
	if (argc != 1)
	{
		printf("no need to put arguments, simply type <./minishell>\n");
		exit(1);
	}
	return ;
}

int	main(int argc, char **argv, char **env)
{
	// char	*str;
	t_list	*bin;

	ft_preparse(argc, argv, env);
	bin = NULL;

	ft_lstadd_back(&bin, ft_lstnew(ft_strdup("salut")));
	ft_lstadd_back(&bin, ft_lstnew(ft_strdup("ca")));
	ft_lstadd_back(&bin, ft_lstnew(ft_strdup("va ?")));
	// str = readline("\033[95mminishell$\033[33m ");
	// while (ft_strcmp(str, "exit") && str != NULL)
	// {
	// 	// printf("la string ->%s<-\n", str);
	// 	if (str[0] != '\0')
	// 		add_history(str);
	// 	free (str);
	// 	str = readline("\033[95mminishell$\033[33m ");
	// }
	// free (str);
	ft_garbage(&bin);
	exit(0);
}