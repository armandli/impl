#include "huffman.h"
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <bitset>
#include <iostream>

#include <cassert>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//huffman encoding implementation

//huffman tree node
struct treeNode {
  char val;
  unsigned long freq;
  treeNode *lchild, *rchild;
  treeNode(treeNode* l, treeNode* r) : freq(l->freq + r->freq), lchild(l), rchild(r) {}
  treeNode(char v, int f) : val(v), freq(f), lchild(NULL), rchild(NULL) {}
  ~treeNode(){ 
    if (lchild) delete lchild; 
    if (rchild) delete rchild; 
  }
};

struct priority_compare {
  bool operator() (const treeNode* a, const treeNode* b){ return a->freq > b->freq; }
};

bool isLeaf(const treeNode* t) {
  if (t->lchild == NULL && t->rchild == NULL) return true;
  return false;
}

//count the frequency of each character in string
void count_freq(const std::string& str, std::map<char, unsigned long>& f){
  for (unsigned long i = 0; i < str.length(); ++i)
    if (f.find(str[i]) == f.end())
      f[str[i]] = 1;
    else
      f[str[i]] += 1;
}

//encode each character using the huffman tree
void encode(const treeNode* root, std::map<char, std::string>& encoding){
  struct converter{
    void operator () (const treeNode* t, std::map<char, std::string>& encoding, std::string prefix = ""){
      if (isLeaf(t)){
        encoding[t->val] = prefix;
        return;
      }  
      (*this)(t->lchild, encoding, prefix + '0');
      (*this)(t->rchild, encoding, prefix + '1');
    }
  };
  converter encoder;
  encoder(root, encoding);
}

void encodeTree(treeNode* root, std::string& encoded){
  //observation: tree internal nodes always have 2 children
  //we'll encode the tree from top down level by level, 0 means internal node, 1 means leaf,
  //the 8 bits after a leaf is the encoded character
  std::queue<treeNode*> q; q.push(root);
  while (!q.empty()){
    treeNode* node = q.front(); q.pop();
    if (isLeaf(node)) {
      encoded += "1";
      std::bitset<8> bs(node->val);
      encoded += bs.to_string();
    } else {
      encoded += "0";
      q.push(node->lchild);
      q.push(node->rchild);
    }
  }
}

void compress(const std::string code, std::string& compressed){
  for (unsigned long i = 0; i < code.length(); i += 8){
    std::bitset<8> bs(code, i, MIN(i+8,code.length()));
    compressed += (char)bs.to_ulong();
  }
}

std::iostream& operator << (std::iostream& s, std::map<char, std::string> map){ for (std::map<char, std::string>::iterator it = map.begin(); it != map.end(); ++it) s << "key = " << (*it).first << " val = " << (*it).second << std::endl;
  return s;
}

std::string huffman_compress(std::string str){
  //count the frequency of occurance of each character
  std::map<char, unsigned long> data_count;
  count_freq(str, data_count);
  //place each character with frequency in a queue, sort by least frequency
  std::priority_queue< treeNode*, std::vector<treeNode*>, priority_compare > q;
  std::map<char, unsigned long>::iterator iter = data_count.begin();
  for (; iter != data_count.end(); ++iter){
    treeNode* node = new treeNode((*iter).first, (*iter).second);
    q.push(node);
  }
  assert(q.size() > 0);
  //produce huffman tree out of the sorted priority queue
  while (q.size() != 1){
    treeNode* l = q.top(); q.pop();
    treeNode* r = q.top(); q.pop();
    treeNode* t = new treeNode(l, r);
    q.push(t);
  }
  assert(q.size() == 1);
  treeNode* root = q.top(); q.pop();
  //encode each charater in original string using huffman coding tree
  std::map<char, std::string> encoding_map;
  encode(root, encoding_map);
  
  //convert input string into encoding string
  std::string encoded;
  for (std::string::size_type i = 0; i < str.length(); ++i)
    encoded += encoding_map[str[i]];
  //produce a compressed tree
  std::string encoded_tree;
  encodeTree(root, encoded_tree);

  std::cout << "encode tree = \n" << encoded_tree << std::endl;
  std::cout << "encoded = \n" << encoded << std::endl;

  //write out the compressed string
  std::string compressed;
  compress(encoded_tree + encoded, compressed);
 
  delete root;
  return compressed;
}

std::string huffman_decompress(std::string str){
  //TODO: read the compression tree
  //TODO: read the rest of the string to decompress to the original data
  std::string data;
  return data;
}
