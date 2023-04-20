#ifndef _IIC_H_
#define _IIC_H_
#include "sys.h"
#include "SysTick.h"
#define IIC_SLAVE_ADDR 0x78

void Write_IIC_Command(uint8_t IIC_Command);
void Write_IIC_Data(uint8_t IIC_Data);

#endif
