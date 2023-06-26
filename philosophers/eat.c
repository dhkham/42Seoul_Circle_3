/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:12:05 by dkham             #+#    #+#             */
/*   Updated: 2023/06/26 20:00:33 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(&p->resrcs->forks[p->right_fork]); // 짝수 철학자는 오른쪽 포크를 먼저 잠급니다.
		print_status("has taken a fork", p);
		pthread_mutex_lock(&p->resrcs->forks[p->left_fork]); // 그리고 왼쪽 포크를 잠급니다.
		print_status("has taken a fork", p);
	}
	else
	{
		pthread_mutex_lock(&p->resrcs->forks[p->left_fork]); // 홀수 철학자는 왼쪽 포크를 먼저 잠급니다.
		print_status("has taken a fork", p);
		pthread_mutex_lock(&p->resrcs->forks[p->right_fork]); // 그리고 오른쪽 포크를 잠급니다.
		print_status("has taken a fork", p);
	}
}

void	eat(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->last_meal_time); // 마지막 식사 시간을 잠그고 식사를 시작합니다.
	p->last_meal_time = get_time(); // 마지막 식사 시간을 현재 시간으로 갱신합니다.
	print_status("is eating", p); // 철학자가 식사 중임을 출력합니다.
	time_lapse(p->args.time_to_eat);
	//usleep(p->args.time_to_eat * 1000); // 철학자가 식사하는 시간만큼 잠듭니다.
	p->eat_count++; // 철학자의 식사 횟수를 증가시킵니다.
	pthread_mutex_unlock(&p->resrcs->last_meal_time); // 식사가 끝나면 마지막 식사 시간 뮤텍스를 해제합니다.
}

void	put_down_forks(t_philo *p)
{
	pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]); // 왼쪽 포크를 내려놓습니다.
	pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]); // 오른쪽 포크를 내려놓습니다.
}
