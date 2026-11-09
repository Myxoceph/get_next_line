/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/09 13:36:28 by abakirca          #+#    #+#             */
/*   Updated: 2026/11/09 13:36:28 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

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

typedef struct s_fd_node
{
	int					fd;
	t_reader			reader;
	struct s_fd_node	*next;
}	t_fd_node;

char	*get_next_line(int fd);
void	copy_bytes(char *dst, const char *src, size_t len);
void	reader_clear(t_reader *rd);
int		reader_reserve(t_reader *rd, size_t extra);
char	*reader_cut(t_reader *rd, size_t len);

#endif
