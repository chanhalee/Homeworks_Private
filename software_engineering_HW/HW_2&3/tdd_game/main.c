/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeoyoon <yeoyoon@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 21:28:28 by yeoyoon           #+#    #+#             */
/*   Updated: 2021/09/19 18:49:07 by chanhale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int		proceed(int line, int **arr, int **sight);
int		parse_input(char *str, int **sight);
int		check_argv_len(char *argv);
void	print_arr(int **arr);
void	memfreenalloc(int **sight, int **matrix, int zerofreeonealoc);

int	main(int argc, char **argv)
{
	int	**sight;
	int	**result;
	int	flag;

	if ((argc == 2) && (check_argv_len(argv[1])))
	{
		sight = (int **)malloc(sizeof(int *) * 4);
		result = (int **)malloc(sizeof(int *) * 4);
		memfreenalloc(sight, result, 1);
		flag = parse_input(argv[1], sight);
		if (flag)
		{
			if (proceed(0, result, sight) == 1)
			{
				print_arr(result);
				memfreenalloc(sight, result, 0);
				return (0);
			}
		}
		memfreenalloc(sight, result, 0);
	}
	write(2, "Error\n", 6);
	return (1);
}

int	parse_input(char *str, int **sight)
{
	int	counter1;
	int	counter2;
	int	container;

	counter1 = -1;
	while (++counter1 < 4)
	{
		counter2 = -1;
		while (++counter2 < 4)
		{
			container = ((counter1 * 4) + counter2) * 2;
			if ((str[container] < '1') || (str[container] > '4'))
				return (0);
			else
				sight[counter1][counter2] = (int)(str[container] - '0');
			if (container + 1 < 31)
				if (str[container + 1] != ' ')
					return (0);
		}
	}
	return (1);
}

void	print_arr(int **arr)
{
	int		r;
	int		c;
	char	k;

	r = -1;
	while (++r < 4)
	{
		c = 0;
		k = (char)arr[r][0] + '0';
		write(1, &k, 1);
		while (++c < 4)
		{
			write(1, " ", 1);
			k = (char) arr[r][c] + '0';
			write(1, &k, 1);
		}
		write(1, "\n", 1);
	}
}

int	check_argv_len(char *argv)
{
	int	i;

	i = 0;
	while (argv[i] != '\0')
		i++;
	if (i == 31)
		return (1);
	return (0);
}

void	memfreenalloc(int **sight, int **matrix, int zerofreeonealoc)
{
	int	counter;

	counter = 0;
	if (zerofreeonealoc)
	{
		while (counter < 4)
		{
			matrix[counter] = (int *)malloc(sizeof(int) * 4);
			sight[counter++] = (int *)malloc(sizeof(int) * 4);
		}
	}
	else
	{
		while (counter < 4)
		{
			free(sight[counter]);
			free(matrix[counter]);
			counter++;
		}
		free(sight);
		free(matrix);
	}
}
