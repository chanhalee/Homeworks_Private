/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unique.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeoyoon <yeoyoon@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 21:28:34 by yeoyoon           #+#    #+#             */
/*   Updated: 2021/09/19 18:42:30 by chanhale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	order_array(int *arr)
{
	int	i;
	int	j;
	int	min;

	i = 0;
	while (i < 3)
	{
		j = i + 1;
		while (j < 4)
		{
			if (arr[j] < arr[i])
			{
				min = arr[j];
				arr[j] = arr[i];
				arr[i] = min;
			}
			j++;
		}
		i++;
	}
}

int	unique_col(int **arr)
{
	int	i;
	int	j;
	int	unique_test[4];

	i = -1;
	j = 0;
	while (j < 4)
	{
		i = -1;
		while (++i < 4)
			unique_test[i] = arr[i][j];
		order_array(unique_test);
		if (unique_test[0] != 1 || unique_test[1] != 2)
			return (0);
		else if (unique_test[2] != 3 || unique_test[3] != 4)
			return (0);
		j++;
	}
	return (1);
}
