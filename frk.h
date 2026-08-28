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
# include <pthread.h>
# include <stdbool.h>


/* setting and unsetting taken_by is done to demonstrate */
/* taken is a fork is taken or not */
typedef struct s_frk
{
	unsigned char	id;
	bool			taken;
	unsigned char	taken_by;
	pthread_mutex_t	taken_mtx;
	pthread_mutex_t	taken_by_mtx;
}					t_frk;

// set default values really
bool				init_frk(t_frk *frk, unsigned char id);
bool				destroy_frk(t_frk *frk);

bool	sync_takeup(unsigned char id, t_frk *f0, t_frk *f1);

// ...
bool				pickup(t_frk *frk, unsigned char phil_id);
bool				putdown(t_frk *frk, unsigned char phil_id);
#endif
