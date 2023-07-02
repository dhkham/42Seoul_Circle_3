/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 19:32:36 by dkham             #+#    #+#             */
/*   Updated: 2023/07/02 13:52:43 by dkham            ###   ########.fr       */
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
	if (str[i] != '\0')
		return (-1);
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

void	parse_arguments(int argc, char **argv, t_args *args)
{
	if (argc < 5 || argc > 6)
	{
		printf("wrong number of arguments\n");
		exit (1);
	}
	args->num_of_philo = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->num_of_must_eat = ft_atoi(argv[5]);
	else
		args->num_of_must_eat = 0;
	if (args->num_of_philo <= 0 || args->time_to_die <= 0 || \
	args->time_to_eat <= 0 || args->time_to_sleep <= 0 || \
	(argc == 6 && args->num_of_must_eat < 0))
	{
		printf("Error: All arguments must be positive integers.\n");
		exit(1);
	}
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
		usleep(1000);
}
