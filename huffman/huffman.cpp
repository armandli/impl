#include "huffman.h"
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <bitset>
#include <algorithm>
#include <iostream>

#ifdef DEBUG
#include <cassert>
#endif

void massert(bool cond){
#ifdef DEBUG
  assert(cond);
#endif
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

//huffman encoding implementation

//huffman tree node
struct treeNode {
  char val;
  unsigned long freq;
  treeNode *lchild, *rchild;
  treeNode() : lchild(NULL), rchild(NULL), val('\0'), freq(0) {}
  treeNode(treeNode* l, treeNode* r) : freq(l->freq + r->freq), lchild(l), rchild(r) {}
  treeNode(char v, int f = 0) : val(v), freq(f), lchild(NULL), rchild(NULL) {}
  ~treeNode(){ 
    if (lchild) delete lchild; 
    if (rchild) delete rchild; 
  }
};

struct priority_compare {
  bool operator() (const treeNode* a, const treeNode* b){ return a->freq > b->freq; }
};

bool isLeaf(const treeNode* t) {
  // all internal nodes must have 2 children
  if (t->lchild == NULL || t->rchild == NULL) return true;
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
        //boundary case, root is leaf
        if (!prefix.length())
          prefix = "1";
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
  for (unsigned long i = 0; i < code.length() / 8; ++i){
    std::bitset<8> bs(code, i*8, (i*8)+8);
    compressed += (char)bs.to_ulong();
  }
  //encode the last <8 bit code compressed string and append mask
  int roundLength = code.length() % 8;
  if (roundLength) {
    std::string lastSegment = code.substr(code.length() - roundLength, roundLength);
    std::string mask;
    for (int i = 0; i < roundLength; ++i)
      mask += '1';
    for (int i = 0; i < 8 - roundLength; ++i) {
      lastSegment += '0';
      mask += '0';
    }
    std::bitset<8> bs(lastSegment);
    compressed += (char)bs.to_ulong();
    std::bitset<8> bmask(mask);
    compressed += (char)bmask.to_ulong();
  } else {
    std::string mask = "11111111";
    std::bitset<8> bs(mask); // the last byte is always a mask
    compressed += (char)bs.to_ulong();
  }
}

#ifdef DEBUG
std::iostream& operator << (std::iostream& s, std::map<char, std::string> map){ for (std::map<char, std::string>::iterator it = map.begin(); it != map.end(); ++it) s << "key = " << (*it).first << " val = " << (*it).second << std::endl;
  return s;
}
#endif

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
  massert(q.size() > 0);
  //produce huffman tree out of the sorted priority queue
  while (q.size() != 1){
    treeNode* l = q.top(); q.pop();
    treeNode* r = q.top(); q.pop();
    treeNode* t = new treeNode(l, r);
    q.push(t);
  }
  massert(q.size() == 1);
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


  //write out the compressed string
  std::string compressed;
  compress(encoded_tree + encoded, compressed);

#ifdef DEBUG
  std::cout << "encode tree(" << encoded_tree.length() << ") = \n" << encoded_tree << std::endl;
  std::cout << "encoded(" << encoded.length() << ") = \n" << encoded << std::endl;
#endif

  delete root;
  return compressed;
}

// class help load only a portion of the compressed string data at any moment
class CompressedFrame{
#define BYTE 8
  private:
  const std::string _encoded;
  unsigned long _index;
  unsigned int _findex;
  unsigned int _findexMax;
  std::string _expanded;
  bool loadNext(){
    if (_index < _encoded.length() - 2) { // normal case
      std::bitset<BYTE> bs(_encoded[_index]);
      _expanded = bs.to_string();
      _index++;
      return true;
    } else if (_index == _encoded.length() - 2){ //special case, need the mask to set _findexMax
      std::bitset<BYTE> bs(_encoded[_encoded.length() - 2]);
      std::bitset<BYTE> bmask(_encoded[_encoded.length() - 1]);
      _expanded = bs.to_string();
      std::string bmaskstr = bmask.to_string();
      int ones = std::count(bmaskstr.begin(), bmaskstr.end(), '1'); 
      _findexMax = ones;
      _index = _encoded.length();
      return true;
    }
    return false;
  }
  public:
  explicit CompressedFrame(const std::string encoded) : _encoded(encoded), _index(0), _findex(0), _findexMax(BYTE) {
    if (_encoded.length() < 3) { 
      std::cerr << "String too short" << std::endl; 
      _index = _encoded.length();
      _findex = BYTE;
    }
    loadNext();
  }
  bool nextBit(char& c){
    if (_findex == _findexMax) { 
      if (loadNext()){
        _findex = 0;
        c = _expanded[_findex++];
        return true;
      }
      return false;
    } 
    c = _expanded[_findex++];
    return true;
  }
};

char bitStringToChar(std::string str){
  std::bitset<8> bs(str);
  return (char) bs.to_ulong();
}

bool decodeNext(treeNode* root, CompressedFrame& frames, char& c){
  char k;
  treeNode* node = root;
  //special case, root is leaf
  if (isLeaf(node)) { 
    c = node->val;
    return frames.nextBit(k);
  }
  while (!isLeaf(node)){
    if (!frames.nextBit(k)) { std::cerr << "unexpected file ending" << std::endl; return false; }
    if (k == '0')
      node = node->lchild;
    else 
      node = node->rchild;
  }
  c = node->val;
  return true;
}

std::string huffman_decompress(std::string str){
  CompressedFrame frames(str);
  char c;
#ifdef DEBUG
  std::cout << "compressed string bits:\n";
  CompressedFrame printBits(str);
  while (printBits.nextBit(c)){
    std::cout<<c;
  }
  std::cout<<std::endl;
#endif
  // reconstruct the compression tree
  if (!frames.nextBit(c)) { std::cerr << "unexpected file ending" << std::endl; return ""; }
  treeNode* root;
  if (c == '1'){
    std::string val;
    for (int i = 0; i < BYTE; ++i){
      if (!frames.nextBit(c)) { std::cerr << "unexpected file ending" << std::endl; return ""; }
      val += c;
    }
    c = bitStringToChar(val);
    root = new treeNode(c);
  } else {
    root = new treeNode();
    std::queue<treeNode*> q;
    q.push(root);
    while (!q.empty()){
      treeNode* node = q.front(); q.pop();
      if (!frames.nextBit(c)) { std::cerr << "unexpected file ending" << std::endl; return ""; }
      if (c == '0') {
        treeNode* l = new treeNode();
        node->lchild = l;
        q.push(l);
      } else {
        std::string val;
        for (int i = 0; i < BYTE; ++i){
          if (!frames.nextBit(c)) { std::cerr << "unexpected file ending" << std::endl; return ""; }
          val += c;
        }
        c = bitStringToChar(val);
        treeNode* l = new treeNode(c);
        node->lchild = l;
      }
      if (!frames.nextBit(c)) { std::cerr << "unexpected file ending" << std::endl; return ""; }
      if (c == '0') {
        treeNode* r = new treeNode();
        node->rchild = r;
        q.push(r);
      } else {
        std::string val;
        for (int i = 0; i < BYTE; ++i){
          if (!frames.nextBit(c)) { std::cerr << "unexpected file ending" << std::endl; return ""; }
          val += c;
        }
        c = bitStringToChar(val);
        treeNode* r = new treeNode(c);
        node->rchild = r;
      }
    }
  }
#ifdef DEBUG
  //TODO: reconstruct compression map
#endif

  //read and reconstruct data
  std::string data;
  while (decodeNext(root, frames, c))
    data += c;
  return data;
}
