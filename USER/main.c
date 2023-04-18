#include "main.h"
#include "init.h"

extern data_buff_t all_data;
int main(void)
{
  int ret;
  static uint32_t pre_tick;
  uint32_t cur_tick;
  peripheral_init();

  while(1){
    /* pre_tick = get_tick();
	  while(mpu_dmp_get_data(&all_data.pitch,&all_data.roll,&all_data.yaw));
    printf("Time:%d\r\n",get_tick()-pre_tick); */
    //printf("pitch:%.2f roll:%.2f yal:%.2f \r\n",all_data.pitch,all_data.roll,all_data.yaw);
    cur_tick = tim_get_tick();
    if(cur_tick != pre_tick){  
      pre_tick++;
      task_dispatch();
    }else{
      
    }
  }
}

