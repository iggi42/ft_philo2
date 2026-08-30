#include "philo_types.h"

static int	mtx_safely_do(pthread_mutex_t *mtx0, pthread_mutex_t *mtx1,
		int (*do_smth)(void *arg), void *arg)
{
	int	result;

	if (do_smth == NULL || mtx0 == mtx1)
		return (-1);
	if (pthread_mutex_lock(mtx0) != 0)
		return (-1);
	if (pthread_mutex_lock(mtx1) != 0)
		return (pthread_mutex_unlock(mtx0), -1);
	result = do_smth(arg);
	if(pthread_mutex_unlock(mtx1))
		result = -1;
	if (pthread_mutex_unlock(mtx0) != 0)
		result = -1;
	return (result);
}

// run do_smth after taking the the mutex
int	frk_sync_do(int (*do_smth)(void *me), t_philo *me)
{
	if (me == NULL || me->left == NULL || me->right == NULL)
		return (-1);
	if (me->left->id > me->right->id)
		return (mtx_safely_do(&me->left->taken_mtx, &me->right->taken_mtx, do_smth, me));
	return (mtx_safely_do(&me->right->taken_mtx, &me->left->taken_mtx, do_smth, me));
}
