#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h" 
using namespace std;

void test(string str, int id){
  string ret = huffman_compress(str);
  cout << "test" << id << " = " << str << "\ncompr = " << ret << endl;
  string uncompressed = huffman_decompress(ret);
  cout << "uncompressed:\n" << uncompressed << std::endl;
}

int main(int argc, char* argv[]){
  test("abcdefg",1);
  test("aaabbb",2);
  test("abcabcabcabc",3);
  test("aaabbbbb",4);
  test("aaaaaa",5);
  test("a", 6);
  test("baaccc", 7);
}
