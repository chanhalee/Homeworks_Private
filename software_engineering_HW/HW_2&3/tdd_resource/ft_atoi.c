char	*removespace(char *s)
{
	int	c;

	c = 0;
	while ((s[c] == '\t' || s[c] == '\n' || s[c] == '\v'
			|| s[c] == '\f' || s[c] == '\r' || s[c] == ' ')
		&& s[c] != '\0')
		c++;
	return (s + c);
}

int	ft_atoi(char *str)
{
	int	counter;
	int	minuscounter;
	int	result;

	result = 0;
	counter = 0;
	minuscounter = 0;
	str = removespace(str);
	while (str[counter] != '\0' && (str[counter] == '-'
			|| str[counter] == '+'))
		if (str[counter++] == '-')
			minuscounter++;
	if (minuscounter % 2 == 1)
		minuscounter = -1;
	else
		minuscounter = 1;
	while (str[counter] != '\0' && str[counter] >= '0'
		&& str[counter] <= '9')
	{
		result *= 10;
		result = result + (str[counter] - '0') * minuscounter;
		counter++;
	}
	return (result);
}
