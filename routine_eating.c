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

#include "frk.h"
#include "logging.h"
#include "meal.h"
#include "philo_types.h"
#include <stdbool.h>
#include <unistd.h>

void	ft_switch(t_frk **a, t_frk **b)
{
	void	*c;

	c = *a;
	*a = *b;
	*b = c;
}

static void	philo_put_down(t_frk *fs[2], t_philo_id id)
{
	putdown(fs[0], id);
	putdown(fs[1], id);
}

// this decides which fork to take up first and which secound
void	sort_cutlery(t_philo *me, t_frk *(*target)[2])
{
	(*target)[0] = me->left;
	(*target)[1] = me->right;
	if (me->c->n_phil % 2 == 0)
	{
		if (me->id % 2 == 0)
			ft_switch(&(*target)[0], &(*target)[1]);
		return ;
	}
}

// bool	sync_takeup(unsigned char id, t_frk *f0, t_frk *f1)
bool static	philo_special_boy(t_philo *me)
{
	while (!sync_takeup(me->id, me->left, me->right))
		usleep(200);
	return false;
}

// returns true if it has eaten
// returns false if philo should abort
// logging is included as side effect
bool	philo_routine_eating(t_philo *me)
{
	t_frk	*fs[2];
	bool	has_eaten;

	if ((me->c->n_phil % 2) == 1)
		return (philo_special_boy(me));
	fs[0] = me->left;
	fs[1] = me->right;
	has_eaten = false;
	if (me->id % 2 == 0)
		ft_switch(&fs[0], &fs[1]);
	if (pickup(fs[0], me->id))
	{
		if (!log_queue(log_forklift, me) || (fs[0] == fs[1]))
			return (putdown(fs[0], me->id), false);
		if (pickup(fs[1], me->id))
		{
			if (!log_queue(log_forklift, me) || !log_queue(log_eating, me)
				|| !set_last_meal2now(me))
				return (philo_put_down(fs, me->id), false);
			has_eaten = true;
			usleep(me->c->t2eat * 1000);
			putdown(fs[1], me->id);
		}
		putdown(fs[0], me->id);
	}
	return (has_eaten);
}
