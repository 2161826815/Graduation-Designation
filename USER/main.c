#include "init.h"

int main(void)
{
  peripheral_init();

  fsm_t m_fsm;
  fsm_init(&m_fsm);

  while(1){
    task_fsm_looper(&m_fsm);
  }

  task_dispatch_looper();
  
  for( ; ;){
  }
}

