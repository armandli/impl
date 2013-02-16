#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h" 
using namespace std;

int main(int argc, char* argv[]){
  string test = "abcdefg";
  string compressed = huffman_compress(test);
  cout << "test1 = " << test << "\ncompressed = " << compressed << endl;

  test = "aaabbb";
  compressed = huffman_compress(test);
  cout << "test2 = " << test << "\ncompressed = " << compressed << endl;
}
