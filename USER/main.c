#include "main.h"
#include "init.h"

int main(void)
{
  peripheral_init();

  task_looper();

  while(1){

  }
}

