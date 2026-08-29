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

// val is true if the fork is in use
bool	init_frk(t_frk *frk, unsigned char id)
{
	if (!frk)
		return (false);
	frk->id = id;
	frk->taken = false;
	if (pthread_mutex_init(&frk->read_mtx, NULL) == 0)
	{
		if (pthread_mutex_init(&frk->write_mtx, NULL) == 0)
			return (true);
		pthread_mutex_destroy(&frk->read_mtx);
	}
	return (false);
}

bool	destroy_frk(t_frk *frk)
{
	bool	a;

	if (!frk)
		return (false);
	a = (pthread_mutex_destroy(&frk->write_mtx) != 0);
	return ((pthread_mutex_destroy(&frk->read_mtx) != 0) && a);
}

bool	pickup(t_frk *frk)
{
	if (!pthread_mutex_lock(&frk->takeup_mtx))
		return (frk->taken = true, true);
	return (false);
}

bool	putdown(t_frk *frk)
{
	frk->taken = false;
	// are you sure? maybe negate (read docs lol)
	return (pthread_mutex_unlock(&frk->takeup_mtx));
}
