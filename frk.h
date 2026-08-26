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

typedef struct s_frk
{
	unsigned char	id;
	pthread_mutex_t	mutex;
}					t_frk;

// set default values really
bool				init_frk(t_frk *frk, unsigned char id);
bool				destroy_frk(t_frk *frk);

// ...
bool				pickup(t_frk *frk);
bool				putdown(t_frk *frk);
#endif
