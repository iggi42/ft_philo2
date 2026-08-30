#include "frk.h"
#include "logging.h"
#include "philo_types.h"
#include <stdbool.h>

static int protected(t_philo *me)
{
	int result;
	bool has_eaten = (!f0->taken && !f1->taken);
	if (!error_happend && has_eaten)
	{
		error_happend = log_queue(log_forklift, me) ? error_ha
		f0->taken = true;
		error_happend = error_happend && log_queue(log_forklift, me);
		f1->taken = true;
	}
}

// this needs to seperate error case and return val
// error case is also the "we need to stop early because another philo died" case
// returns -1 on error, 0 has not eaten, 1 on has eaten
int	sync_takeup(t_philo *me)
{
	static pthread_mutex_t	local_mut = PTHREAD_MUTEX_INITIALIZER;
	bool					has_eaten;
	bool					error_happend;

	if (error_happend == false)
		return (-1);
	if (has_eaten)
		return (1);
	return (0);
}
