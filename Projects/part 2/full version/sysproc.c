#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// HelloWorld system process
int
sys_HelloWorld(void)
{
  return HelloWorld();
}

// Calls the function which returns the number of active/used processes
int
sys_getProcCount(void)
{
  return getProcCount();
}

// Calls the function which retuns the number of Read system-calls since boot
int
sys_getReadCount(void)
{
  return getReadCount();
}

// Calls the function which does the processing for unit 0
int
sys_unit0_operation(void)
{
  int value;

  // Stores the 0th int parameter into value.
  if(argint(0, &value) < 0)
    return -1;

  return unit0_operation(value);
}

// Calls the function which does the processing for unit 1
int
sys_unit1_operation(void)
{
  int value;

  // Stores the 0th int parameter into value.
  if(argint(0, &value) < 0)
    return -1;

  return unit1_operation(value);
}

// Calls the function which does the processing for unit 2
int
sys_unit2_operation(void)
{
  int value;

  // Stores the 0th int parameter into value.
  if(argint(0, &value) < 0)
    return -1;

  return unit2_operation(value);
}

// Calls the function which does the processing for unit 3
int
sys_unit3_operation(void)
{
  int value;

  // Stores the 0th int parameter into value.
  if(argint(0, &value) < 0)
    return -1;
  return unit3_operation(value);
}