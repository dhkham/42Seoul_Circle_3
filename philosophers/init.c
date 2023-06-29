/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:12:26 by dkham             #+#    #+#             */
/*   Updated: 2023/06/29 18:51:49 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_resrcs(t_resrcs *resrcs, t_args *args)
{
	int	i;

	resrcs->forks = malloc(sizeof(pthread_mutex_t) * args->num_of_philo);
	resrcs->forks_stat = malloc(sizeof(int) * args->num_of_philo);
	i = 0;
	while (i < args->num_of_philo)
	{
		pthread_mutex_init(&resrcs->forks[i], NULL);
		resrcs->forks_stat[i] = 1;
		i++;
	}
	pthread_mutex_init(&resrcs->last_meal_time, NULL);
	pthread_mutex_init(&resrcs->full, NULL);
	pthread_mutex_init(&resrcs->alive, NULL);
	pthread_mutex_init(&resrcs->print_mutex, NULL);
	resrcs->start_time = get_time();
	resrcs->full_count = 0;
	resrcs->alive_stat = 1;
}

void	init_philo_and_run(t_philo *philo, t_resrcs *resrcs, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->num_of_philo)
	{
		init_philosopher(&philo[i], resrcs, args, i);
		if (args->num_of_philo == 1)
		{
			pthread_create(&philo[i].threads, NULL, philo_one, &philo[i]);
			break ;
		}
		pthread_create(&philo[i].threads, NULL, philosopher, &philo[i]);
		i++;
	}
}

void	*philo_one(void *args)
{
	t_philo		*p;

	p = (t_philo *)args;
	pthread_mutex_lock(&p->resrcs->forks[p->left_fork]);
	print_status("has taken a fork", p);
	p->resrcs->forks_stat[p->left_fork] = 1;
	pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]);
	return (NULL);
}

void	init_philosopher(t_philo *p, t_resrcs *r, t_args *a, int i)
{
	p->resrcs = r;
	p->args = *a;
	p->last_meal_time = r->start_time;
	p->id = i + 1;
	p->left_fork = i;
	if (i == a->num_of_philo - 1)
		p->right_fork = 0;
	else
		p->right_fork = i + 1;
	p->eat_count = 0;
}

void	destroy_resources(t_resrcs *resrcs, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->num_of_philo)
	{
		pthread_mutex_destroy(&resrcs->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&resrcs->last_meal_time);
	pthread_mutex_destroy(&resrcs->full);
	pthread_mutex_destroy(&resrcs->alive);
	pthread_mutex_destroy(&resrcs->print_mutex);
	free(resrcs->forks);
	free(resrcs->forks_stat);
}
