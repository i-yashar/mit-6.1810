//modifications in kernel/sysproc.c

uint64 sys_trace(void) {
  int n;
  argint(0, &n);
  myproc()->mask = n;
  return 0;
}

//modifications to fork() in kernel/proc.c

int fork(void) {
  //...

  acquire(&np->lock);
  np->mask = p->mask;
  release(&np->lock);

  return pid;
}

//modifications in kernel/syscall.c

extern uint64 sys_trace(void);

static uint64 (*syscalls[])(void) = {
//..
[SYS_trace]   sys_trace
};

static char *call_names[]= {
[SYS_fork]    "fork",
[SYS_exit]    "exit",
[SYS_wait]    "wait",
[SYS_pipe]    "pipe",
[SYS_read]    "read",
[SYS_kill]    "kill",
[SYS_exec]    "exec",
[SYS_fstat]   "fstat",
[SYS_chdir]   "chdir",
[SYS_dup]     "dup",
[SYS_getpid]  "getpid",
[SYS_sbrk]    "sbrk",
[SYS_sleep]   "sleep",
[SYS_uptime]  "uptime",
[SYS_open]    "open",
[SYS_write]   "write",
[SYS_mknod]   "mknod",
[SYS_unlink]  "unlink",
[SYS_link]    "link",
[SYS_mkdir]   "mkdir",
[SYS_close]   "close",
[SYS_trace]   "trace"
};

int isTraceEnabled(int sysCallNumber, int mask) {
  mask = mask >> sysCallNumber;

  return mask % 2 == 1;
}

void
syscall(void)
{
  //..
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    ///..
    uint64 returnValue = syscalls[num]();
    p->trapframe->a0 = returnValue;

    if(isTraceEnabled(num, p->mask)) {
      printf("%d: syscall %s -> %d\n",
              p->pid, call_names[num], returnValue);
    }
  } else {
    //..
  }
}
