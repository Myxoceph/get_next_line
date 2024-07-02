/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 13:59:32 by abakirca          #+#    #+#             */
/*   Updated: 2024/02/14 15:40:32 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

static char	*copy_to_stash(char *stash, char *buffer)
{
	char	*res;

	if (!stash && buffer)
		return (ft_strdup(buffer));
	res = ft_strjoin(stash, buffer);
	free_stash(&stash, 0);
	return (res);
}

static int	nl_check(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static char	*extract_line(char *stash)
{
	char	*line;
	size_t	i;
	size_t	j;

	i = 0;
	while (stash[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (0);
	j = 0;
	while (j < i + 1)
	{
		line[j] = stash[j];
		j++;
	}
	line[j] = '\0';
	return (line);
}

static char	*recreate_stash(char *stash)
{
	size_t	i;
	char	*res;

	i = 0;
	while (stash[i] != '\n')
		i++;
	if (stash[i + 1] == '\0')
		return (free_stash(&stash, 0));
	res = ft_substr(stash, i + 1, ft_strlen(stash));
	free_stash(&stash, 0);
	return (res);
}

char	*get_next_line(int fd)
{
	char		buffer[BUFFER_SIZE + 1];
	long		retval;
	static char	*stash = NULL;
	char		*line;

	retval = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_stash(&stash, 0));
	while (retval > 0)
	{
		retval = read(fd, buffer, BUFFER_SIZE);
		if ((retval == 0 && !stash) || retval == -1)
			return (free_stash(&stash, 0));
		buffer[retval] = '\0';
		stash = copy_to_stash(stash, buffer);
		if (nl_check(stash))
		{
			line = extract_line(stash);
			if (!line)
				return (free_stash(&stash, 0));
			return (stash = recreate_stash(stash), line);
		}
	}
	return (free_stash(&stash, 1));
}
