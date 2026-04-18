#include "types.h"
#include "stat.h"
#include "user.h"
#include "uthread.h"

#define MAX_THREADS 16
#define STACK_SIZE 4096

struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum { FREE, RUNNABLE, RUNNING, ZOMBIE };

struct thread {
  int state;
  void *stack;
  struct context saved;
  struct context *ctx;
};

static struct thread threads[MAX_THREADS];
static int current_tid;
static struct context *current_ctx;

static void thread_stub(void);

void thread_init(void)
{
  for(int i = 0; i < MAX_THREADS; i++){
    threads[i].state = FREE;
    threads[i].stack = 0;
    threads[i].ctx = 0;
  }

  threads[0].state = RUNNING;
  threads[0].stack = 0;
  threads[0].ctx = &threads[0].saved;

  current_tid = 0;
  current_ctx = threads[0].ctx;
}

int thread_create(void (*fn)(void*), void *arg)
{
  int i;
  for(i = 1; i < MAX_THREADS; i++){
    if(threads[i].state == FREE)
      break;
  }
  if(i == MAX_THREADS)
    return -1;

  char *stk = malloc(STACK_SIZE);
  if(stk == 0)
    return -1;

  uint *base = (uint*)(stk + STACK_SIZE - 8 * sizeof(uint));
  base[0] = 0;
  base[1] = 0;
  base[2] = 0;
  base[3] = 0;
  base[4] = (uint)thread_stub;
  base[5] = 0xffffffff;
  base[6] = (uint)fn;
  base[7] = (uint)arg;

  threads[i].stack = stk;
  threads[i].ctx = (struct context*)base;
  threads[i].state = RUNNABLE;

  return i;
}

static void thread_stub(void)
{
  uint *sp;
  asm volatile("movl %%esp, %0" : "=r"(sp));
  void (*fn)(void*) = (void(*)(void*))sp[1];
  void *arg = (void*)sp[2];

  fn(arg);

  threads[current_tid].state = ZOMBIE;
  thread_yield();
}

static int pick_next(void)
{
  int tid = current_tid;
  for(int k = 0; k < MAX_THREADS; k++){
    int t = (tid + 1 + k) % MAX_THREADS;
    if(threads[t].state == RUNNABLE)
      return t;
  }
  return -1;
}

void thread_yield(void)
{
  int tid = current_tid;

  if(threads[tid].state == RUNNING)
    threads[tid].state = RUNNABLE;

  int next = pick_next();

  if(next < 0){
    if(threads[tid].state == RUNNABLE)
      threads[tid].state = RUNNING;
    return;
  }

  if(next == tid){
    threads[tid].state = RUNNING;
    return;
  }

  threads[next].state = RUNNING;
  current_tid = next;
  uswtch(&current_ctx, threads[next].ctx);
}

int thread_join(int tid)
{
  if(tid < 0 || tid >= MAX_THREADS || tid == 0)
    return -1;

  while(threads[tid].state != ZOMBIE)
    thread_yield();

  if(threads[tid].stack)
    free(threads[tid].stack);

  threads[tid].stack = 0;
  threads[tid].ctx = 0;
  threads[tid].state = FREE;

  return 0;
}
