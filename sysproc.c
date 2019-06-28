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


/* La nueva llamada a sistema getprocs(void), retorna un contador
que indica el numero de procesos que estan siendo usados por
el OS en el momento de su ejecución.
*/
int
getprocs(void){
  struct proc *p;
  int count = 0;
  acquire(&ptable.lock); //solicitamos un lock para evitar que otro thread comience esta tarea
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state != UNUSED && p->state != ZOMBIE){
      //Permite verificar que el estado del proceso en iteracion no sea UNUSED (no usado)
      // o que sea un proceso ZOMBIE (es un proceso que ha completado la ejecución 
      //(a través de la llamada exit) pero aún tiene una entrada en la tabla de procesos)
      count++; //se aumenta el contador
    }
  }
  release(&ptable.lock);
  return count;
}
