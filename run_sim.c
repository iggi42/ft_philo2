/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_sim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 03:39:12 by fkruger           #+#    #+#             */
/*   Updated: 2026/07/14 20:51:11 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "frk.h"
#include "logging.h"
#include "philo.h"
#include "time.h"
#include "utils.h"
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static t_frk	*bring_the_cutlery(size_t n)
{
	t_frk	*result;
	size_t	i;

	i = 0;
	result = ft_calloc(n, sizeof(t_frk));
	while (result != NULL && i < n)
		if (!init_frk(&result[i++]))
			result = (free(result), NULL);
	return (result);
}

static void	init_thinker(t_philo *fresh, size_t i, t_philo_conf *c,
		t_frk *cutlery)
{
	fresh->id = i + 1;
	fresh->c = c;
	fresh->last_meal = 0;
	fresh->left = &cutlery[i];
	fresh->right = &cutlery[(i + 1) % c->n_phil];
}

static void	cleanup_philos(t_philo *philos, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		pthread_mutex_destroy(&philos[i++].last_meal_mutex);
	free(philos);
}

static void	cleanup_forks(t_frk *cutlery, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		destroy_frk(&cutlery[i++]);
	free(cutlery);
}

static t_philo	*create_thinkers(t_philo_conf *c, t_frk *cutlery)
{
	t_philo	*thinkers;
	size_t	i;

	i = 0;
	thinkers = ft_calloc(c->n_phil, sizeof(t_philo));
	if (thinkers == NULL || cutlery == NULL)
		return (NULL);
	while (i < c->n_phil)
	{
		init_thinker(&thinkers[i], i, c, cutlery);
		if (pthread_mutex_init(&(thinkers[i].last_meal_mutex), NULL))
			return (cleanup_philos(thinkers, i), NULL);
		i++;
	}
	return (thinkers);
}

static pthread_t	*start_thinkers(t_philo_conf *c, t_philo *philo)
{
	pthread_t	*thread_ids;
	size_t		i;

	void		*(*routine)(void *);
	thread_ids = ft_calloc(c->n_phil, sizeof(pthread_t));
	i = 0;
	if (thread_ids == NULL || philo == NULL)
		return (NULL);
	if (c->max_meals < 0)
		routine = philo_routine_endless;
	else
		routine = philo_routine_maxmeals;
	log_queue(NULL, NULL);
	start_timer();
	while (i < c->n_phil)
	{
		if (pthread_create(&thread_ids[i], NULL, routine, &philo[i]))
			perror("pthread_create failed");
		usleep(100);
		i++;
	}
	return (thread_ids);
}

bool	has_starved(t_philo *p)
{
	bool result;

	if (pthread_mutex_lock(&p->last_meal_mutex))
		return (false);
	result = (p->last_meal != -1) && ((read_timer() - p->last_meal) >= p->c->t2die);
	if(result && log_queue(log_died, p))
		p->last_meal = -1;
	if (pthread_mutex_unlock(&p->last_meal_mutex))
		return (false);
	return (result);
}

static void	ft_phil_void(t_philo *p)
{
	(void)p;
}

//TODO: return if all philos have meal->meal = -1
static void	find_starved(t_philo_conf *c, t_philo *philo)
{
	size_t	i;

	i = 0;
	while (log_queue(ft_phil_void, &philo[i]))
	{
		if(has_starved(&philo[i]))
			return;
		i = (i + 1) % c->n_phil;
		usleep(400);
	}
}

static void	wait4thinkers(pthread_t *ids, size_t n)
{
	size_t	i;

	if (ids == NULL)
		return ;
	i = 0;
	while (i < n)
		if (pthread_join(ids[i++], NULL))
			perror("pthread_join failed");
	free(ids);
}

bool	run_sim(t_philo_conf *c)
{
	t_frk		*frks;
	t_philo		*thinkers;
	pthread_t	*pthr_id;

	frks = bring_the_cutlery(c->n_phil);
	thinkers = create_thinkers(c, frks);
	pthr_id = start_thinkers(c, thinkers);
	find_starved(c, thinkers);
	wait4thinkers(pthr_id, c->n_phil);
	cleanup_philos(thinkers, c->n_phil);
	cleanup_forks(frks, c->n_phil);
	return (true);
}
