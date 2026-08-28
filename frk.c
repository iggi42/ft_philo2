/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frk.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 04:02:18 by fkruger           #+#    #+#             */
/*   Updated: 2026/07/13 05:52:20 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "frk.h"
#include "pthread.h"
#include <stdbool.h>
#include <stdio.h>

// taken is true if the fork is in use
bool	init_frk(t_frk *frk, unsigned char id)
{
	if (!frk)
		return (false);
	frk->id = id;
	frk->taken = false;
	return (pthread_mutex_init(&frk->taken_by_mtx, NULL) == 0);
}

bool	destroy_frk(t_frk *frk)
{
	if (!frk)
		return (false);
	return (pthread_mutex_destroy(&frk->taken_by_mtx) != 0);
}


// this needs to seperate error case and return val
// error case is also the "we need to stop early because another philo died" case
bool	sync_takeup(unsigned char id, t_frk *f0, t_frk *f1)
{
	static pthread_mutex_t	local_mut = PTHREAD_MUTEX_INITIALIZER;
	bool					result;

	result = false;
	if (!f0 || !f1)
		return (false);
	if (f0->id > f1->id)
		return (sync_takeup(id, f1, f0));
	if (!pthread_mutex_lock(&local_mut))
		return (false);
	if (pthread_mutex_lock(&f0->taken_mtx) == 0)
	{
		if(pthread_mutex_lock(&f1->taken_mtx) == 0)
		{
			result = (!f0->taken && !f1->taken);
			if (result)
			{
				//TODO: handle if pickup returns false on mutex error, then put down forks again
				result = pickup(f0, id);
				if(result)
					result = pickup(f1, id);
			}
		}
		pthread_mutex_unlock(&f0->taken_mtx);
	}

	if (!pthread_mutex_unlock(&local_mut))
		return (false);
	return (result);
}

bool	pickup(t_frk *frk, unsigned char phil_id)
{
	if (pthread_mutex_lock(&frk->taken_by_mtx))
		return (false);
	frk->taken_by = phil_id;
	return (true);
}

bool	putdown(t_frk *frk, unsigned char phil_id)
{
	if (phil_id != frk->taken_by)
		return (printf("WTF BRO\n"), false);
	frk->taken_by = 0;
	frk->taken = false;
	return (!pthread_mutex_unlock(&frk->taken_mtx));
}
