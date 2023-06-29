/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:12:05 by dkham             #+#    #+#             */
/*   Updated: 2023/06/29 18:55:56 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive);
	if (p->resrcs->alive_stat == 0)
	{
		pthread_mutex_unlock(&p->resrcs->alive);
		return (1);
	}
	pthread_mutex_unlock(&p->resrcs->alive);
	pthread_mutex_lock(&p->resrcs->forks[p->left_fork]);
	print_status("has taken a fork", p);
	p->resrcs->forks_stat[p->left_fork] = 1;
	pthread_mutex_lock(&p->resrcs->forks[p->right_fork]);
	print_status("has taken a fork", p);
	p->resrcs->forks_stat[p->right_fork] = 1;
	return (0);
}

int	eat(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive);
	if (p->resrcs->alive_stat == 0)
	{
		pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]);
		pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]);
		pthread_mutex_unlock(&p->resrcs->alive);
		return (1);
	}
	pthread_mutex_unlock(&p->resrcs->alive);
	print_status("is eating", p);
	pthread_mutex_lock(&p->resrcs->last_meal_time);
	p->last_meal_time = get_time();
	p->eat_count++;
	pthread_mutex_unlock(&p->resrcs->last_meal_time);
	ft_usleep(p->args.time_to_eat);
	pthread_mutex_lock(&p->resrcs->full);
	if (p->eat_count == p->args.num_of_must_eat)
		p->resrcs->full_count++;
	pthread_mutex_unlock(&p->resrcs->full);
	return (0);
}

int	put_down_forks(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive);
	if (p->resrcs->alive_stat == 0)
	{
		pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]);
		pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]);
		pthread_mutex_unlock(&p->resrcs->alive);
		return (1);
	}
	pthread_mutex_unlock(&p->resrcs->alive);
	if (p->resrcs->forks_stat[p->left_fork] == 1)
		p->resrcs->forks_stat[p->left_fork] = 0;
	if (p->resrcs->forks_stat[p->right_fork] == 1)
		p->resrcs->forks_stat[p->right_fork] = 0;
	pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]);
	pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]);
	return (0);
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
