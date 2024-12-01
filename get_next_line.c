/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlaffita <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 14:10:42 by mlaffita          #+#    #+#             */
/*   Updated: 2024/12/01 19:10:54 by mlaffita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_free(char **line)
{
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	return (NULL);
}

int	read_to_buffer(int fd, char *buffer)
{
	int	rd;

	rd = read(fd, buffer, BUFFER_SIZE);
	if (rd >= 0)
		buffer[rd] = '\0';
	return (rd);
}

int	update_line(char *buffer, char **line)
{
	char	*n_buff;

	n_buff = ft_strchr(buffer, '\n');
	if (n_buff)
	{
		*n_buff = '\0';
		*line = ft_strjoin(*line, buffer);
		if (!*line)
		{
			ft_free(&buffer);
			return (0);
		}
		*line = ft_strjoin(*line, "\n");
		if (!*line)
		{
			ft_free(&buffer);
			return (0);
		}
		buffer = ft_strndup(n_buff + 1, BUFFER_SIZE - (n_buff - buffer));
		return (1);
	}
	*line = ft_strjoin(*line, buffer);
	if (!*line)
		return (0);
	*buffer = '\0';
	return (0);
}

int	handle_end_of_file(int rd, char *buffer, char **line)
{
	if (rd == GNL_ERROR)
		return (0);
	if (rd == GNL_EOF && !*line && buffer[0] == '\0')
		return (0);
	return (1);
}

char	*get_next_line(int fd)
{
	char		buffer[BUFFER_SIZE + 1] = {0};
	char		*line;
	int			rd;
	static char *static_buffer;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		rd = read_to_buffer(fd, buffer);
		if (!handle_end_of_file(rd, buffer, &line))
			return (ft_free(&line));
		static_buffer = ft_strjoin(static_buffer, buffer);
		if (!static_buffer)
			return (ft_free(&line));
		if (update_line(static_buffer, &line))
			return (line);
		if (rd == GNL_EOF)
			break ;
	}
	return (line);
}

// #include <stdio.h>
// #include <fcntl.h>
// int	main(int ac, char **av)
// {
// 	int 	fd;
// 	char	*line;

// 	fd = open(*(++av), O_RDONLY);
// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("Read line: '%s'", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return 0;
// }