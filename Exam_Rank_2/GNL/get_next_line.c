/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iderighe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 10:46:17 by iderighe          #+#    #+#             */
/*   Updated: 2021/06/14 11:27:23 by iderighe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	recursive(char **line, int index, int fd)
{
	char	buf;
	int	ret;

	ret = read(fd, &buf, 1);
	if (ret == 1 && buf != '\n')
	{
		recursive(line, index + 1, fd);
		if (ret != -1)
			(*line)[index] = buf;
	}
	else if (ret != -1)
	{
		*line = (char *)malloc(sizeof(char) * (index + 1));
		if (*line != NULL)
			(*line)[index] = '\n';
		else
			ret = -1;
	}
	return (ret);
}

int	get_next_line(char **line)
{
	return (recursive(line, 0, 0));
}
