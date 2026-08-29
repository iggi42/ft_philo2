#include "frk.h"
#include "logging.h"
#include "philo_types.h"
#include <stdio.h>

int	frk_sync_do(int (*do_smth)(t_philo *me, t_frk *f0, t_frk *f1), t_philo *me,
		t_frk *f0, t_frk *f1)
{
	static pthread_mutex_t	local_mut = PTHREAD_MUTEX_INITIALIZER;
	bool					has_eaten;
	bool					error_happend;

	has_eaten = false;
	error_happend = false;
	if (!f0 || !f1)
		return (-1);
	if (f0->id > f1->id)
		return (sync_takeup(me, f1, f0));
	if (pthread_mutex_lock(&local_mut) != 0)
		return (-1);
	if (pthread_mutex_lock(&f0->taken_mtx) == 0)
	{
		if (pthread_mutex_lock(&f1->taken_mtx) == 0)
		{
			do_smth(me, f0, f1);
			if (pthread_mutex_unlock(&f1->taken_mtx) != 0)
				error_happend = true;
		}
		else
			error_happend = true;
		pthread_mutex_unlock(&f0->taken_mtx);
	}
	else
		error_happend = true;
	if (pthread_mutex_unlock(&local_mut) != 0)
		return (-1);
	if (error_happend == false)
		return (-1);
	if (has_eaten)
		return (1);
	return (0);
}

// this needs to seperate error case and return val
// error case is also the "we need to stop early because another philo died" case
// returns -1 on error, 0 has not eaten, 1 on has eaten
int	sync_takeup(t_philo *me, t_frk *f0, t_frk *f1)
{
	static pthread_mutex_t	local_mut = PTHREAD_MUTEX_INITIALIZER;
	bool					has_eaten;
	bool					error_happend;

	has_eaten = false;
	error_happend = false;
	if (!f0 || !f1)
		return (-1);
	if (f0->id > f1->id)
		return (sync_takeup(me, f1, f0));
	if (pthread_mutex_lock(&local_mut) != 0)
		return (-1);
	if (pthread_mutex_lock(&f0->taken_mtx) == 0)
	{
		printf("philo [%d] has taken fork [%d]\n", me->id, f0->id);
		if (pthread_mutex_lock(&f1->taken_mtx) == 0)
		{
			printf("philo [%d] has taken fork [%d]\n", me->id, f0->id);
			has_eaten = (!f0->taken && !f1->taken);
			if (!error_happend && has_eaten)
			{
				error_happend = error_happend && log_queue(log_forklift, me);
				f0->taken = true;
				error_happend = error_happend && log_queue(log_forklift, me);
				f1->taken = true;
			}
			if (pthread_mutex_unlock(&f1->taken_mtx) != 0)
				error_happend = true;
		}
		else
			error_happend = true;
		pthread_mutex_unlock(&f0->taken_mtx);
	}
	else
		error_happend = true;
	if (pthread_mutex_unlock(&local_mut) != 0)
		return (-1);
	if (error_happend == false)
		return (-1);
	if (has_eaten)
		return (1);
	return (0);
}
