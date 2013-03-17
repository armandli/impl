#include <iostream>
#include "bitcount.h"
using namespace std;

int main(){
    for (unsigned i = 0; i < 0xFFFFFFFF; ++i){
    unsigned j = bitcount(i);
    //cout<<i<<":"<<j<<" ";
    //unsigned ref = iterative_count(i);
    //if (j != ref) cout<<"ERROR:"<<i<<":"<<j<<" "<<ref<<endl;
  }
  //cout<<endl;
}
