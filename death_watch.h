
#ifndef DEATH_WATCH_H
# define DEATH_WATCH_H
#include <stdbool.h>
#include <pthread.h>

typedef struct s_death_signal
{
	bool			kys;
	pthread_mutex_t	mutex;
}					t_ds;

bool form_death_watch(t_ds *s);
void free_death_watch(t_ds *s);

bool check_death_watch(t_ds *watch);
void trigger_death_watch(t_ds *watch);
#endif
