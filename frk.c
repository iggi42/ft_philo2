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
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

// taken is true if the fork is in use
bool	init_frk(t_frk *frk, unsigned char id)
{
	bool crt_by;
	bool crt_tkn;
	
	if (!frk)
		return (false);
	frk->id = id;
	frk->taken = false;
	crt_by = (pthread_mutex_init(&frk->taken_by_mtx, NULL) == 0);
	crt_tkn = (pthread_mutex_init(&frk->taken_by_mtx, NULL) == 0);
	if(crt_by && crt_tkn)
		return true;
	crt_by = (pthread_mutex_destroy(&frk->taken_by_mtx) == 0);
	crt_tkn = (pthread_mutex_destroy(&frk->taken_by_mtx) == 0);
	return false;
}

bool	destroy_frk(t_frk *frk)
{
	if (!frk)
		return (false);
	return (pthread_mutex_destroy(&frk->taken_by_mtx) != 0);
}

// returns -1 on error, 0 has not eaten, 1 on has eaten
bool	pickup(t_frk *frk, t_philo_id id)
{
	if (pthread_mutex_lock(&frk->taken_by_mtx))
		return (false);
	frk->taken_by = id;
	return (true);
}

bool	putdown(t_frk *frk, t_philo_id phil_id)
{
	if (phil_id != frk->taken_by)
		return (printf("WTF BRO\n"), false);
	frk->taken_by = 0;
	frk->taken = false;
	return (pthread_mutex_unlock(&frk->taken_by_mtx) != 0);
}
