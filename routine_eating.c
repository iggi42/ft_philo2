/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_eating.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 20:06:36 by fkruger           #+#    #+#             */
/*   Updated: 2026/08/26 20:06:37 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_types.h"
#include "logging.h"
#include "meal.h"
#include <unistd.h>

static void	ft_switch(t_frk **a, t_frk **b)
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

// returns true if it has eaten
// returns false if philo should abort
// logging is included as side effect
bool	philo_routine_eating(t_philo *me)
{
	t_frk	*fs[2];
	bool	has_eaten;

	has_eaten = false;
	fs[0] = me->left;
	fs[1] = me->right;
	if (me->id == 1)
		ft_switch(&fs[0], &fs[1]);
	if ((me->c->n_phil % 2 && (me->id % 2) == 0))
		ft_switch(&fs[0], &fs[1]);
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
			usleep(me->c->t2eat * 1000);
			// if (!philo_sleep(me->c->t2eat))
			// 	return (philo_put_down(fs), false);
			putdown(fs[1]);
		}
		putdown(fs[0]);
	}
	return (has_eaten);
}
