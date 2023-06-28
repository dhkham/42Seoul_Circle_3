/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:12:05 by dkham             #+#    #+#             */
/*   Updated: 2023/06/28 18:48:58 by dkham            ###   ########.fr       */
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
	pthread_mutex_lock(&p->resrcs->forks[p->right_fork]);
	print_status("has taken a fork", p);
	return (0);
}

int	eat(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive);
	if (p->resrcs->alive_stat == 0)
	{
		pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]);
		pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]);
		pthread_mutex_unlock(&p->resrcs->alive);
		return (1);
	}
	pthread_mutex_unlock(&p->resrcs->alive);
	print_status("is eating", p);
	pthread_mutex_lock(&p->resrcs->last_meal_time);
	p->last_meal_time = get_time();
	ft_usleep(p->args.time_to_eat);
	p->eat_count++;
	pthread_mutex_unlock(&p->resrcs->last_meal_time);
	pthread_mutex_lock(&p->resrcs->full);
	if (p->eat_count == p->args.num_of_must_eat)
		p->resrcs->full_stat++;
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
	pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]);
	pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]);
	return (0);
}
