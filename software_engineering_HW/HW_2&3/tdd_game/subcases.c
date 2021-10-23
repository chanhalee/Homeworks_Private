/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subcases.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeoyoon <yeoyoon@student.42seoul.k>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/18 21:28:49 by yeoyoon           #+#    #+#             */
/*   Updated: 2021/09/19 18:45:10 by chanhale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//1, 2, 4, 3
//1, 3, 4, 2
//2, 3, 4, 1
int	three_two_sub(int a, int b)
{
	if (b == 2)
		return (4);
	if (b == 0)
	{
		if (a == 0 || a == 1)
			return (1);
		else
			return (2);
	}
	if (b == 1)
	{
		if (a == 0)
			return (2);
		else
			return (3);
	}
	return (3 - a);
}
//1, 3, 2, 4
//2, 1, 3, 4
//2, 3, 1, 4

int	three_one_sub(int a, int b)
{
	if (b == 3)
		return (4);
	if (b == 1 && (a == 0 || a == 2))
		return (3);
	if (b == 2 && a == 1)
		return (3);
	if (b == 0 && (a == 1 || a == 2))
		return (2);
	if (b == 2 && a == 0)
		return (2);
	return (1);
}
//1, 4, 2, 3
//2, 4, 1, 3
//3, 4, 1, 2
//3, 2, 4, 1
//3, 1, 4, 2
//2, 1, 4, 3

int	two_two_sub(int a, int b)
{
	if ((b == 0 && (a < 5 && a > 1))
		|| (b == 3 && (a < 2 || a > 4)))
		return (3);
	if ((b == 1 && a < 3) || (b == 2 && a > 2))
		return (4);
	if ((b == 0 && a == 0) || (b == 3 && a == 3))
		return (1);
	if (b == 1 && (a == 4 || a == 5))
		return (1);
	if (b == 2 && (a == 1 || a == 2))
		return (1);
	return (2);
}
//3, 1, 2, 4
//3, 2, 1, 4

int	two_one_sub(int a, int b)
{
	if (b == 0)
		return (3);
	if (b == 3)
		return (4);
	if (a + b == 2)
		return (2);
	return (1);
}
