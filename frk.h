/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frk.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 11:57:59 by fkruger           #+#    #+#             */
/*   Updated: 2026/07/24 01:03:25 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRK_H
# define FRK_H
# include "philo_types.h"
# include <pthread.h>
# include <stdbool.h>

// set default values really
bool	init_frk(t_frk *frk, unsigned char id);
bool	destroy_frk(t_frk *frk);

// executes do_somthing with the philo after locking both fork mutexes for "taken"
int	frk_sync_do(int (*do_smth)(void *me), t_philo *me);

// this needs to seperate error case and return val
// error case is also the "we need to stop early because another philo died" case
// returns -1 on error, 0 has not eaten, 1 on has eaten
int		sync_takeup(t_philo *me, t_frk *f0, t_frk *f1);
int		sync_putdown(t_philo *me, t_frk *f0, t_frk *f1);
//
bool	pickup(t_frk *frk, t_philo_id phil_id);
bool	putdown(t_frk *frk, t_philo_id phil_id);
#endif
