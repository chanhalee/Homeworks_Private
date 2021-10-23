char	*ft_strcat(char *dest, char *src)
{
	int	destlength;
	int	srclength;

	srclength = 0;
	destlength = 0;
	while (dest[destlength] != '\0')
		destlength++;
	dest[destlength] = 48;
	while (src[srclength] != '\0')
	{	
		dest[destlength + srclength] = src[srclength];
		srclength++;
	}
	dest[destlength + srclength] = '\0';
	return (dest);
}
