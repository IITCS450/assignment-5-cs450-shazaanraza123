# Assignment 5 — RESULTS

## Context switching

Each thread has its own stack (except main, which uses the normal process stack). We save registers in a small `struct context` and switch with `uswtch`. `thread_yield` picks the next runnable thread in round-robin order and switches to it. New threads start in `thread_stub`, which runs the real function, then marks the thread zombie and yields.

## Mutex

One `volatile int` for locked/unlocked. `mutex_lock` does `while (locked) thread_yield()` then sets locked to 1. `mutex_unlock` sets it back to 0. Cooperative scheduling only, so no atomics needed.

## test_pc

Bounded buffer with two producers and one consumer, mutex around the shared buffer. Main joins the three threads, then prints `test_pc: done`.

## Limits

Max 16 threads, 2048-byte stacks for children, kept in a static `stack_pool` (no heap `malloc` for stacks — xv6’s allocator triggers `allocuvm` OOM otherwise). No preemption; threads have to call yield (or wait on the mutex) to switch.
