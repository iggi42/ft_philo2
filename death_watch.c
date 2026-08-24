#include "death_watch.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

// constructur
bool form_death_watch(t_ds *s)
{
	s->kys = false;
	return (!pthread_mutex_init(&s->mutex,NULL));
}

// deconstructor
void free_death_watch(t_ds *s)
{
	pthread_mutex_destroy(&s->mutex);
	free(s);
}

// returns true if should end asap
bool	check_death_watch(t_ds *s)
{
	bool	result;

	if (pthread_mutex_lock(&s->mutex))
		return (true);
	result = s->kys;
	return (pthread_mutex_unlock(&s->mutex) || result);
}

// this causes all later calls of check_death_watch to return true
void	trigger_death_watch(t_ds *s)
{
	if (!pthread_mutex_lock(&s->mutex))
		return ;
	s->kys = true;
	pthread_mutex_unlock(&s->mutex);
}
