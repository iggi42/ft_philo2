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

// val is true if the fork is in use
bool	init_frk(t_frk *frk, unsigned char id)
{
	if (!frk)
		return (false);
	frk->id = id;
	frk->taken = false;
	return (pthread_mutex_init(&frk->mutex, NULL) == 0);
}

bool	destroy_frk(t_frk *frk)
{
	if (!frk)
		return (false);
	return (pthread_mutex_destroy(&frk->mutex) != 0);
}

bool	pickup(t_frk *frk)
{
	if (!pthread_mutex_lock(&frk->mutex))
		return (frk->taken = true, true);
	return (false);
}

bool	putdown(t_frk *frk)
{
	frk->taken = false;
	return (!pthread_mutex_unlock(&frk->mutex));
}
