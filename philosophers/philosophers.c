/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:15 by dkham             #+#    #+#             */
/*   Updated: 2023/06/25 19:26:40 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// 짝 홀 구현
// 짝수번째 철학자와 홀수번째 철학자 잘 조율해야
// visualizer 이용하기

// 질문: 아래 함수 써야함??
// void    time_lapse(unsigned int time)
// {
// 	unsigned int    start_time;
// 	start_time = get_time();
// 	while (get_time() < start_time + (unsigned int)time)
// 	{
// 		usleep(50);
// 	}
// }

int	main(int argc, char **argv)
{
	t_philo			*philo;
	t_resrcs		resrcs;
	t_args			args;
	unsigned int	i;
	pthread_t		tid;

	if (argc < 5 || argc > 6)
	{
		printf("wrong number of arguments\n");
		return (1);
	}
	parse_arguments(argc, argv, &args);
	init_resrcs(&resrcs, &args);
	philo = malloc(sizeof(t_philo) * args.num_of_philo);
	init_philo_and_run(philo, &resrcs, &args);
	pthread_create(&tid, NULL, &monitor_death, philo);
	pthread_detach(tid);
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
	while (p->resrcs->alive_stat && \
	(!p->args.num_of_must_eat || p->eat_count < p->args.num_of_must_eat))
	{
		take_forks(p);
		eat(p);
		put_down_forks(p);
		if (p->args.num_of_must_eat && p->eat_count >= p->args.num_of_must_eat)
			break ;
		print_status("is sleeping", p);
		usleep(p->args.time_to_sleep * 1000);
		print_status("is thinking", p);
	}
	return (NULL);
}

void	*monitor_death(void *args)
{
	t_philo			*p;
	unsigned int	i;
	long			cur_time;

	p = (t_philo *)args;
	while (1) // 무한히 반복하여 철학자들의 상태를 지속적으로 감시
	{
		i = 0;
		while (i < p[0].args.num_of_philo)
		{
			pthread_mutex_lock(&p[i].resrcs->last_meal_time); // 철학자의 마지막 식사 시간에 대한 접근을 동기화하기 위해 뮤텍스를 잠급니다.
			cur_time = get_time();
			if (!p[i].resrcs->alive_stat || cur_time - p[i].last_meal_time > p[i].args.time_to_die) // 철학자가 죽었거나, 현재 시간과 마지막 식사 시간의 차이가 time_to_die를 초과했다면
			{
				p[i].resrcs->alive_stat = 0; // 철학자가 죽었음을 표시합니다.
				print_status("died", &p[i]); // 철학자가 죽었음을 출력합니다.
				return (NULL); // 철학자가 죽었으므로 감시자 스레드를 종료합니다.
			}
			pthread_mutex_unlock(&p[i].resrcs->last_meal_time); // 철학자의 마지막 식사 시간에 대한 뮤텍스를 해제
			i++;
		}
		usleep(1000); // 철학자의 상태 체크 간에 잠시 멈춤
	}
}

void	print_status(char *status, t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->print_mutex); // 출력을 동기화하기 위해 뮤텍스를 잠급니다.
	printf("%lld %d %s\n", (get_time() - p->resrcs->start_time), p->id, status); // 철학자의 상태를 출력합니다. 출력되는 시간은 프로그램 시작 시간으로부터의 상대적인 시간입니다.
	pthread_mutex_unlock(&p->resrcs->print_mutex); // 출력이 끝나면 뮤텍스를 해제합니다.
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL); // 현재 시간을 가져옵니다.
	return ((tv.tv_sec * (long)1000) + (tv.tv_usec / 1000)); // 현재 시간을 밀리초 단위로
}
