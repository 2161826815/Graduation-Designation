#include "main.h"
#include "init.h"

int main(void)
{
  peripheral_init();

  task_looper();

  while(1){
    tim3_init((TIM_IT_TIME*10-1),7199);
    mpu6050_task();
    delay_ms(30);
  }
}

