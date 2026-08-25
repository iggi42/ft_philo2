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
#include "philo_types.h"
#include "frk.h"
#include "logging.h"
#include "time.h"
#include <unistd.h>

void	set_last_meal2now(t_philo *p)
{
	if (p == NULL || pthread_mutex_lock(&p->last_meal_mutex))
		return ;
	p->last_meal = read_timer();
	pthread_mutex_unlock(&p->last_meal_mutex);
}

// IDEA CHECK WITH TIMER OVER RANGE OF DEATH 
t_timespan	time_since_last_meal(t_philo *p)
{
	t_timespan	last_meal;
	t_timespan	now;

	now = read_timer();
	if (pthread_mutex_lock(&p->last_meal_mutex))
		return (-1);
	last_meal = p->last_meal;
	if (pthread_mutex_unlock(&p->last_meal_mutex))
		return (-1);
	return (now - last_meal);
}

// returns true if it has slept the full time
// returns false if philo should abort
static bool philo_sleep(t_timespan t)
{
	usleep(t * 1000);
	return true;
}

// returns true if it has slept the full time
// returns false if philo should abort
// logging is included as side effect (sleeping log at start, thinking log afterwards)
static bool philo_routine_sleep(t_philo *thinker)
{
	if(!log_queue(log_sleeping, thinker))
		return false;
	// IDEA: wait only a fracation at once and check death watch and go back to sleep
	// might be necessary to use timer values
	philo_sleep(thinker->c->t2nap);
	return log_queue(log_thinking, thinker);
}

// returns true if it has eaten
// returns false if philo should abort
// logging is included as side effect
static bool philo_routine_eating(t_philo *me)
{
	t_frk	*fs[2];
	bool	has_eaten;

	has_eaten = false;
	fs[me->id % 2] = me->right;
	fs[(me->id + 1) % 2] = me->left;
	while (true)
	{
		if (pickup(fs[0]))
		{
			if (!log_queue(log_forklift, me))
				return false;
			if (pickup(fs[1]))
			{
				if(!log_queue(log_forklift, me))
					return false;
				set_last_meal2now(me);
				has_eaten = true;
				if(!log_queue(log_eating, me))
					return false;

				putdown(fs[1]);
			}
			putdown(fs[0]);
		}
		if (has_eaten)
			return true;
		usleep(1000);
	}
}


void	*philo_routine_maxmeals(void *s)
{
	t_philo *me;
	int meals;

	me = s;
	meals = 0;
	while (meals < me->c->max_meals)
	{
		if(!philo_routine_eating(me))
			break;
		meals++;
		if(!philo_routine_sleep(me))
			break;
	}

	return NULL;
}

void	*philo_routine_endless(void *s)
{
	t_philo *me;

	me = s;
	while (42)
	{
		if(!philo_routine_eating(me))
			break;
		if(!philo_routine_sleep(me))
			break;
	}

	return NULL;
}
