/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkham <dkham@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:11:31 by dkham             #+#    #+#             */
/*   Updated: 2023/06/29 18:56:38 by dkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_resources
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	last_meal_time;
	pthread_mutex_t	full;
	pthread_mutex_t	alive;
	pthread_mutex_t	print_mutex;
	int				*forks_stat;
	long long		start_time;
	int				full_count;
	int				alive_stat;
}	t_resrcs;

typedef struct s_args_info
{
	int	num_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	num_of_must_eat;
}	t_args;

typedef struct s_philosopher
{
	pthread_t		threads;
	t_resrcs		*resrcs;
	t_args			args;
	long long		last_meal_time;
	int				id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
}	t_philo;

void	parse_arguments(int argc, char **argv, t_args *args);
void	init_resrcs(t_resrcs *resrcs, t_args *args);
void	init_philo_and_run(t_philo *philo, t_resrcs *resrcs, t_args *args);
void	init_philosopher(t_philo *p, t_resrcs *r, t_args *a, int i);
int		take_forks(t_philo *p);
int		eat(t_philo *p);
int		put_down_forks(t_philo *p);
void	*philosopher(void *args);
void	monitor(t_philo *p);
int		monitor_death(t_philo *p, int i);
int		monitor_eating(t_philo *p, int i);
void	destroy_resources(t_resrcs *resrcs, t_args *args);
void	print_status(char *status, t_philo *p);
long	get_time(void);
void	ft_usleep(long long time);
int		ft_atoi(const char *str);
char	*ft_strchr(const char *s, int c);
void	*philo_one(void *args);

#endif