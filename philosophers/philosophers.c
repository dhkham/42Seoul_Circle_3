/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:15 by dkham             #+#    #+#             */
/*   Updated: 2023/06/27 22:52:19 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// visualizer 이용하기
// time_lapse 함수 제대로 작동하는 것 맞는지 확인 필요

// time_lapse 함수 써서 쪼개기
// (검사 할 경우 sanitize지우고 하기)
// must_eat 입력되는 경우 데드락 발생
// 전반적인 구현 방식 체크

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
	if (p->id % 2 == 0)
		usleep(3000);
	while (1)
	{
		// 죽음 체크
		// 만약 alive_stat = 0이면 break, 뮤텍스 걸기
		if (take_forks(p) == 1)
			return (NULL);
		if (eat(p) == 1)
			return (NULL);
		if (put_down_forks(p) == 1)
			return (NULL);
		print_status("is sleeping", p);
		//time_lapse(p->args.time_to_sleep);
		usleep(p->args.time_to_sleep * 1000);
		print_status("is thinking", p);
	}
	return (NULL);
}

void	monitor(t_philo *p)
{
	int	i;

	while (1) // 철학자들의 죽음 상태와 식사 상태를 지속적으로 감시
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
		//time_lapse(1); // 철학자의 상태 체크 간에 잠시 멈춤 //
		//usleep(1000);
	}
}

int	monitor_death(t_philo *p, int i)
{
	long	cur_time;

	pthread_mutex_lock(&p[i].resrcs->last_meal_time); // 철학자의 마지막 식사 시간에 대한 접근을 동기화하기 위해 뮤텍스를 잠급니다.
	pthread_mutex_lock(&p[i].resrcs->alive); 
	cur_time = get_time();
	if (!p[i].resrcs->fin_stat || cur_time - p[i].last_meal_time > p[i].args.time_to_die)
	{// 철학자가 죽었거나, 현재 시간과 마지막 식사 시간의 차이가 time_to_die를 초과했다면
		p[i].resrcs->fin_stat = 0; // 철학자가 죽었음을 표시합니다.
		pthread_mutex_unlock(&p[i].resrcs->last_meal_time); // 철학자의 마지막 식사 시간에 대한 뮤텍스를 해제
		pthread_mutex_unlock(&p[i].resrcs->alive);
		printf("%lld %d %s\n", (get_time() - p->resrcs->start_time), p->id, "died");
		// print_status("died", &p[i]); // 철학자가 죽었음을 출력합니다. //exit(0); // 철학자가 죽었으므로 프로그램을 종료합니다.
		return (1);
	}        
	pthread_mutex_unlock(&p[i].resrcs->last_meal_time); // 철학자의 마지막 식사 시간에 대한 뮤텍스를 해제
	pthread_mutex_unlock(&p[i].resrcs->alive);
	return (0);
}

int	monitor_eating(t_philo *p, int i)
{
	pthread_mutex_lock(&p[i].resrcs->full); // 철학자의 식사 상태에 대한 접근을 동기화하기 위해 뮤텍스를 잠급니다.
	// if (p[i].eat_count >= p[0].args.num_of_must_eat) // 각 철학자가 필요한 만큼 식사했는지 확인
	// 	(*fin_eating)++; // 필요한 만큼 식사한 철학자의 수를 증가
	if (p[i].args.num_of_philo == p[i].resrcs->full_stat)
	{
		pthread_mutex_lock(&p[i].resrcs->alive); 
		p[i].resrcs->fin_stat = 0;
		pthread_mutex_unlock(&p[i].resrcs->alive);
		return (1);
	}
	pthread_mutex_unlock(&p[i].resrcs->full); // 철학자의 식사 상태에 대한 뮤텍스를 해제
	return (0);
	// if (*fin_eating == p[0].args.num_of_philo) // 모든 철학자가 필요한 만큼 식사를 완료했다면// 	exit(0); // 프로그램을 종료합니다.
}

void	print_status(char *status, t_philo *p)
{
	pthread_mutex_lock(&p->resrcs->alive);
	pthread_mutex_lock(&p->resrcs->print_mutex); // 출력을 동기화하기 위해 뮤텍스를 잠급니다.
	if (!p->resrcs->fin_stat)
	{
		pthread_mutex_unlock(&p->resrcs->alive);
		pthread_mutex_unlock(&p->resrcs->print_mutex); // 출력을 동기화하기 위해 뮤텍스를 잠급니다.
		return ;
	}
	else
		printf("%lld %d %s\n", (get_time() - p->resrcs->start_time), p->id, status); // 철학자의 상태를 출력합니다. 출력되는 시간은 프로그램 시작 시간으로부터의 상대적인 시간입니다.
	pthread_mutex_unlock(&p->resrcs->alive);
	pthread_mutex_unlock(&p->resrcs->print_mutex); // 출력이 끝나면 뮤텍스를 해제합니다.
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL); // 현재 시간을 가져옵니다.
	return ((tv.tv_sec * (long)1000) + (tv.tv_usec / 1000)); // 현재 시간을 밀리초 단위로
}

// void    time_lapse(long time)
// {
// 	long	start_time;

// 	start_time = get_time();
// 	while (get_time() < start_time + (long)time)
// 	{
// 		usleep(50);
// 	}
// }
void    time_lapse(unsigned int time)
{
	unsigned int	start_time;

	start_time = get_time();
	while (get_time() < start_time + (unsigned int)time)
	{
		usleep(50);
	}
}
