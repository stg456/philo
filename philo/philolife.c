/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philolife.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stgerard <stgerard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 16:26:24 by stgerard          #+#    #+#             */
/*   Updated: 2022/12/07 17:29:59 by stgerard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *philo, size_t id)
{
	pthread_mutex_lock(&philo->forks[id - 1]);
	pthread_mutex_lock(&philo->writing);
	printf("\x1B[34m%lld philo %zu ", diff_chrono(*philo), id);
	printf("has taken a fork\n\x1B[0m");
	pthread_mutex_unlock(&philo->writing);
	pthread_mutex_lock(&philo->forks[id % philo->rules.nb_philo]);
	pthread_mutex_lock(&philo->writing);
	printf("\x1B[34m%lld philo %zu ", diff_chrono(*philo), id);
	printf("has taken a fork\n\x1B[0m");
	pthread_mutex_unlock(&philo->writing);
	philo->lunch_time[id - 1] = diff_chrono(*philo);
	pthread_mutex_lock(&philo->writing);
	printf("\x1B[32m%lld philo %zu ", diff_chrono(*philo), id);
	printf("is eating\n\x1B[0m");
	pthread_mutex_unlock(&philo->writing);
	ft_wait(philo->rules.time_eat);
	pthread_mutex_unlock(&philo->forks[id - 1]);
	pthread_mutex_unlock(&philo->forks[id % philo->rules.nb_philo]);
}

void	sleeping(t_philo *philo, size_t id)
{
	if (philo->rules.dead == 0)
	{
		pthread_mutex_lock(&philo->writing);
		printf("\x1B[33m%lld philo %zu ", diff_chrono(*philo), id);
		printf("is sleeping\n\x1B[0m");
		pthread_mutex_unlock(&philo->writing);
	}
	ft_wait(philo->rules.time_sleep);
}

int	ft_print(t_philo *philo, int msg, int id)
{
	pthread_mutex_lock(&philo->writing);
	if (msg == 0)
		printf("\x1B[34m%lld philo %i has taken a fork\n\x1B[0m",
			diff_chrono(*philo), id);
	else if (msg == 1)
		printf("\x1B[32m%lld philo %i is eating\n\x1B[0m",
			diff_chrono(*philo), id);
	else if (msg == 2)
		printf("\x1B[33m%lld philo %i is sleeping\n\x1B[0m",
			diff_chrono(*philo), id);
	else if (msg == 3)
		printf("\x1B[36m%lld philo %i is thinking\n\x1B[0m",
			diff_chrono(*philo), id);
	else if (msg == 4)
		printf("\x1B[31m%lld philo %d died\n\x1B[0m",
			diff_chrono(*philo), id);
	pthread_mutex_unlock(&philo->writing);
	return (0);
}
