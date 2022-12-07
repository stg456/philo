/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stgerard <stgerard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 10:55:56 by stgerard          #+#    #+#             */
/*   Updated: 2022/12/07 17:10:27 by stgerard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_wait(long long delay)
{
	long long	t;
	long long	start;

	t = timestamp();
	start = t;
	while (t != start + delay)
		t = timestamp();
}

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

long long	diff_chrono(t_philo philo)
{
	struct timeval	t;
	long long		ms;

	gettimeofday(&t, NULL);
	ms = (t.tv_sec * 1000 + t.tv_usec / 1000);
	return ((ms - philo.rules.runtime));
}

void	closephilo(t_philo *philo)
{
	size_t	i;
	size_t	j;

	j = 0;
	while (j < philo->rules.nb_philo)
	{
		pthread_join(philo->threads[j], NULL);
		j++;
	}
	i = 0;
	while (i < philo->rules.nb_philo)
	{
		pthread_mutex_destroy(&philo->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&philo->writing);
	pthread_mutex_destroy(&philo->dead);
	free(philo->forks);
	free(philo->threads);
	free(philo->lunch_time);
	free(philo);
}
