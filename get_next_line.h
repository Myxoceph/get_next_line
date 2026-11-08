/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 11:54:23 by abakirca          #+#    #+#             */
/*   Updated: 2026/11/08 11:44:12 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

typedef struct s_reader
{
	char	*data;
	size_t	cap;
	size_t	start;
	size_t	end;
	size_t	scanned;
}	t_reader;

char	*get_next_line(int fd);
void	copy_bytes(char *dst, const char *src, size_t len);
void	reader_clear(t_reader *rd);
int		reader_reserve(t_reader *rd, size_t extra);
char	*reader_cut(t_reader *rd, size_t len);

#endif
