/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:23:51 by abakirca          #+#    #+#             */
/*   Updated: 2026/02/16 17:21:43 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	found_newline(t_list *stash)
{
	int		i;
	t_list	*current;

	if (!stash)
		return (0);
	current = ft_lstlast(stash);
	i = 0;
	while (current->content[i])
	{
		if (current->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*tmp;

	tmp = lst;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	add_to_stash(t_list **stash, char *buf)
{
	t_list	*last;
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(buf);
		return ;
	}
	new_node->next = NULL;
	new_node->content = buf;
	if (!(*stash))
	{
		*stash = new_node;
		return ;
	}
	last = ft_lstlast(*stash);
	last->next = new_node;
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	line_len(t_list *stash)
{
	int	i;
	int	len;

	if (!stash)
		return (0);
	len = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
				return (len + 1);
			i++;
			len++;
		}
		stash = stash->next;
	}
	return (len);
}
