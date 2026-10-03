#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
extern struct proc proc[];

uint64
sys_ps(void)
{
  static char *states[] = {
    [UNUSED]    "unused",
    [USED]      "used",
    [SLEEPING]  "sleep ",
    [RUNNABLE]  "runble",
    [RUNNING]   "run   ",
    [ZOMBIE]    "zombie"
  };
  struct proc *p;

  printk("PID\tSTATE\t\tNAME\n");
  for(p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock);
    if(p->state != UNUSED){
      printk("%d\t%s\t%s\n", p->pid, states[p->state], p->name);
    }
    release(&p->lock);
  }
  return 0;
}
uint64
sys_getsyscount(void)
{
  return myproc()->syscall_count;
}
uint64
sys_freemem(void)
{
  return freemem_count();
}
uint64
sys_getppid(void)
{
  struct proc *p = myproc();
  if(p->parent){
    return p->parent->pid;
  }
  return 0; // if there no parent
}
uint64
sys_halt(void)
{
  // QEMU Test Device memory address
  *(volatile uint32 *) 0x100000 = 0x5555; 

  return 0; 
}
uint64
sys_setname(void)
{
  char name[16];
  // argument read from user space
  if(argstr(0, name, 16) < 0)
    return -1;

  struct proc *p = myproc();
  safestrcpy(p->name, name, sizeof(p->name));
  return 0;
}
uint64
sys_getopenfiles(void)
{
  struct proc *p = myproc();
  int count = 0;
  // xv6 process open more than 16 files (NOFILE)
  for(int i = 0; i < 16; i++){
    if(p->ofile[i]){ // if any file open in this index
      count++;
    }
  }
  return count;
}
uint64
sys_getcpuid(void)
{
  int id;
  // Disable interrupts to safely read CPU core ID
  push_off(); 
  id = cpuid();
  // Enable interrupts back
  pop_off();  

  return id;
}
uint64
sys_runnable(void)
{
  struct proc *p;
  int count = 0;

  // Loop through all possible processes
  for(p = proc; p < &proc[NPROC]; p++){
    // Check if the process state is RUNNABLE
    if(p->state == RUNNABLE) {
      count++;
    }
  }

  return count;
}
uint64
sys_getsatp(void)
{
  // Read and return the RISC-V hardware 'satp' register
  return r_satp();
}