#ifndef KU_MMU_H
# define KU_MMU_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdarg.h>
# include <signal.h>
# include <sys/time.h>


typedef struct ku_pte {
	char			pid;
	struct ku_pte	*prev;
	struct ku_pte	*next;
	char			data;
}	t_my_pte;


typedef struct s_pcb
{
	char			prcess_id;
	struct s_pcb	*next;
	struct s_pcb	*prev;
	t_my_pte		*page_table;
	int				state;
}	t_pcb;


static void		*ku_mmu_swap = NULL;
static t_pcb	*ku_mmu_pcb_list = NULL;

int		ku_page_fault(char pid, char va);
void	*ku_mmu_init(unsigned int mem_size, unsigned int swap_size);
int		ku_run_proc(char pid, struct ku_pte **ku_cr3);

/* ===================== uitl ======================== */


void free_page_table(t_my_pte *page_table)
{
	t_my_pte	*next;
	t_my_pte	*temp;

	next = page_table;
	while(next)
	{
		temp = next->next;
		free(next);
		next = temp;
	}
}

void free_pcb(t_pcb *pcb)
{
	t_pcb	*next;
	t_pcb	*temp;

	next = pcb;
	while (next)
	{
		free_page_table (next->page_table);
		temp = next->next;
		free(next);
		next = temp;
	}
}

void emergency_free(void)
{
	if (ku_mmu_pcb_list)
		free_pcb(ku_mmu_pcb_list);
	if (ku_mmu_swap)
		free(ku_mmu_swap);
}

t_my_pte	*make_page_table(char pid)
{
	int			entry;
	t_my_pte	*ret;
	t_my_pte	*prev;
	t_my_pte	*now;

	entry = -1;
	now = NULL;
	ret = (t_my_pte	*)malloc(sizeof(t_my_pte));
	if (!ret)
	{
		emergency_free();
		return (NULL);
	}
	ret->prev = NULL;
	ret->data = 0;
	ret->pid = pid;
	prev = ret;
	while(++entry < 256)
	{
		now = (t_my_pte	*)malloc(sizeof(t_my_pte));
		if (!now)
		{
			emergency_free();
			return (NULL);
		}
		prev->next = now;
		now->prev = prev;
		now->next = NULL;
		now->data = 0;
		now->pid = pid;
		prev = now;
	}
	return (ret);
}

t_pcb *	make_pcb(char pid)
{
	t_pcb	*pcb;
	pcb= (t_pcb *)malloc(sizeof(t_pcb));
	if (!pcb)
	{
		emergency_free();
		return (NULL);
	}
	pcb->next = NULL;
	pcb->prev = NULL;
	pcb->prcess_id = pid;
	pcb->page_table = make_page_table(pid);
	if (pcb->page_table == NULL)
		return (NULL);
	return (pcb);
}

/* ===================== subject ======================== */

int ku_page_fault(char pid, char va)
{
	t_pcb		*now;
	t_my_pte	*table;
	t_my_pte	*next_entry;

	now = ku_mmu_pcb_list;
	while(now != NULL)
	{
		if (now->prcess_id == pid)
		{
			break;
		}
		else
		{
			now = ku_mmu_pcb_list->next;
		}
	}
	table = now->page_table;
	while (va-- != 0)
		table = table->next;
}

void	*ku_mmu_init(unsigned int mem_size, unsigned int swap_size)
{
	void	*phy_mem;

	ku_mmu_swap = (void *)malloc(swap_size);
	if(!ku_mmu_swap)
	{
		emergency_free();
		return (0);
	}
	phy_mem = (void *)malloc(mem_size);
	if(!phy_mem)
	{
		emergency_free();
		return (0);
	}
	return (phy_mem);
}


int		ku_run_proc(char pid, struct ku_pte **ku_cr3)
{
	t_pcb	*next;
	t_pcb	*prev;

	next = ku_mmu_pcb_list;
	while(next != NULL)
	{
		if (next->prcess_id == pid)
		{
			ku_cr3 = &(next->page_table);
			return (0);
		}
		else
		{
			prev = next;
			next = ku_mmu_pcb_list->next;
		}
	}

	if (ku_mmu_pcb_list == NULL)
	{
		ku_mmu_pcb_list = make_pcb(pid);
		if (ku_mmu_pcb_list == NULL)
			return (-1);
	}
	else
	{
		next = make_pcb(pid);
		if (next == NULL)
			return (-1);
		prev -> next = next;
		next -> prev = prev;
		ku_cr3 = &(next->page_table);
			return (0);
	}
}

#endif