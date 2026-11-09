/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 14:19:14 by abakirca          #+#    #+#             */
/*   Updated: 2026/11/09 14:51:46 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>

static size_t	find_line(t_reader *rd)
{
	if (rd->scanned < rd->start)
		rd->scanned = rd->start;
	while (rd->scanned < rd->end)
	{
		if (rd->data[rd->scanned] == '\n')
			return (rd->scanned - rd->start + 1);
		rd->scanned++;
	}
	return (0);
}

static ssize_t	fill_reader(t_reader *rd, int fd)
{
	ssize_t	bytes;
	int		reserved;

	reserved = reader_reserve(rd, BUFFER_SIZE);
	if (!reserved)
		return (-1);
	bytes = read(fd, rd->data + rd->end, BUFFER_SIZE);
	if (bytes > 0)
		rd->end += bytes;
	return (bytes);
}

static t_fd_node	*fd_node_get(t_fd_node **head, int fd)
{
	t_fd_node	*node;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = *head;
	while (node && node->fd != fd)
		node = node->next;
	if (node)
		return (node);
	node = malloc(sizeof(t_fd_node));
	if (!node)
		return (NULL);
	node->fd = fd;
	node->reader.data = NULL;
	node->reader.cap = 0;
	node->reader.start = 0;
	node->reader.end = 0;
	node->reader.scanned = 0;
	node->next = *head;
	*head = node;
	return (node);
}

static void	fd_node_remove(t_fd_node **head, int fd)
{
	t_fd_node	**link;
	t_fd_node	*node;

	link = head;
	while (*link && (*link)->fd != fd)
		link = &(*link)->next;
	node = *link;
	if (!node)
		return ;
	*link = node->next;
	reader_clear(&node->reader);
	free(node);
}

char	*get_next_line(int fd)
{
	static t_fd_node	*head;
	t_fd_node			*node;
	ssize_t				bytes;
	size_t				len;
	char				*line;

	node = fd_node_get(&head, fd);
	if (!node)
		return (NULL);
	len = find_line(&node->reader);
	bytes = 1;
	while (len == 0 && bytes > 0)
	{
		bytes = fill_reader(&node->reader, fd);
		len = find_line(&node->reader);
	}
	if (len == 0 && bytes == 0)
		len = node->reader.end - node->reader.start;
	line = NULL;
	if (len > 0)
		line = reader_cut(&node->reader, len);
	if (bytes <= 0 || !line)
		fd_node_remove(&head, fd);
	return (line);
}
