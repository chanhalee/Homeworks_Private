/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   casefunc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chanhale <chanhale@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/19 14:25:56 by chanhale          #+#    #+#             */
/*   Updated: 2021/09/19 18:51:05 by chanhale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	proceed(int line, int **arr, int **sight);
int	three_two_sub(int a, int b);
int	three_one_sub(int a, int b);
int	two_two_sub(int a, int b);
int	two_one_sub(int a, int b);

int	four_one(int type, int line, int **arr, int **sight)
{
	int	col;
	int	newcol;

	col = -1;
	while (++col < 4)
	{
		newcol = col;
		if (type == -1)
			newcol = 3 - col;
		arr[line][col] = newcol + 1;
	}
	if (proceed(line + 1, arr, sight) == 1)
		return (1);
	else
		return (0);
}

int	three_two(int type, int line, int **arr, int **sight)
{
	int	subzcase;
	int	col;
	int	newcol;

	subzcase = -1;
	while (++subzcase < 3)
	{
		col = -1;
		while (++col < 4)
		{
			newcol = col;
			if (type == -1)
				newcol = 3 - col;
			arr[line][col] = three_two_sub(subzcase, newcol);
		}
		if (proceed(line + 1, arr, sight) == 1)
			return (1);
	}
	return (0);
}

int	three_one(int type, int line, int **arr, int **sight)
{
	int	subzcase;
	int	col;
	int	newcol;

	subzcase = -1;
	while (++subzcase < 2)
	{
		col = -1;
		while (++col < 4)
		{
			newcol = col;
			if (type == -1)
				newcol = 3 - col;
			arr[line][col] = three_one_sub(subzcase, newcol);
		}
		if (proceed(line + 1, arr, sight) == 1)
			return (1);
	}
	return (0);
}

int	two_two(int type, int line, int **arr, int **sight)
{
	int	subzcase;
	int	col;
	int	newcol;

	subzcase = -1;
	while (++subzcase < 6)
	{
		col = -1;
		while (++col < 4)
		{
			newcol = col;
			if (type == -1)
				newcol = 3 - col;
			arr[line][col] = two_two_sub(subzcase, newcol);
		}
		if (proceed(line + 1, arr, sight) == 1)
			return (1);
	}
	return (0);
}

int	two_one(int type, int line, int **arr, int **sight)
{
	int	subzcase;
	int	col;
	int	newcol;

	subzcase = -1;
	while (++subzcase < 2)
	{
		col = -1;
		while (++col < 4)
		{
			newcol = col;
			if (type == -1)
				newcol = 3 - col;
			arr[line][col] = two_one_sub(subzcase, newcol);
		}
		if (proceed(line + 1, arr, sight) == 1)
			return (1);
	}
	return (0);
}
