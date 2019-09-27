#include "type.h"

int enqueue(PROC **queue, PROC *p)
{
  PROC *q  = *queue;
  if (q==0 || p->priority > q->priority){
    *queue = p;
    p->next = q;
    return 0;
  }
  while (q->next && p->priority <= q->next->priority){
    q = q->next;
  }
  p->next = q->next;
  q->next = p;
  return 0;
}

PROC *dequeue(PROC **queue)
{
  PROC *p = *queue;
  if (p)
    *queue = p->next;
  return p;
}

int printList(char *name, PROC *list)
{
  printf("%s=", name);
  PROC *p = list;
  while(p){
    printf("[%d%d]->", p->pid, p->priority);
    p = p->next;
  }
  printf("NULL\n");
  return 0;
}