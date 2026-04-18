#pragma once

struct context;

void thread_init(void);
int thread_create(void (*fn)(void*), void *arg);
void thread_yield(void);
int thread_join(int tid);

void uswtch(struct context **old, struct context *new);
