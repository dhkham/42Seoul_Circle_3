/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:12:05 by dkham             #+#    #+#             */
/*   Updated: 2023/06/28 18:31:26 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	take_forks(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive); // 철학자가 죽었는지 확인합니다.
	if (p->resrcs->fin_stat == 0)
	{
		pthread_mutex_unlock(&p->resrcs->alive);
		return 1;
	}
	pthread_mutex_unlock(&p->resrcs->alive);
	// if (p->id % 2 == 0)
	// {
		pthread_mutex_lock(&p->resrcs->forks[p->left_fork]); // 그리고 왼쪽 포크를 잠급니다.
		print_status("has taken a fork", p);
		pthread_mutex_lock(&p->resrcs->forks[p->right_fork]); // 짝수 철학자는 오른쪽 포크를 먼저 잠급니다.
		print_status("has taken a fork", p);
	// }
	// else
	// {
	// 	pthread_mutex_lock(&p->resrcs->forks[p->left_fork]); // 홀수 철학자는 왼쪽 포크를 먼저 잠급니다.
	// 	print_status("has taken a fork", p);
	// 	pthread_mutex_lock(&p->resrcs->forks[p->right_fork]); // 그리고 오른쪽 포크를 잠급니다.
	// 	print_status("has taken a fork", p);
	// }
	return (0);
}

int	eat(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive); // 철학자가 죽었는지 확인합니다.
	if (p->resrcs->fin_stat == 0)
	{
		pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]); // 왼쪽 포크를 내려놓습니다.
		pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]); // 오른쪽 포크를 내려놓습니다.
		pthread_mutex_unlock(&p->resrcs->alive);
		return 1;
	}
	pthread_mutex_unlock(&p->resrcs->alive);
	print_status("is eating", p); // 철학자가 식사 중임을 출력합니다.
	pthread_mutex_lock(&p->resrcs->last_meal_time); // 마지막 식사 시간을 잠그고 식사를 시작합니다.
	p->last_meal_time = get_time(); // 마지막 식사 시간을 현재 시간으로 갱신합니다.
	ft_usleep(p->args.time_to_eat); //usleep(p->args.time_to_eat * 1000); // 철학자가 식사하는 시간만큼 잠듭니다.
	p->eat_count++; // 철학자의 식사 횟수를 증가시킵니다.
	pthread_mutex_unlock(&p->resrcs->last_meal_time); // 식사가 끝나면 마지막 식사 시간 뮤텍스를 해제합니다.
	pthread_mutex_lock(&p->resrcs->full); // 철학자의 식사 상태에 대한 접근을 동기화하기 위해 뮤텍스를 잠급니다.

	if (p->eat_count == p->args.num_of_must_eat) // 철학자의 식사 횟수가 필수 식사 횟수와 같다면
		p->resrcs->full_stat++; // 필수 식사를 마친 철학자의 수를 증가시킵니다.
	pthread_mutex_unlock(&p->resrcs->full); // 철학자의 식사 상태에 대한 뮤텍스를 해제
	return (0);
}

int	put_down_forks(t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive); // 철학자가 죽었는지 확인합니다.
	if (p->resrcs->fin_stat == 0)
	{
		pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]); // 오른쪽 포크를 내려놓습니다.
		pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]); // 왼쪽 포크를 내려놓습니다.
		pthread_mutex_unlock(&p->resrcs->alive);
		return 1;
	}
	pthread_mutex_unlock(&p->resrcs->alive);
	pthread_mutex_unlock(&p->resrcs->forks[p->right_fork]); // 오른쪽 포크를 내려놓습니다.
	pthread_mutex_unlock(&p->resrcs->forks[p->left_fork]); // 왼쪽 포크를 내려놓습니다.
	return (0);
}
