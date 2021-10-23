/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_col.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeoyoon <yeoyoon@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 21:28:59 by yeoyoon           #+#    #+#             */
/*   Updated: 2021/09/19 18:40:36 by chanhale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	up(int **arr, int **sight, int count)
{
	int	a;
	int	b;
	int	max;

	a = -1;
	while (++a < 4)
	{
		b = -1;
		count = 0;
		max = 0;
		while (++b < 4)
		{
			if (arr[0][a] - arr[b][a] <= 0)
			{
				if (max < arr[b][a])
				{
					max = arr[b][a];
					count++;
				}
			}
		}
		if (sight[0][a] != count)
			 return (0);
	}
	return (1);
}

int	down(int **arr, int **sight, int count)
{
	int	a;
	int	b;
	int	max;

	a = -1;
	while (++a < 4)
	{
		b = -1;
		count = 0;
		max = 0;
		while (++b < 4)
		{
			if (arr[3][a] - arr[3 - b][a] <= 0)
			{
				if (max < arr[3 - b][a])
				{
					max = arr[3 - b][a];
					count++;
				}
			}
		}
		if (sight[1][a] != count)
			return (0);
	}
	return (1);
}

int	check_col(int **a, int **s)
{
	int	c;

	c = 0;
	if (up(a, s, c) == 0 || down(a, s, c) == 0)
		return (0);
	return (1);
}
