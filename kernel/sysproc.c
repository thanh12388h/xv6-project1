#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

extern struct proc proc[NPROC]; 

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
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

uint64
sys_trace(void)
{
  int mask;

  argint(0, &mask);

  myproc()->tracemask = mask;
  return 0;
}

uint64
sys_procinfo(void)
{
  int pid;
  uint64 uaddr;   // địa chỉ user-space của struct procinfo
  struct procinfo kinfo;  // bản copy phía kernel

  argint(0, &pid);
  argaddr(1, &uaddr);  // argaddr(): đọc pointer argument

  // Duyệt process table tìm pid
  struct proc *p;
  int found = 0;
  
  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->pid == pid) {
      kinfo.pid   = p->pid;
      kinfo.state = p->state;
      kinfo.sz    = p->sz;
      kinfo.ppid  = (p->parent) ? p->parent->pid : 0;
      safestrcpy(kinfo.name, p->name, sizeof(kinfo.name));
      found = 1;
      release(&p->lock);
      break;
    }
    release(&p->lock);
  }

  if(!found) return -1;

  // copyout(): copy từ kernel space → user space
  // Cần dùng copyout vì không thể deref pointer user-space trực tiếp
  if(copyout(myproc()->pagetable, uaddr,
             (char*)&kinfo, sizeof(kinfo)) < 0)
    return -1;

  return 0;
}