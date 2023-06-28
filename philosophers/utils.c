/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 19:32:36 by dkham             #+#    #+#             */
/*   Updated: 2023/06/28 20:14:20 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	minus;
	int	result;

	i = 0;
	minus = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-')
	{
		minus = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (minus * result);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	return ((char *)s + i);
}

void	print_status(char *status, t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive);
	pthread_mutex_lock(&p->resrcs->print_mutex);
	if (!p->resrcs->alive_stat)
	{
		pthread_mutex_unlock(&p->resrcs->alive);
		pthread_mutex_unlock(&p->resrcs->print_mutex);
		return ;
	}
	else
		printf("%lld %d %s\n", (get_time() - p->resrcs->start_time), \
		p->id, status);
	pthread_mutex_unlock(&p->resrcs->alive);
	pthread_mutex_unlock(&p->resrcs->print_mutex);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (long)1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long time)
{
	long long	start_time;

	start_time = get_time();
	while (get_time() < start_time + time)
		usleep(50);
}
