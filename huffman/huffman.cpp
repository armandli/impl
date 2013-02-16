#include "huffman.h"
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <boost/dynamic_bitset.hpp>


//huffman encoding implementation

//huffman tree node
struct treeNode {
  char val;
  unsigned long freq;
  treeNode *lchild, *rchild;
  treeNode(treeNode* l, treeNode* r) : freq(l->freq + r->freq), lchild(l), rchild(r) {}
  treeNode(char v, int f) : val(v), freq(f), lchild(NULL), rchild(NULL) {}
  ~treeNode(){ delete lchild; delete rchild; }
};

struct priority_compare {
  bool operator() (const treeNode* a, const treeNode* b){ return a->freq < b->freq; }
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
  //assert(q.size() > 0);
  //produce huffman tree out of the sorted priority queue
  while (q.size() != 1){
    treeNode* l = q.top(); q.pop();
    treeNode* r = q.top(); q.pop();
    treeNode* t = new treeNode(l, r);
    q.push(t);
  }
  treeNode* root = q.top(); q.pop();
  //encode each charater in original string using huffman coding tree
  std::map<char, std::string> encoding_map;
  encode(root, encoding_map);
  
  //convert input string into encoding string
  std::string encoded;
  for (std::string::size_type i = 0; i < str.length(); ++i)
    encoded += encoding_map[str[i]];
  //produce a compressed tree
  //write out the compressed string
  std::string compressed;
 
  delete root;
  return compressed;
}

std::string huffman_decompress(std::string str){
  //TODO: read the compression tree
  //TODO: read the rest of the string to decompress to the original data
  std::string data;
  return data;
}
