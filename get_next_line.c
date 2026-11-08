/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 12:45:47 by abakirca          #+#    #+#             */
/*   Updated: 2026/11/08 12:45:47 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
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

char	*get_next_line(int fd)
{
	static t_reader	rd;
	ssize_t			bytes;
	size_t			len;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	len = find_line(&rd);
	bytes = 1;
	while (len == 0 && bytes > 0)
	{
		bytes = fill_reader(&rd, fd);
		len = find_line(&rd);
	}
	if (len == 0)
	{
		reader_clear(&rd);
		return (NULL);
	}
	line = reader_cut(&rd, len);
	return (line);
}
