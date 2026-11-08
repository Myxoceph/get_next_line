/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 12:13:47 by abakirca          #+#    #+#             */
/*   Updated: 2026/11/08 11:44:12 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	copy_bytes(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		dst[i] = src[i];
		i++;
	}
}

void	reader_clear(t_reader *rd)
{
	free(rd->data);
	rd->data = NULL;
	rd->cap = 0;
	rd->start = 0;
	rd->end = 0;
	rd->scanned = 0;
}

static void	reader_compact(t_reader *rd)
{
	if (rd->scanned < rd->start)
		rd->scanned = rd->start;
	copy_bytes(rd->data, rd->data + rd->start, rd->end - rd->start);
	rd->end -= rd->start;
	rd->scanned -= rd->start;
	rd->start = 0;
}

int	reader_reserve(t_reader *rd, size_t extra)
{
	char	*fresh;
	size_t	new_cap;

	if (rd->start > 0)
		reader_compact(rd);
	if (rd->cap - rd->end >= extra)
		return (1);
	new_cap = rd->cap * 2;
	if (new_cap < rd->end + extra)
		new_cap = rd->end + extra;
	fresh = malloc(new_cap);
	if (!fresh)
		return (0);
	copy_bytes(fresh, rd->data, rd->end);
	free(rd->data);
	rd->data = fresh;
	rd->cap = new_cap;
	return (1);
}

char	*reader_cut(t_reader *rd, size_t len)
{
	char	*line;

	line = malloc(len + 1);
	if (!line)
	{
		reader_clear(rd);
		return (NULL);
	}
	copy_bytes(line, rd->data + rd->start, len);
	line[len] = '\0';
	rd->start += len;
	if (rd->start == rd->end)
	{
		rd->start = 0;
		rd->end = 0;
		rd->scanned = 0;
	}
	return (line);
}
