#include "ternary_tree.h"

TTMap::TTMap() : root(NULL), sz(0) {}

TTMap::TTMap(const TTMap& other){
  class recursiveCopy{
  public:
    node* operator()(const node* const n){
      if (!n) return NULL;
      node* newnode = new node(*n);
      newnode->lo = (*this)(n->lo);
      newnode->hi = (*this)(n->hi);
      newnode->eq = (*this)(n->eq);
      return newnode;
    }
  };
  if (root) delete root;
  recursiveCopy rc;
  root = rc(other.root);
  sz = other.sz;
}

TTMap::~TTMap(){ delete root; }

//helper function to find the matching last node
TTMap::node* TTMap::nfind(const std::string& key){
  node* ne(root),* pre(NULL);
  size_t i = 0;
  while (ne && i < key.length()){
    pre = ne;
    if      (key[i] < ne->k)      ne = ne->lo;
    else if (key[i] > ne->k)      ne = ne->hi;
    else                         {ne = ne->eq; i++;}
  }
  if (i < key.length() || !pre || !pre->val) return NULL;
  return pre;
}

void TTMap::add(const std::string& key){
  if (key.length() == 0) return;
  node* ne(root),* pre(root);
  enum link_state { low, equal, high };
  link_state ls = equal;
  size_t i = 0;
  while (ne && i < key.length()){
    pre = ne;
    if (key[i] < ne->k)      {ne = ne->lo; ls = low;}
    else if (key[i] > ne->k) {ne = ne->hi; ls = high;}
    else                     {ne = ne->eq; i++; ls = equal;}
  }
  //partial key found
  if (i < key.length()){
    node* tmp = new node(key[i++]),* tmpre;
    ne = tmp;
    for (; i < key.length(); ++i, tmp = tmp->eq){
      tmpre = tmp;
      tmp->eq = new node(key[i]);
    }
    tmp->val = true;
    if (pre){
      switch (ls){
        case low: pre->lo = ne; break;
        case equal: pre->eq = ne; break;
        case high: pre->hi = ne; break;
      }
    } else
      root = ne;
    sz++;
  //key already exist, just not considered a key
  } else if (!pre->val){
    pre->val = true;
    sz++;
  }
}

bool TTMap::remove(const std::string& key){
  node* n = nfind(key);
  if (!n) return false;
  //not going to trim the tree
  sz--;
  n->val = false;
  return true;
}

bool TTMap::find(const std::string& key){
  node* n = nfind(key);
  if (n) return true;
  return false;
}

std::vector<std::string> TTMap::print(void){
  class PrefixTraverse{
  public:
    void operator() (const node* const n, std::string prefix, std::vector<std::string>& lst){
      if (!n) return;
      if (n->val) { 
        std::string v = prefix + n->k;
        lst.push_back(v);
      }
      (*this)(n->lo, prefix, lst);
      (*this)(n->hi, prefix, lst);
      (*this)(n->eq, prefix + n->k, lst);
    }
  };
  std::vector<std::string> vals;
  PrefixTraverse pt;
  pt(root, "", vals);
  return vals;
}

size_t TTMap::size(void){ return sz; }

TTMap::node::node() : k('\0'), eq(NULL), lo(NULL), hi(NULL), val(false){}
TTMap::node::node(char c) : k(c), eq(NULL), lo(NULL), hi(NULL), val(false){}

TTMap::node::node(const node& other){
  this->k = other.k;
  this->lo = other.lo;
  this->hi = other.hi;
  this->eq = other.eq;
  this->val = other.val;
}

TTMap::node::~node(){
  if (eq) delete eq;
  if (lo) delete lo; 
  if (hi) delete hi;
}
