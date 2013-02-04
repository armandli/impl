#include <string>
#include <vector>
#include <bitset>
#include <iostream>
using namespace std;

int main(){
  string a = "abc";
  string b = "xyz";
  cout << a + b << endl;
  string c = a + 'k';
  cout << c << endl;

  vector<bool> v;
  for (int i = 0; i < 13; ++i)
    v.push_back(i % 2 ? true : false);
  bitset<v.size()> bs;
  for (int i = 0; i < 13; ++i)
    bs[i] = v[i];
  for (int i = 0; i < 13; ++i)
    cout<<bs[i];
  cout<<endl;
  
  bitset<8> b2('c');
  for (int i = 0; i < 8; ++i)
    cout<<b2[i];
  cout<<endl;
}
