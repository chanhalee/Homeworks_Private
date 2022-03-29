#include "ku_mlfq.h"
#include <stdio.h>

t_scheduler_infobox sched_info;


/*============= queue manipulation =============*/

t_pcb_queue	*queue_push_pcb(t_pcb_queue *queue, t_pcb *pcb)
{
	if (!queue || !pcb)
		return (queue);
	if (!(queue->front))
		queue->front = pcb;
	else
		queue->rear->lower = pcb;
	(queue->quantity)++;
	pcb->upper = queue->rear;
	queue->rear = pcb;
	return (queue);
}

t_pcb	*queue_pop_pcb(t_pcb_queue *queue)
{
	t_pcb	*ret;

	if (!queue->front)
		return (NULL);
	ret = queue->front;
	queue->quantity--;
	queue->front = ret->lower;
	if (queue->front == NULL)
		queue->rear = NULL;
	else
		queue->front->upper = NULL;
	ret->lower = NULL;
	return (ret);
}


/*============= utils =============*/


void	emergency_exit(int exit_code)
{
	int	priority;

	priority = -1;
	while (++priority < TYPE_PRIOR_MAX)
	{
		while (sched_info.queue[priority].front)
		{
			free(queue_pop_pcb(&sched_info.queue[priority]));
		}
	}
	if(exit_code != 0)
		exit(exit_code);
}

void	make_pcb(t_pcb **pcb, pid_t pcb_pid)
{
	*pcb = (t_pcb *)malloc(sizeof(t_pcb));
	if (!pcb)
		emergency_exit(1);
	(*pcb)->lower = NULL;
	(*pcb)->upper = NULL;
	(*pcb)->time_left = TYPE_TIME_GAMING_TOLERANCE;
	(*pcb)->prcess_id = pcb_pid;
	(*pcb)->state = TYPE_STATE_READY;
}

/*============= handlers =============*/

void	alarm_handler(int signal)
{
	int	priority;
	t_pcb *pcb;

	sched_info.time_terminate -= TYPE_TIME_SLICE;
	sched_info.time_next_boost -= TYPE_TIME_SLICE;
	priority = TYPE_PRIOR_MAX;
	while (--priority >= 0)
	{
		if (sched_info.queue[priority].front)
		{
			sched_info.queue[priority].front->time_left -= TYPE_TIME_SLICE;
			if (sched_info.queue[priority].front->time_left > 0)
			{
				pcb = queue_pop_pcb(&(sched_info.queue[priority]));
				kill(pcb->prcess_id, SIGSTOP);
				pcb->state = TYPE_STATE_READY;
				queue_push_pcb(&(sched_info.queue[priority]), pcb);
			}
			else
			{
				pcb = queue_pop_pcb(&(sched_info.queue[priority]));
				kill(pcb->prcess_id, SIGSTOP);
				pcb->state = TYPE_STATE_READY;
				pcb->time_left = TYPE_TIME_GAMING_TOLERANCE;
				if (priority > 0)
				{
					queue_push_pcb(&(sched_info.queue[priority - 1]), pcb);
				}
				else
				{
					queue_push_pcb(&(sched_info.queue[priority]), pcb);
				}
			}
			break ;
		}
	}
	if (sched_info.time_next_boost <= 0)
	{
		priority = TYPE_PRIOR_MAX - 1;
		while (--priority >= 0)
		{
			while (sched_info.queue[priority].front)
			{
				pcb = queue_pop_pcb(&sched_info.queue[priority]);
				pcb->time_left = TYPE_TIME_GAMING_TOLERANCE;
				queue_push_pcb(&sched_info.queue[TYPE_PRIOR_MAX - 1], pcb);
			}
		}
		sched_info.time_next_boost = TYPE_TIME_BOOST;
	}
	if (sched_info.time_terminate <= 0)
	{
		return ;
	}
	priority = TYPE_PRIOR_MAX;
	while (--priority >= 0)
	{
		if (sched_info.queue[priority].front)
		{
			sched_info.queue[priority].front->state = TYPE_STATE_RUNNING;
			/* printf("%d\n", priority); */
			kill(sched_info.queue[priority].front->prcess_id, SIGCONT);
			return ;
		}
	}
}


/*============= initiate =============*/

static int	scheduler_initiate(char **argv)
{
	static struct sigaction	sig_act;
	int						process_quant;
	pid_t					process_pid;
	char					process_arg[2];
	t_pcb					*new_pcb;

	sched_info.time_terminate = atoi(argv[2]) * TYPE_TIME_SLICE;
	sched_info.process_quant = atoi(argv[1]);
	if (sched_info.time_terminate <= 0)
		return (-1);
	if (sched_info.process_quant < 1 || sched_info.process_quant > 26)
		return (-1);

	sigemptyset(&(sig_act.sa_mask));
	sig_act.sa_handler = &alarm_handler;
	sigaction(SIGALRM, &sig_act, NULL);

	sched_info.scheduler_pid = getpid();
	sched_info.time_next_boost = TYPE_TIME_BOOST;
	process_quant = -1;
	process_arg[1] = '\0';
	while (++process_quant < sched_info.process_quant)
	{
		process_pid = fork();
		if (process_pid)
		{
			make_pcb(&new_pcb, process_pid);
			queue_push_pcb(&sched_info.queue[TYPE_PRIOR_MAX - 1], new_pcb);
		}
		else
		{
			process_arg[0] = ('A' + process_quant);
			execl("./ku_app", "ku_app", (char *)process_arg,  (char *) NULL);
		}
	}
	sleep(5);
	process_quant = 0;
	while (++process_quant <= TYPE_PRIOR_MAX)
	{
		sched_info.queue[process_quant - 1].priority = process_quant;
	}
	return (0);
}


/*============= main =============*/

int	main(int argc, char **argv)
{
	struct itimerval	timer;
	struct timeval 		time_val;

	/* validate argv and initiate*/
	if (argc != 3)
		return (0);
	if (scheduler_initiate(argv))
		emergency_exit(1);

	/* set timer value */
	time_val.tv_sec = TYPE_TIME_SLICE;
	time_val.tv_usec = 0;
	timer.it_value = time_val;
	timer.it_interval = time_val;

	/* start scheduling */
	if (sched_info.queue[TYPE_PRIOR_MAX - 1].front)
	{
		kill(sched_info.queue[TYPE_PRIOR_MAX - 1].front->prcess_id, SIGCONT);
		setitimer(ITIMER_REAL, &timer, NULL);
	}
	while (sched_info.time_terminate > 0)
		sleep(TYPE_TIME_SLICE + 1);
	
	/* free heap memory and return */
	emergency_exit(0);
	return (0);
}
