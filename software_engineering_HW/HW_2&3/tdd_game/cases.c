/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeoyoon <yeoyoon@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 21:28:08 by yeoyoon           #+#    #+#             */
/*   Updated: 2021/09/19 18:49:26 by chanhale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		unique_col(int **arr);
int		check_col(int **arr, int **sight);
int		four_one(int type, int line, int **arr, int **sight);
int		three_two(int type, int line, int **arr, int **sight);
int		three_one(int type, int line, int **arr, int **sight);
int		two_two(int type, int line, int **arr, int **sight);
int		two_one(int type, int line, int **arr, int **sight);
int		proceedsub(int type, int line, int **arr, int **sight);

int	proceed(int line, int **arr, int **sight)
{
	int	type;
	int	subreturns;

	type = 1;
	if ((sight[2][line] - sight[3][line]) < 0)
		type = -1;
	if (line < 4)
	{
		subreturns = proceedsub(type, line, arr, sight);
		return (subreturns);
	}
	else
	{
		if (unique_col(arr))
			return (check_col(arr, sight));
	}
	return (0);
}

int	proceedsub(int type, int line, int **arr, int **sight)
{
	if (sight[2][line] == 1 || sight[3][line] == 1)
	{
		if (sight[2][line] == 4 || sight[3][line] == 4)
			return (four_one(type, line, arr, sight));
		else if (sight[2][line] == 3 || sight[3][line] == 3)
			return (three_one(type, line, arr, sight));
		else if (sight[2][line] == 2 || sight[3][line] == 2)
			return (two_one(type, line, arr, sight));
		else
			return (2);
	}
	else if (sight[2][line] == 2 || sight[3][line] == 2)
	{
		if (sight[2][line] == 3 || sight[3][line] == 3)
			return (three_two(type, line, arr, sight));
		else if (sight[2][line] == 2 && sight[3][line] == 2)
			return (two_two(type, line, arr, sight));
		else
			return (2);
	}
	return (2);
}
