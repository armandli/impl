#include <iostream>
#include <string>
#include <vector>
#include "ternary_tree.h"
using namespace std;

int main(){
  TTMap ts;
  int n; cin>>n;
  string str;
  while (n--){
    cin>>str;
    cout<<"add "<<str<<endl;
    ts.add(str);
  }
  getline(cin, str);
  cin>>n;
  while (n--){
    cin>>str;
    cout<<"remove "<<str<<endl;
    ts.remove(str);
  }
  cin>>n;
  while (n--){
    cin>>str;
    cout<<"find "<<str<<endl;
    cout << ts.find(str) << endl;
  }
  vector<string> vs = ts.print();
  for (size_t i = 0; i < vs.size(); ++i)
    cout<<vs[i]<<endl;
}
