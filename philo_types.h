/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 00:51:19 by fkruger           #+#    #+#             */
/*   Updated: 2026/07/24 00:51:58 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TYPES_H
# define PHILO_TYPES_H
# include "frk.h"
# include "time.h"
# include <stdbool.h>
# include <stddef.h>

// 0 is not a valid philo id btw
typedef unsigned char	t_philo_id;

// if max_meals is -1, the simulation should run endless / till one dies
typedef struct s_philo_conf
{
	size_t				n_phil;
	t_timespan			t2die;
	t_timespan			t2eat;
	t_timespan			t2nap;
	int					max_meals;
}						t_philo_conf;

typedef struct s_philo
{
	t_frk				*left;
	t_frk				*right;
	t_timespan			last_meal;
	pthread_mutex_t		last_meal_mutex;
	t_philo_id			id;
	t_philo_conf		*c;
}						t_philo;

#endif
