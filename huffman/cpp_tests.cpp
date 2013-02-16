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

  //boost dynamic size bitset
  string str = "1101"; //string must be using only 1s and 0s
  boost::dynamic_bitset<> bs(str);
  for (boost::dynamic_bitset<>::size_type i = 0; i < bs.size(); ++i)
    cout<<bs[i];
  cout<<endl;
}
