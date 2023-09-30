//kernel/sysproc.c

#include "sysinfo.h"

uint64 sys_sysinfo(void) {
  struct sysinfo sysinfo;

  calculateFreeMemory(&sysinfo);
  calculateNumberOfProcesses(&sysinfo);

  printf("mem %d\n", sysinfo.freemem);
  printf("proc %d\n", sysinfo.nproc);

  uint64 sysinfoArg;
  argaddr(0, &sysinfoArg);

  struct proc *p = myproc();

  if(copyout(p->pagetable, sysinfoArg, (char *)&sysinfo, sizeof(sysinfo)) < 0) {
    return -1;
  }

  return 0;
}

//kernel/kalloc.c

#include "sysinfo.h"

void calculateFreeMemory(struct sysinfo * sysinfo) {
  struct run *r;
  uint64 count = 0;

  acquire(&kmem.lock);
  r = kmem.freelist;
  while(r) {
    r = r->next;
    count++;
  }
  release(&kmem.lock);
  
  sysinfo->freemem = count * 4096;
}

//kernel/proc.c

#include "sysinfo.h"

void calculateNumberOfProcesses(struct sysinfo *sysinfo) {
  struct proc *p;
  uint64 count = 0;

  for(p = proc; p < &proc[NPROC]; p++) {
    if(p->state != UNUSED) {
      count++;
    }
  }

  sysinfo->nproc = count;
}

//kernel/sysinfo.h

struct sysinfo {
  uint64 freemem;   // amount of free memory (bytes)
  uint64 nproc;     // number of process
};

void calculateFreeMemory(struct sysinfo *);

void calculateNumberOfProcesses(struct sysinfo *);