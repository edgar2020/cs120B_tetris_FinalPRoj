#ifndef RNG_H
#define RNG_H

using namespace std;

class RNG {

 private:
  long seed;
  const long m = 2147483647;
  const long a = 16807;
  const long q = 127773;
  const long r = 2836;
  long lo, hi, test;
  
 public:
 RNG();
 void setSeed( long);
 long getSeed() { return seed; }
 long newRNG();
  
};

#endif