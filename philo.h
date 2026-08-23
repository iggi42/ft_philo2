/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fkruger <fkruger@student.42vienna.com      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 08:10:02 by fkruger           #+#    #+#             */
/*   Updated: 2026/07/24 00:47:20 by fkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include "philo_types.h"
# include <stdbool.h>
# include <stddef.h>

t_philo	*new_philos(t_philo_conf *c, t_frk *cutler);
void	set_last_meal2now(t_philo *p);
void	turn_off_philo(t_philo *p);

// run a round
bool	run_sim(t_philo_conf *c);

void	*philo_routine_endless(void *);
void	*philo_routine_maxmeals(void *);

/*
bool	io_queue(void (*print_smth)(t_philo *p), t_philo *p);
void	log_animated(t_philo *philo);
void	log_forklift(t_philo *philo);
void	log_eating(t_philo *philo);
void	log_sleeping(t_philo *philo);
void	log_thinking(t_philo *philo);
void	log_died(t_philo *philo);
*/

#endif
