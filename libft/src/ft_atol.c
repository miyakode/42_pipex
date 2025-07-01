/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <xxxxxxxx@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 23:37:17 by xxxxxxxx          #+#    #+#             */
/*   Updated: 2024/12/02 21:38:33 by xxxxxxxx         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long	ft_atol(const char *str)
{
	int			sign;
	long long	n;

	sign = 1;
	n = 0;
	while (*str == 32 || (9 <= *str && *str <= 13))
		str++;
	while (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (48 <= *str && *str <= 57)
	{
		n = (n * 10) + (*str - 48);
		str++;
	}
	return (n * sign);
}
