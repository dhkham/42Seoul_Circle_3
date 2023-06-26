/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:15 by dkham             #+#    #+#             */
/*   Updated: 2023/06/26 21:05:14 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// visualizer 이용하기

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
	monitor(philo); // pthread_create(&tid, NULL, &monitor_death, philo); // pthread_detach(tid);
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
	{// 철학자가 살아있고, 철학자가 먹어야 할 횟수가 0이거나 철학자가 먹은 횟수가 먹어야 할 횟수보다 작을 때
		take_forks(p);
		eat(p);
		put_down_forks(p);
		if (p->args.num_of_must_eat && p->eat_count >= p->args.num_of_must_eat)
			break ; // 철학자가 먹어야 할 횟수가 0이 아니고, 철학자가 먹은 횟수가 먹어야 할 횟수보다 크거나 같을 때
		print_status("is sleeping", p); //usleep(p->args.time_to_sleep * 1000);
		time_lapse(p->args.time_to_sleep);
		print_status("is thinking", p);
	}
	return (NULL);
}

void	monitor_death(t_philo *p, unsigned int i)
{
	long cur_time;

	pthread_mutex_lock(&p[i].resrcs->last_meal_time); // 철학자의 마지막 식사 시간에 대한 접근을 동기화하기 위해 뮤텍스를 잠급니다.
	cur_time = get_time();
	if (!p[i].resrcs->alive_stat || cur_time - p[i].last_meal_time > p[i].args.time_to_die)
	{// 철학자가 죽었거나, 현재 시간과 마지막 식사 시간의 차이가 time_to_die를 초과했다면
		p[i].resrcs->alive_stat = 0; // 철학자가 죽었음을 표시합니다.
		print_status("died", &p[i]); // 철학자가 죽었음을 출력합니다.
		exit(0); // 철학자가 죽었으므로 프로그램을 종료합니다.
	}
	pthread_mutex_unlock(&p[i].resrcs->last_meal_time); // 철학자의 마지막 식사 시간에 대한 뮤텍스를 해제
}

void	monitor_eating(t_philo *p, unsigned int i, unsigned int *fin_eating)
{
	pthread_mutex_lock(&p[i].resrcs->full); // 철학자의 식사 상태에 대한 접근을 동기화하기 위해 뮤텍스를 잠급니다.
	if (p[i].eat_count >= p[0].args.num_of_must_eat) // 각 철학자가 필요한 만큼 식사했는지 확인
		(*fin_eating)++; // 필요한 만큼 식사한 철학자의 수를 증가
	pthread_mutex_unlock(&p[i].resrcs->full); // 철학자의 식사 상태에 대한 뮤텍스를 해제
}

void	monitor(t_philo *p)
{
	unsigned int	i;
	unsigned int	fin_eating;

	while (1) // 철학자들의 죽음 상태와 식사 상태를 지속적으로 감시
	{
		i = 0;
		fin_eating = 0;
		while (i < p[0].args.num_of_philo)
		{
			monitor_death(p, i);
			monitor_eating(p, i, &fin_eating);
			i++;
		}
		if (fin_eating == p[0].args.num_of_philo) // 모든 철학자가 필요한 만큼 식사를 완료했다면
			exit(0);
		time_lapse(1); // 철학자의 상태 체크 간에 잠시 멈춤
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

void    time_lapse(unsigned int time)
{
	unsigned int    start_time;

	start_time = get_time();
	while (get_time() < start_time + (unsigned int)time)
	{
		usleep(50);
	}
}
