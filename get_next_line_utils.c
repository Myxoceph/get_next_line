/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 12:13:47 by abakirca          #+#    #+#             */
/*   Updated: 2026/11/07 12:13:47 by abakirca         ###   ########.fr       */
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
