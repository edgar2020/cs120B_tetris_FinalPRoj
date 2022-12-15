#include "RNG.h"

RNG::RNG()
{
  seed = 1;//change so last seed is stored in eeprom
}

void RNG::setSeed( long s)
{
  seed = s;
}
long RNG::newRNG()
{
  hi = (long)(seed/q);
  lo = seed - q * hi;
  test = a * lo - r * hi;

  if(test > 0)
  {
    seed = test;
  }
  else
  {
    seed = test + m;
  }
  return seed; //I always get rid of the divided by m since it always gives me 0
}