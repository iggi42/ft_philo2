/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 20:28:57 by fkruger           #+#    #+#             */
/*   Updated: 2026/08/23 20:29:00 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "frk.h"
#include "logging.h"
#include "philo_types.h"
#include "time.h"
#include <stdbool.h>
#include <unistd.h>

// returns true if it worked as expected
// returns false if the philo should abort
bool	set_last_meal2now(t_philo *p)
{
	bool	result;

	if (p == NULL || pthread_mutex_lock(&p->last_meal_mutex))
		return (false);
	result = p->last_meal != -1;
	if (result)
		p->last_meal = read_timer();
	pthread_mutex_unlock(&p->last_meal_mutex);
	return (result);
}

void	set_last_meal2off(t_philo *p)
{
	if (p == NULL || pthread_mutex_lock(&p->last_meal_mutex))
		return ;
	p->last_meal = -1;
	pthread_mutex_unlock(&p->last_meal_mutex);
}

bool	is_last_meal2on(t_philo *p)
{
	bool	result;

	if (p == NULL || pthread_mutex_lock(&p->last_meal_mutex))
		return (false);
	result = p->last_meal == -1;
	if (!pthread_mutex_unlock(&p->last_meal_mutex))
		return (false);
	return (result);
}

// IDEA: wait only a fracation at once and check death watch and go back to sleep
// might be necessary to use timer values
// returns true if it has slept the full time
// returns false if philo should abort
static bool	philo_sleep(t_timespan t)
{
	usleep(t * 1000);
	return (true);
}

// returns true if it has slept the full time
// returns false if philo should abort
// logging is included as side effect (sleeping log at start,
// thinking log afterwards)
static bool	philo_routine_sleep(t_philo *thinker)
{
	if (!log_queue(log_sleeping, thinker))
		return (false);
	if (!philo_sleep(thinker->c->t2nap))
		return (false);
	return (log_queue(log_thinking, thinker));
}

void	ft_switch(t_frk **a, t_frk **b)
{
	void	*c;

	c = *a;
	*a = *b;
	*b = c;
}

static void	philo_put_down(t_frk *fs[2])
{
	putdown(fs[0]);
	putdown(fs[1]);
}
#include <stdio.h>

// TODO: REWRITE ME for the forks as mutex
// returns true if it has eaten
// returns false if philo should abort
// logging is included as side effect
static bool	philo_routine_eating(t_philo *me)
{
	t_frk	*fs[2];
	bool	has_eaten;

	has_eaten = false;
	fs[0] = me->left;
	fs[1] = me->right;
	// if(me->right->id < me->left->id)
	// 	ft_switch(&fs[0], &fs[1]);
	if (pickup(fs[0]))
	{
		if (!log_queue(log_forklift, me) || (fs[0] == fs[1]))
			return (putdown(fs[0]), false);
		if (pickup(fs[1]))
		{
			if (!log_queue(log_forklift, me))
				return (philo_put_down(fs), false);
			if (!log_queue(log_eating, me))
				return (philo_put_down(fs), false);
			if (!set_last_meal2now(me))
				return (philo_put_down(fs), false);
			has_eaten = true;
			if (!philo_sleep(me->c->t2eat))
				return (philo_put_down(fs), false);
			putdown(fs[1]);
		}
		putdown(fs[0]);
	}
	return (has_eaten);
}

void	*philo_routine_maxmeals(void *s)
{
	t_philo	*me;
	int		meals;

	me = s;
	meals = 0;
	while (true)
	{
		if (!philo_routine_eating(me))
			return (NULL);
		if (++meals >= me->c->max_meals)
			break ;
		if (!philo_routine_sleep(me))
			return (NULL);
	}
	set_last_meal2off(me);
	return (NULL);
}

void	*philo_routine_endless(void *s)
{
	t_philo	*me;

	me = s;
	while (true)
	{
		if (!philo_routine_eating(me))
			break ;
		if (!philo_routine_sleep(me))
			break ;
	}
	return (NULL);
}
