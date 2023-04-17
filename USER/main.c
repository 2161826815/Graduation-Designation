#include "main.h"
#include "init.h"


int main(void)
{
  static uint32_t pre_tick;
  uint32_t cur_tick;
  peripheral_init();

  while(1){
    /* pre_tick = get_tick();
    mpu6050_task();
    cur_tick = get_tick();
    printf("delay:%d\r\n",cur_tick-pre_tick); */
    cur_tick = get_tick();
    if(cur_tick != pre_tick){  //确保单片机正常运行
      pre_tick++;
      task_dispatch();
    }else{
      
    }
  }
}

