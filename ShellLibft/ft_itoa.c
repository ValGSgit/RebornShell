/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vagarcia <vagarcia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 16:46:52 by vagarcia          #+#    #+#             */
/*   Updated: 2025/03/18 12:26:54 by vagarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	numlen(int n)
{
    int	count;

    count = 0;
    while (n)
    {
        count++;
        n /= 10;
    }
    return (count);
}

char	*ft_itoa(int n)
{
    char	*nbstr;
    int		len;
    long	num;

    num = n;
    len = numlen(n);
	if (len == 0)
		len = 1;
    nbstr = (char *)malloc(sizeof(char) * (len + 1));
    if (!nbstr)
        return (NULL);
    nbstr[len] = '\0';
    if (num < 0)
    {
        nbstr[0] = '-';
        num = -num;
    }
    while (--len >= 0 && nbstr[len] != '-')
    {
        nbstr[len] = '0' + (num % 10);
        num /= 10;
    }
    return (nbstr);
}
/*
#include <stdio.h>
int	main(void)
{
	printf("Itoa test1: %s\n", ft_itoa(-1234567));
	printf("Itoa test2: %s\n", ft_itoa(3456789));
	printf("Itoa test3: %s\n", ft_itoa(2147483647));
	printf("Itoa test4: %s\n", ft_itoa(-2147483648));
	printf("Itoa test5: %s\n", ft_itoa(10000000));
	printf("Itoa test6: %s\n", ft_itoa(-10000000));
	printf("Itoa test7: %s\n", ft_itoa(-1));
	printf("Itoa test8: %s\n", ft_itoa(-0));
	printf("Itoa test1: %s\n", ft_itoa(0));
}*/
