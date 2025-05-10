---
title: Project Milestone 2
Author(s): Aaryan Singh and Jaxon Rogers
Date: 05/12/2025
---

High level description
======================

We’re extending xv6 with a new `thread_create` syscall and simple user‐level API so that a process can spawn multiple threads, each with its own pagetable copy and independent user‐stack. For Milestone 2, threads do **not** share memory, but otherwise behave like schedulable entities: they run concurrently on separate stacks and begin execution at any user‐supplied function.


Thread creation
================

We chose the minimal, pthread‐style signature:

int thread_create(thread_t *new_tid,
                  void *(*thread_fn)(void *),
                  void *fd1);

Stack separation
================

Describe how your threads gets assigned different stacks (even though they are
still in separate address spaces for now).

Test cases
===========

- Describe the test case you used for the creation of the threads.

Addition comments
==================

Add any addition comments, questions, or design decisions that you are
considering for the final submission.