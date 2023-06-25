/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:31 by dkham             #+#    #+#             */
/*   Updated: 2023/06/25 17:11:48 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_resources
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	last_meal_time;
	pthread_mutex_t	full;
	pthread_mutex_t	alive;
	pthread_mutex_t	print_mutex;
	int				*forks_stat; // for each fork: 0 = unavailable, 1 = available (int array)
	int				start_time; // when the program start
	int				full_stat; // how many philosophers are full
	int				alive_stat; // 0 = dead, 1 = alive
}	t_resrcs;

typedef struct s_args_info
{
	unsigned int	num_of_philo; // number of philosophers
	unsigned int	time_to_die; // time to die in ms
	unsigned int	time_to_eat; // time to eat in ms
	unsigned int	time_to_sleep; // time to sleep in ms
	unsigned int	num_of_must_eat; // number of time each philosopher must eat
}	t_args;

typedef struct s_philosopher
{
	pthread_t		threads; // threads
	t_resrcs		*resrcs; // mutexes
	t_args			args; // arguments
	unsigned int	last_meal_time; // last time the philosopher ate (finish time)
	int				id; // philosopher id
	int				left_fork; // fork id
	int				right_fork; // fork id: left_fork + 1
	unsigned int	eat_count; // how many time the philosopher ate
}	t_philo;

// eat count = num_of_must_eat일 때 full count 증가
// full count = num_of_philo일 때 모든 철학자가 full이므로 프로그램 종료
// num_of_must_eat는 optional argument이므로 없을 경우 full_count가 무의미함

// 먹는 행위는 usleep으로 구현
void	parse_arguments(int argc, char **argv, t_args *args);
void	init_resrcs(t_resrcs *resrcs, t_args *args);
void	init_philo_and_run(t_philo *philo, t_resrcs *resrcs, t_args *args);
void	init_philosopher(t_philo *p, t_resrcs *r, t_args *a, int i);
void	take_forks(t_philo *p);
void	eat(t_philo *p);
void	put_down_forks(t_philo *p);
void	*philosopher(void *args);
void	*monitor(void *args);
void	destroy_resources(t_resrcs *resrcs, t_args *args);
void	print_status(char *status, t_philo *p);
long	get_time(void);