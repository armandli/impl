// compute the nearest power of 2, rounded up
// return the value that is a power of 2
unsigned nearest_power_2(unsigned v){
  v--;
  v |= v >> 1U;
  v |= v >> 2U;
  v |= v >> 4U;
  v |= v >> 8U;
  v |= v >> 16U;
  v++;
  return v;
}
