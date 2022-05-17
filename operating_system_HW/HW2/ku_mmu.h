#ifndef KU_MMU_H
# define KU_MMU_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>
# include <stdarg.h>
# include <signal.h>
# include <sys/time.h>


typedef struct ku_pte {
	unsigned char eight_bit;
}	t_my_pte;

typedef struct s_alloc_entry{
	unsigned int	p_addr;
	unsigned char			pid;
	unsigned char			va;
}	t_alloc_entry;

typedef struct s_free_entry{
	unsigned int	s_addr;
	unsigned char			pid;
	unsigned char			va;
} t_free_entry;

typedef struct s_alloc_list{
	unsigned		max_size;
	unsigned		next;	// 0 == full
	t_alloc_entry	*list;
}	t_alloc_list;

typedef struct s_free_list{
	unsigned		max_size;
	unsigned		next;	// 0 == full
	t_free_entry	*list;
}	t_free_list;

typedef struct s_pcb
{
	unsigned char			prcess_id;
	struct s_pcb	*next;
	struct s_pcb	*prev;
	struct ku_pte	page_table[64];
}	t_pcb;


static t_alloc_list		ku_mmu_alloc_list;
static t_free_list		ku_mmu_free_list;
static t_pcb	*ku_mmu_pcb_list = NULL;

int		ku_page_fault(char pid, char va);
void	*ku_mmu_init(unsigned int mem_size, unsigned int swap_size);
int		ku_run_proc(char pid, struct ku_pte **ku_cr3);

/* ===================== uitl ======================== */


void free_pcb(t_pcb *pcb)
{
	t_pcb	*next;
	t_pcb	*temp;

	next = pcb;
	while (next)
	{
		temp = next->next;
		free(next);
		next = temp;
	}
}

void emergency_free(void)
{
	if (ku_mmu_pcb_list)
		free_pcb(ku_mmu_pcb_list);
	if (ku_mmu_alloc_list.list)
		free(ku_mmu_alloc_list.list);
	if (ku_mmu_free_list.list)
		free(ku_mmu_free_list.list);
}

void init_page_table(struct ku_pte *table){
	int	index;

	index = -1;
	while(++index < 64){
		(table)[index].eight_bit = 0;
	}
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
	init_page_table(pcb->page_table);
	return (pcb);
}

t_pcb *find_pcb_by_pid(char pid)
{
	t_pcb	*next;

	next = ku_mmu_pcb_list;
	while (!next)
	{
		if((*next).prcess_id == pid)
		{
			return (next);
		}
		next = &((*next).next);
	}
	return (NULL);
}

char find_free_smem()
{
	unsigned char	ret;
	t_pcb	*pcb_kick;

	if (ku_mmu_free_list.list[ku_mmu_free_list.next].pid == 0)
	{
		ret = ku_mmu_free_list.list[ku_mmu_free_list.next].s_addr;
		ku_mmu_free_list.next += 4;
		if (ku_mmu_free_list.next >= ku_mmu_free_list.max_size )
		{
			ku_mmu_free_list.next = 4;
		}
	}
	else // swap이 꽉 찬 경우 ret -1일지 그냥 kick일지 모르지만, kick을 가정하고 진행
	{
		pcb_kick = find_pcb_by_pid(ku_mmu_free_list.list[ku_mmu_free_list.next].pid);
		pcb_kick->page_table[ku_mmu_free_list.list[ku_mmu_free_list.next].va / 4].eight_bit = 0;
		ret = ku_mmu_free_list.list[ku_mmu_free_list.next].s_addr;
		ku_mmu_free_list.next += 4;
		if (ku_mmu_free_list.next >= ku_mmu_free_list.max_size )
		{
			ku_mmu_free_list.next = 4;
		}
	}
	return (ret / 4);
}

char find_free_pmem() // pmem_size, swap_size가 4의 배수일 것으로 가정하고 진행했음.
{
	unsigned char	ret;
	t_pcb	*pcb_evic;

	if (ku_mmu_alloc_list.list[ku_mmu_alloc_list.next].pid == 0)
	{
		ret = ku_mmu_alloc_list.list[ku_mmu_alloc_list.next].p_addr;
		ku_mmu_alloc_list.next += 4;
		if (ku_mmu_alloc_list.next >= ku_mmu_alloc_list.max_size )
		{
			ku_mmu_alloc_list.next = 4;
		}
	}
	else
	{
		pcb_evic = find_pcb_by_pid(ku_mmu_alloc_list.list[ku_mmu_alloc_list.next].pid);
		pcb_evic->page_table[ku_mmu_alloc_list.list[ku_mmu_alloc_list.next].va /4].eight_bit = find_free_smem() * 2;
		ret = ku_mmu_alloc_list.list[ku_mmu_alloc_list.next].p_addr;
		ku_mmu_alloc_list.next += 4;
		if (ku_mmu_alloc_list.next >= ku_mmu_alloc_list.max_size )
		{
			ku_mmu_alloc_list.next = 4;
		}
	}
	return (ret / 4);
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
	if(now == NULL)
	{ // 중대한 오류. 확률은 0
		emergency_free();
		return (-1);
	}
	table = now->page_table;
	if(table[va / 4].eight_bit == 0) // unmapped
	{
		table[va / 4].eight_bit = (find_free_pmem()*4)|0X01;
		// printf("%d: %d\n",va/4, *((char *)(table)+25));
		return (0);
	}
	else // swap outed
	{	// swap 에서 다시 pmem으로 복귀할 경우 어떻게 swap의 빈 공간 처리할지 약속되지 않음. 임의로 그냥 구멍둟린채 진행한다고 가정
		ku_mmu_free_list.list[(table[va / 4].eight_bit/2)*4].pid = 0;
		ku_mmu_free_list.list[(table[va / 4].eight_bit/2)*4].va = 0;
		table[va / 4].eight_bit = (find_free_pmem()*4)|0X01;
		return (0);
	}
}

void	*ku_mmu_init(unsigned int mem_size, unsigned int swap_size)
{
	unsigned int		entry_index;
	void	*ret;

	ret = (void *)malloc(mem_size);
	if (!ret)
	{
		return (0);
	}
	ku_mmu_alloc_list.max_size = mem_size;
	ku_mmu_alloc_list.next = 4;
	ku_mmu_alloc_list.list = (t_alloc_entry *)malloc(sizeof(t_alloc_entry) * mem_size);
	if (!ku_mmu_alloc_list.list)
	{
		emergency_free();
		return (0);
	}
	entry_index = -1;
	while(++entry_index < mem_size)
	{
		ku_mmu_alloc_list.list[entry_index].va = 0;
		ku_mmu_alloc_list.list[entry_index].p_addr = entry_index;
		ku_mmu_alloc_list.list[entry_index].pid = 0;
	}
	ku_mmu_free_list.max_size = swap_size;
	ku_mmu_free_list.next = 4;
	ku_mmu_free_list.list = (t_free_entry *)malloc(sizeof(t_free_entry) * swap_size);
	if (!ku_mmu_free_list.list)
	{
		emergency_free();
		return (0);
	}
	entry_index = -1;
	while(++entry_index < swap_size)
	{
		ku_mmu_free_list.list[entry_index].va = 0;
		ku_mmu_free_list.list[entry_index].s_addr = entry_index;
		ku_mmu_free_list.list[entry_index].pid = 0;
	}
	ku_mmu_pcb_list = make_pcb(0);
	if (ku_mmu_pcb_list == NULL)
	{
		emergency_free();
		return (0);
	}
	return ret;
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

	next = make_pcb(pid);
	if (next == NULL)
		return (-1);
	prev -> next = next;
	next -> prev = prev;
	ku_cr3 = &(next->page_table);
	return (0);
}

#endif


// void free_page_table(t_my_pte *page_table)
// {
// 	t_my_pte	*next;
// 	t_my_pte	*temp;

// 	next = page_table;
// 	while(next)
// 	{
// 		temp = next->next;
// 		free(next);
// 		next = temp;
// 	}
// }


// t_my_pte	*make_page_table(char pid)
// {
// 	int			entry;
// 	t_my_pte	*ret;
// 	t_my_pte	*prev;
// 	t_my_pte	*now;

// 	entry = -1;
// 	now = NULL;
// 	ret = (t_my_pte	*)malloc(sizeof(t_my_pte));
// 	if (!ret)
// 	{
// 		emergency_free();
// 		return (NULL);
// 	}
// 	ret->prev = NULL;
// 	ret->data = 0;
// 	ret->pid = pid;
// 	prev = ret;
// 	while(++entry < 256)
// 	{
// 		now = (t_my_pte	*)malloc(sizeof(t_my_pte));
// 		if (!now)
// 		{
// 			emergency_free();
// 			return (NULL);
// 		}
// 		prev->next = now;
// 		now->prev = prev;
// 		now->next = NULL;
// 		now->data = 0;
// 		now->pid = pid;
// 		prev = now;
// 	}
// 	return (ret);
// }