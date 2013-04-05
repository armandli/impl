const unsigned int OneasInt = 0x3F800000;
//multiply or devide by 2 for float
const float ScaleUp = float(0x00800000); //2^23 converted to float
const float ScaleDown = 1.0f/ScaleUp;

inline unsigned int asInt(float);
inline float asFloat(unsigned int);
float negateFloat(float);
float absFloat(float);
float flog2(float);
float fexp2(float);
float fpow(float, float);
float fsqrt(float);
float frsqrt(float);
float frsqrt2(float);
