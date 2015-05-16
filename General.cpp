// General functions

#include "General.h"

int Sign(float var)
{
  if(var < 0)
    return -1;
  if(var == 0)
    return 0;
  if(var > 0)
    return 1;  
}
