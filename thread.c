//apt-get install gcc-multilib

#include "thread.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


// thread metadata
struct thread {
	void *esp;
	struct thread *next;
	struct thread *prev;
};

struct thread *ready_list = NULL;     // ready list
struct thread *cur_thread = NULL;     // current thread

// defined in context.s
void context_switch(struct thread *prev, struct thread *next);

// insert the input thread to the end of the ready list.
static void push_back(struct thread *t)
{
	t->next = NULL;
	t->prev = NULL;
	if (ready_list==NULL){
		ready_list = t;
		return;
	}
	struct thread *ptr = ready_list;
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = t;
	t->prev = ptr;
}

// remove the first thread from the ready list and return to caller.
static struct thread *pop_front()
{
	struct thread *ptr = ready_list;
	ready_list = ready_list->next;
	return ptr;
}

// the next thread to schedule is the first thread in the ready list.
// obtain the next thread from the ready list and call context_switch.
static void schedule()
{
	struct thread *prev = cur_thread;
	struct thread *next = pop_front();
	cur_thread = next;
	context_switch(prev, next);
}

// push the cur_thread to the end of the ready list and call schedule
// if cur_thread is null, allocate struct thread for cur_thread
static void schedule1()
{
	if (cur_thread==NULL){
		cur_thread = malloc(sizeof(struct thread));
	}
	push_back(cur_thread);
	schedule();
}

// allocate stack and struct thread for new thread
// save the callee-saved registers and parameters on the stack
// set the return address to the target thread
// save the stack pointer in struct thread
// push the current thread to the end of the ready list
void create_thread(func_t func, void *param)
{
	unsigned *stack = malloc(4096);
	stack += 1024;
	struct thread *t =malloc(sizeof(struct thread));
	*stack = (unsigned)param;
	stack --;
	*stack = 0;
	stack--;
	*stack = (unsigned)func;
	stack--;
	*stack = 0;
	stack--;
	*stack = 0;
	stack--;
	*stack = 0;
	stack--;
	*stack = 0;
	t->esp = stack;
	t->next = NULL;
	t->prev = NULL;
	push_back(t);

	
}

// call schedule1
void thread_yield()
{
	schedule1();
}

// call schedule
void thread_exit()
{
	schedule();
}

// call schedule1 until ready_list is null
void wait_for_all()
{
	while(ready_list!=NULL){
		schedule1();
	}
}
