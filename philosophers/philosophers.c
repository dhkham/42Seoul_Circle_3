/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:15 by dkham             #+#    #+#             */
/*   Updated: 2023/07/02 11:19:44 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philo		*philo;
	t_resrcs	resrcs;
	t_args		args;
	int			i;

	parse_arguments(argc, argv, &args);
	init_resrcs(&resrcs, &args);
	philo = malloc(sizeof(t_philo) * args.num_of_philo);
	init_philo_and_run(philo, &resrcs, &args);
	monitor(philo);
	i = 0;
	while (i < args.num_of_philo)
	{
		pthread_join(philo[i].threads, NULL);
		i++;
	}
	destroy_resources(&resrcs, &args);
	free(philo);
	return (0);
}

void	*philosopher(void *args)
{
	t_philo		*p;

	p = (t_philo *)args;
	if (p->id % 2 == 0)
		ft_usleep(3);
	while (1)
	{
		if (take_forks(p) == 1)
			return (NULL);
		if (eat(p) == 1)
			return (NULL);
		if (put_down_forks(p) == 1)
			return (NULL);
		print_status("is sleeping", p);
		ft_usleep(p->args.time_to_sleep);
		print_status("is thinking", p);
		ft_usleep(1);
	}
	return (NULL);
}

void	monitor(t_philo *p)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < p[0].args.num_of_philo)
		{
			if (monitor_death(p, i) == 1)
				return ;
			if (monitor_eating(p, i) == 1)
				return ;
			i++;
			ft_usleep(1);
		}
	}
}

int	monitor_death(t_philo *p, int i)
{
	long	cur_time;

	pthread_mutex_lock(&p[i].resrcs->last_meal_time);
	pthread_mutex_lock(&p[i].resrcs->alive);
	cur_time = get_time();
	if (cur_time - p[i].last_meal_time > p[i].args.time_to_die)
	{
		p[i].resrcs->alive_stat = 0;
		pthread_mutex_unlock(&p[i].resrcs->last_meal_time);
		pthread_mutex_unlock(&p[i].resrcs->alive);
		printf("%lld %d %s\n", (get_time() - p->resrcs->start_time), \
		p->id, "died");
		return (1);
	}
	pthread_mutex_unlock(&p[i].resrcs->last_meal_time);
	pthread_mutex_unlock(&p[i].resrcs->alive);
	return (0);
}

int	monitor_eating(t_philo *p, int i)
{
	pthread_mutex_lock(&p[i].resrcs->full);
	if (p[i].args.num_of_philo == p[i].resrcs->full_count)
	{
		pthread_mutex_lock(&p[i].resrcs->alive);
		p[i].resrcs->alive_stat = 0;
		pthread_mutex_unlock(&p[i].resrcs->alive);
		pthread_mutex_unlock(&p[i].resrcs->full);
		return (1);
	}
	pthread_mutex_unlock(&p[i].resrcs->full);
	return (0);
}
