//count number of bits in an unsigned int
//using merge count
unsigned bitcount(unsigned k){
  k = k - ((k>>1) & 0x55555555);    //count # of bits for every 2 bits
  k = (k & 0x33333333) + ((k>>2) & 0x33333333); //sum for every 4 bits
  k = (k + (k>>4)) & 0x0F0F0F0F;                //sum for every 8 bits
  k = (k * 0x01010101) >> 24;                   //sum all 4 bytes
  return k;
}

//count # of bits in int iteratively
unsigned iterative_count(unsigned k){
  unsigned sum = 0;
  while (k){
    k = (k - 1) & k;
    sum++;
  }
  return sum;
}

unsigned bitcount(unsigned long long k){
  return sizeof(unsigned long long);
}
