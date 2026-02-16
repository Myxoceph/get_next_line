/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:23:49 by abakirca          #+#    #+#             */
/*   Updated: 2026/02/16 17:25:58 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>

static void	free_stash(t_list *stash)
{
	t_list	*tmp;
	t_list	*current;

	current = stash;
	while (current)
	{
		tmp = current->next;
		free(current->content);
		free(current);
		current = tmp;
	}
}

static void	read_and_stash(int fd, t_list **stash)
{
	char	*buf;
	int		bytes_read;

	bytes_read = 1;
	while (!found_newline(*stash) && bytes_read != 0)
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buf)
			return ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buf);
			return ;
		}
		buf[bytes_read] = '\0';
		if (bytes_read == 0)
		{
			free(buf);
			return ;
		}
		add_to_stash(stash, buf);
	}
}

static char	*extract_line(t_list *stash)
{
	char	*line;
	int		i;
	int		j;

	if (!stash)
		return (NULL);
	line = malloc(sizeof(char) * (line_len(stash) + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (stash)
	{
		j = 0;
		while (stash->content[j])
		{
			if (stash->content[j] == '\n')
				return (line[i++] = '\n', line[i] = '\0', line);
			line[i++] = stash->content[j++];
		}
		stash = stash->next;
	}
	line[i] = '\0';
	return (line);
}

static int	clean_stash(t_list **stash)
{
	t_list	*last;
	t_list	*clean_node;
	int		i;
	int		j;

	clean_node = malloc(sizeof(t_list));
	if (!stash || !clean_node)
		return (free_stash(*stash), *stash = NULL, 0);
	clean_node->next = NULL;
	last = ft_lstlast((*stash));
	i = -1;
	while (last->content[++i] && last->content[i] != '\n')
		;
	i += (last->content[i] == '\n');
	if (!last->content[i])
		return (free(clean_node), free_stash(*stash), *stash = NULL, 1);
	clean_node->content = malloc(sizeof(char)
			* (ft_strlen(last->content) - i + 1));
	if (!clean_node->content)
		return (free(clean_node), free_stash(*stash), *stash = NULL, 0);
	j = -1;
	while (last->content[i + (++j)])
		clean_node->content[j] = last->content[i + j];
	return (clean_node->content[j] = '\0', free_stash(*stash),
		*stash = clean_node, 1);
}

char	*get_next_line(int fd)
{
	static t_list	*stash[1024];
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
		return (NULL);
	read_and_stash(fd, &stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = extract_line(stash[fd]);
	if (!line)
	{
		free_stash(stash[fd]);
		stash[fd] = NULL;
		return (NULL);
	}
	if (!clean_stash(&stash[fd]))
	{
		free(line);
		return (NULL);
	}
	return (line);
}
