/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:15 by dkham             #+#    #+#             */
/*   Updated: 2023/06/29 20:00:48 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// visualizer 이용하기
// 검사 할 경우 fsanitize=thread 지우고 하기

/*
test:

200명 이상 철학자 금지
60ms 미만 시간 금지

// ./philo 3 610 200 200 (아무도 죽지 않아야 함)
// ./philo 4 410 200 200 (아무도 죽지 않아야 함)
// ./philo 1 800 200 200 (먹지 말고 죽어야 함)
// ./philo 5 800 200 200 (아무도 죽지 않아야 함)
// ./philo 5 800 200 200 7 (모든 철학자가 적어도 7번 먹었을 때 시뮬레이션 중지)
// ./philo 4 310 200 100 (죽어야 함)
 
- 2 명의 철학자와 테스트하고 다른 시간을 확인합니다 (10ms 이상 지연된 사망은 허용되지 않음).  
- 모든 규칙을 확인하기 위해 값으로 테스트하십시오. 철학자가 포크를 훔치지 않으면 적시에 죽는 지 확인하십시오.

*/

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
