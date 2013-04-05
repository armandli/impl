#include <iostream>
#include <cmath>
#include "floatop.h"
using namespace std;

float negate(float f){
  return f * -1.;
}

float invsqrt(float f){
  return 1. / sqrt(f);
}

void test(float (*ffun)(float), float (*fun)(float), float val, int testid){
  cout<< testid << ") val = " << val << "; " << ffun(val) << " vs " << fun(val) << endl;
}

int main(){
  int n; cin>>n;
  for (int i = 1; i <= n; ++i){
    float f; cin>>f;
    test(absFloat, abs, f, i);
  }
  cin>>n;
  for (int i = 1; i <= n; ++i){
    float f; cin>>f;
    test(fsqrt, sqrt, f, i);
  }
  cin>>n;
  for (int i = 1; i <= n; ++i){
    float f; cin>>f;
    test(frsqrt, invsqrt, f, i);
  }
  cin>>n;
  for (int i = 1; i <= n; ++i){
    float f; cin>>f;
    test(frsqrt2, invsqrt, f, i);
  }
}
