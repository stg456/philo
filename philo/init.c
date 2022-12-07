/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stgerard <stgerard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 13:03:28 by stgerard          #+#    #+#             */
/*   Updated: 2022/12/07 18:15:45 by stgerard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_arg(char **argv, int argc, t_philo *philo)
{
	size_t	i;
	t_rules	*rules;

	i = 0;
	rules = &philo->rules;
	if (ft_isdigit_str(argv[1]) || ft_isdigit_str(argv[2])
		|| ft_isdigit_str(argv[3]) || ft_isdigit_str(argv[4]))
		return (1);
	if (rules->nb_philo < 0 || rules->time_die < 0 || rules->time_eat < 0
		|| rules->time_sleep < 0)
		return (1);
	if (argc == 6 && (ft_isdigit_str(argv[5]) || rules->nb_eat < 0))
		return (1);
	philo->rules.nb_philo = ft_atoi(argv[1]);
	philo->rules.time_die = ft_atoi(argv[2]);
	philo->rules.time_eat = ft_atoi(argv[3]);
	philo->rules.time_sleep = ft_atoi(argv[4]);
	philo->rules.eat_mode = 0;
	if (argc == 6)
	{
		philo->rules.nb_eat = ft_atoi(argv[5]);
		philo->rules.eat_mode = 1;
	}
	return (0);
}

int	init_arg(int argc, char **argv, t_philo *philo)
{
	philo->id = 1;
	if (argc < 5 || argc > 6)
	{
		ft_error("Error : Bad numbers of arguments.");
		return (1);
	}
	if (check_arg(argv, argc, philo))
	{
		ft_error("Error : Invalid arguments.");
		return (1);
	}
	philo->rules.runtime = timestamp();
	philo->rules.dead = 0;
	if (philo->rules.nb_philo == 1)
		philo->rules.dead = 1;
	philo->lunch_time = malloc(sizeof(int) * philo->rules.nb_philo);
	if (philo->lunch_time == NULL)
		return (1);
	memset(philo->lunch_time, 0, sizeof(int) * philo->rules.nb_philo);
	return (0);
}

int	init_mutex(t_philo *philo)
{
	size_t	i;

	i = 0;
	philo->forks = malloc(sizeof(pthread_mutex_t) * philo->rules.nb_philo);
	pthread_mutex_init(&philo->writing, NULL);
	pthread_mutex_init(&philo->dead, NULL);
	pthread_mutex_lock(&philo->dead);
	if (philo->forks == NULL)
	{
		printf("Mutex malloc error\n");
		return (1);
	}
	while (i < philo->rules.nb_philo)
	{
		pthread_mutex_init(&philo->forks[i], NULL);
		++i;
	}
	return (0);
}

int	init_thread(t_philo *philo)
{
	size_t	i;

	i = 0;
	philo->threads = malloc(sizeof(pthread_t) * philo->rules.nb_philo);
	if (philo->threads == NULL)
	{
		printf("Pthread malloc error\n");
		return (1);
	}
	pthread_create(&philo->police, 0, check_end, philo);
	pthread_detach(philo->police);
	while (i < philo->rules.nb_philo)
	{
		pthread_create(&philo->threads[i], 0, gestphilo, philo);
		pthread_detach(philo->threads[i]);
		if (philo->rules.nb_philo == 1)
			solitude(philo);
		if ((philo->rules.nb_philo % 2 == 1
				&& i == philo->rules.nb_philo - 1)
			|| i % 2 == 0)
			usleep(300);
		++i;
	}
	return (0);
}
