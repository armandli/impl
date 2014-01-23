#include "gcd.h"

#define UL unsigned long

UL gcd(UL a, UL b){
  if (b == 0)
    return a;
  return gcd(b, a % b);
}

