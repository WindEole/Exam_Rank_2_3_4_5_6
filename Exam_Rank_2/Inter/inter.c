/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iderighe <iderighe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 10:18:18 by iderighe          #+#    #+#             */
/*   Updated: 2023/03/17 13:40:03 by iderighe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	main(int argc, char **argv)
{
	int	cara[255] = {0};
	int	i;
	int j;

	if (argc == 3)
	{
		i = 2;
		while (i > 0)
		{
			j = 0;
			while (argv[i][j])
			{
				if (i == 2 && cara[(unsigned char)argv[i][j]] == 1)
					cara[(unsigned char)argv[i][j]] = 2;
				else if (i == 1 && !cara[(unsigned char)argv[i][j]])
				{
					write(1, &argv[i][j], 1);
					cara[(unsigned char)argv[i][j]] = 1;
				}
				j++;
			}
			i--;
		}
	}
	write(1, "\n", 1);
	return (0);
}
