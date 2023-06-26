/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:31 by dkham             #+#    #+#             */
/*   Updated: 2023/06/26 21:48:50 by dkham            ###   ########.fr       */
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
	long long		start_time; // when the program start
	int				full_stat; // how many philosophers are full
	int				alive_stat; // 0 = dead, 1 = alive
}	t_resrcs;

typedef struct s_args_info
{
	int	num_of_philo; // number of philosophers
	int	time_to_die; // time to die in ms
	int	time_to_eat; // time to eat in ms
	int	time_to_sleep; // time to sleep in ms
	int	num_of_must_eat; // number of time each philosopher must eat
}	t_args;

typedef struct s_philosopher
{
	pthread_t		threads; // threads
	t_resrcs		*resrcs; // mutexes
	t_args			args; // arguments
	long long		last_meal_time; // last time philosopher ate (finish time)
	int				id; // philosopher id
	int				left_fork; // fork id
	int				right_fork; // fork id: left_fork + 1
	int	eat_count; // how many time the philosopher ate
}	t_philo;

void	parse_arguments(int argc, char **argv, t_args *args);
void	init_resrcs(t_resrcs *resrcs, t_args *args);
void	init_philo_and_run(t_philo *philo, t_resrcs *resrcs, t_args *args);
void	init_philosopher(t_philo *p, t_resrcs *r, t_args *a, int i);
void	take_forks(t_philo *p);
void	eat(t_philo *p);
void	put_down_forks(t_philo *p);
void	*philosopher(void *args);
void	monitor(t_philo *p);
void	monitor_death(t_philo *p, int i);
void	monitor_eating(t_philo *p, int i, int *fin_eating);
void	destroy_resources(t_resrcs *resrcs, t_args *args);
void	print_status(char *status, t_philo *p);
long	get_time(void);
void    time_lapse(unsigned int time);
int		ft_atoi(const char *str);
char	*ft_strchr(const char *s, int c);