#include "stm32f10x.h"
#include "misc.h"

int main()
{
  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);



  return 0;
}