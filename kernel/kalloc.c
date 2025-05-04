// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"


#define Nof ((PHYSTOP - KERNBASE) / PGSIZE)
static int rc[Nof];
// used a lot of stackoverlow and geeks for geeks for this lab in general
#define FRINDEX(pa) (((uint64)(pa) - KERNBASE) / PGSIZE)


extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void freerange(void *pa_start, void *pa_end);

void
kinit(void)
{
  initlock(&kmem.lock, "kmem");

  int allfrms = (PHYSTOP - KERNBASE) / PGSIZE;
  for (int j = 0; j < allfrms; j++) {
    rc[j] = 1;
  }

  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  acquire(&kmem.lock);
  

  int id = FRINDEX((uint64)pa);
  if (--rc[id] > 0) {
    release(&kmem.lock);
    return;
  }
  if (rc[id] < 0)

    panic("kfre ref count negtive");

  memset(pa, 1, PGSIZE);
  r = (struct run*)pa;
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;

  if(r) {

    kmem.freelist = r->next;
    rc[FRINDEX((uint64)r)] = 1;
  }

  release(&kmem.lock);

  if(r) 
    memset((char*)r, 5, PGSIZE);

  return (void*)r;

}

void incr(uint64 pa) {
  acquire(&kmem.lock);
  rc[FRINDEX(pa)]++;
  release(&kmem.lock);
}

void decr(uint64 pa) {
  acquire(&kmem.lock);
  int id = FRINDEX(pa);
  rc[id]--;
  if(rc[id] < 0)
	  panic("decrr: count neg");
  release(&kmem.lock);
}
