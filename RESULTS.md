# Assignment 5 — RESULTS

## Context switching

Each thread has its own stack (except main, which uses the normal process stack). We save registers in a small `struct context` and switch with `uswtch`. `thread_yield` picks the next runnable thread in round-robin order and switches to it. New threads start in `thread_stub`, which runs the real function, then marks the thread zombie and yields.

## Mutex

One `volatile int` for locked/unlocked. `mutex_lock` does `while (locked) thread_yield()` then sets locked to 1. `mutex_unlock` sets it back to 0. Cooperative scheduling only, so no atomics needed.

## test_pc

Bounded buffer with two producers and one consumer, mutex around the shared buffer. Main joins the three threads, then prints `test_pc: done`.

## Limits

Max 16 threads, 2048-byte stacks (smaller than 4K so three `malloc` stacks fit xv6’s user heap). No preemption; threads have to call yield (or wait on the mutex) to switch.
