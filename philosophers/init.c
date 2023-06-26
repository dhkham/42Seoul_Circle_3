/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:12:26 by dkham             #+#    #+#             */
/*   Updated: 2023/06/26 21:41:55 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	parse_arguments(int argc, char **argv, t_args *args)
{
	if (argc < 5 || argc > 6)
	{
		printf("wrong number of arguments\n");
		exit (1);
	}
	args->num_of_philo = ft_atoi(argv[1]); // 철학자의 수
	args->time_to_die = ft_atoi(argv[2]); // 철학자가 죽을 때까지의 시간
	args->time_to_eat = ft_atoi(argv[3]); // 철학자가 식사하는 시간
	args->time_to_sleep = ft_atoi(argv[4]); // 철학자가 잠을 자는 시간
	if (argc == 6)
		args->num_of_must_eat = ft_atoi(argv[5]); // 철학자가 식사해야 하는 횟수
	else
		args->num_of_must_eat = 0; // 주어지지 않았다면 0으로 설정
	if (args->num_of_philo <= 0 || args->time_to_die <= 0 || \
	args->time_to_eat <= 0 || args->time_to_sleep <= 0 || \
	(argc == 6 && args->num_of_must_eat < 0))
	{
		printf("Error: All arguments must be positive integers.\n");
		exit(1);
	}
}

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
	resrcs->full_stat = 0;
	resrcs->alive_stat = 1;
}

void	init_philo_and_run(t_philo *philo, t_resrcs *resrcs, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->num_of_philo)
	{
		init_philosopher(&philo[i], resrcs, args, i);
		pthread_create(&philo[i].threads, NULL, philosopher, &philo[i]);
		i++;
	}
}

// 특정 철학자에 대한 정보를 초기화하는 함수
void	init_philosopher(t_philo *p, t_resrcs *r, t_args *a, int i)
{
	p->resrcs = r; // 공유 자원의 주소를 철학자 구조체에 복사합니다.
	p->args = *a; // 철학자의 인수 정보를 복사합니다.
	p->last_meal_time = r->start_time; // 철학자의 마지막 식사 시간을 시작 시간으로 초기화합니다.
	p->id = i + 1; // 철학자의 ID를 설정: 1번 철학자부터 시작
	p->left_fork = i; // 철학자의 왼쪽 포크를 설정: 0번 포크부터 시작
	if (i == a->num_of_philo - 1)
		p->right_fork = 0; // 마지막 철학자의 경우, 오른쪽 포크는 0번 포크입니다.
	else
		p->right_fork = i + 1; // 그 외 철학자의 경우, 오른쪽 포크는 자신의 ID와 같습니다.
	p->eat_count = 0; // 철학자의 식사 횟수를 0으로 초기화합니다.
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
