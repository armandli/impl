#include "floatop.h"

inline unsigned int asInt(float f){
  return *(unsigned int *)&f;
}

inline float asFloat(unsigned int i){
  return *(float *)&i;
}

//Warning: these tricks does not handle Nan propagation

float NegateFloat(float f){
  return asFloat(asInt(f)^0x80000000);
}

float absFloat(float f){
  return asFloat(asInt(f)&0x7FFFFFFF);
}

//the idea is log10(1.3672*10^7) = 7.13672, like shift down the value and put
//exponent before the decimal point. we can do the same for float with base 2 
//exponent
//warning: no error checking, so negative values will give random wrong values
float flog2(float x){
  return float (asInt(x)-OneasInt) * ScaleDown;
}

float fexp2(float x){
  return asFloat(int(x*ScaleUp) + OneasInt);
}

//based on the identity: x^p = 2^(p * log2(x))
//only close approximation
float fpow(float x, float p){
  return asFloat(int(p*(asInt(x) - OneasInt)) + OneasInt);
}

//returns random wrong values if x is negative; this is 3 times faster than hardware
//NOTE: you may want to do newton rapson aftwards to increase accuracy.
//newton raphson:
// y = sqrt(x);
// y = (y * y + x) / (2 * y);
// or
// y = y - (y * y - x) / (2 * y);
// newton rapson is only good for 1-2 times, after that it will take just as much time
// as hardware
float fsqrt(float x){
  return asFloat((asInt(x) >> 1) + (OneasInt >> 1));
}

//inverse square root from quake 3
float frsqrt(float f){
  int i;
  float x2, y;
  const float threehalfs = 1.5F;
  x2 = f * 0.5F;
  y = f;
  i = asInt(f);
  i = 0x5F3759DF - (i >> 1);
  y = asFloat(i);
  y = y * ( threehalfs - (x2 * y * y) ); //newton raphson
  return y;
}

//inverse square root with more accuracy
float frsqrt2(float f){
  float xhalf = 0.5F*f;
  int i = asInt(f);
  i = 0x5F375A86 - (i>>1);
  f = asFloat(i);
  f = f * (1.5F - xhalf * f * f);
  return f;
}

//more optimal inverse squareroot
float frsqrt3(float f){
  uint32_t i = asInt(f);
  i = 0x5F1FFFF9U - (i >> 1);
  float f2 = toFloat(i);
  return 0.703952253F * f2 * (2.38924456F - f * f2 * f2);
}
