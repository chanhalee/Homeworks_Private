#ifndef KU_MLFQ_H
# define KU_MLFQ_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdarg.h>
# include <signal.h>
# include <sys/time.h>

# define TYPE_TIME_SLICE 1
# define TYPE_PRIOR_MAX 3
# define TYPE_TIME_GAMING_TOLERANCE 2
# define TYPE_TIME_BOOST 10
# define TYPE_STATE_READY 0
# define TYPE_STATE_RUNNING 1

typedef struct s_pcb
{
	pid_t			prcess_id;
	struct s_pcb	*upper;
	struct s_pcb	*lower;
	int				time_left;
	int				state;
}	t_pcb;

typedef struct s_pcb_queue
{
	int			quantity;
	int			priority;
	t_pcb		*front;
	t_pcb		*rear;
}	t_pcb_queue;


typedef struct s_scheduler_infobox
{
	pid_t		scheduler_pid;
	int			time_next_boost;
	int			time_terminate;
	int			process_quant;
	t_pcb_queue	queue[TYPE_PRIOR_MAX];
}	t_scheduler_infobox;

#endif