/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:15 by dkham             #+#    #+#             */
/*   Updated: 2023/06/28 21:54:00 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// 문제: 1, 3번이 식사 후 usleep에서 잘 빠져나오지 못해 식사 시간이 2배 소요 되고 있음 (해결)

// visualizer 이용하기
// time_lapse 함수 제대로 작동하는 것 맞는지 확인 필요
// (검사 할 경우 sanitize지우고 하기)
// ./philo 3 610 200 200
// ./philo 4 410 200 200

// must_eat 입력되는 경우 데드락 발생? (완료)
// 철학자 1명인 경우? 예외처리 필요 (완료)
// 데이터 레이스 fsanitize로 체크하기

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
		ft_usleep(3); // 만약 짝수번째 철학자라면, 처음에 3ms만큼 쉬어준다.
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
		print_status("is thinking", p); //ft_usleep(1); ???
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
