#include <string>
#include <vector>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <iostream>
using namespace std;

int main(){
  string a = "abc";
  string b = "xyz";
  cout << a + b << endl;
  string c = a + 'k';
  cout << c << endl;

  //bitset must take a const length value
  bitset<8> b2('c');
  for (int i = 7; i >= 0; --i)
    cout<<b2[i];
  cout<<endl;
  cout << b2.to_string() << endl;
  char k('\0');
  for (int i = 0; i < 8; ++i)
    k = b2.to_ulong();
  cout << k << endl;

  string str = "001001100010011100101111";
  bitset<8> b3(str, 8, 16);
  cout << b3.to_string() << endl;
  k = b3.to_ulong();
  cout << k << endl;

  //boost dynamic size bitset
  str = "1101"; //string must be using only 1s and 0s
  boost::dynamic_bitset<> bs(str);
  for (boost::dynamic_bitset<>::size_type i = 0; i < bs.size(); ++i)
    cout<<bs[i];
  cout<<endl;
}
