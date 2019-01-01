#include <cstring>
#include <string>

namespace s = std;

constexpr size_t ALPHABET_SIZE = 256;

//naive Trie wastes a lot of space because each child does not have full alphabet scope,
//but still reserve space for each
class NaiveTrie {
  struct Node {
    Node* children[ALPHABET_SIZE];
    bool end;

    Node(): end(false) {
      s::memset(children, 0, sizeof(Node*) * ALPHABET_SIZE);
    }
    ~Node(){
      for (Node* child : children)
        if (child)
          delete child;
    }
  };

  Node mRoot;

  const Node* find(const s::string& s) const {
    const Node* pn = &mRoot;
    for (char ch : s){
      unsigned char uch = ch;
      if (pn->children[uch])
        pn = pn->children[uch];
      else {
        pn = nullptr;
        break;
      }
    }
    if (pn && pn->end) return pn;
    else               return nullptr;
  }
public:
  ~NaiveTrie(){
    for (Node*& child : mRoot.children)
      if (child){
        delete child;
        child = nullptr;
      }
  }

  bool insert(const s::string& s){
    Node* pn = &mRoot;
    for (char ch : s){
      unsigned char uch = ch;
      if (pn->children[uch] == nullptr)
        pn->children[uch] = new Node();
      pn = pn->children[uch];
    }
    pn->end = true;
    return true;
  }
  bool exist(const s::string& s) const {
    const Node* ptr = find(s);
    return ptr != nullptr;
  }
  bool remove(const s::string& s){
    Node* ptr = const_cast<Node*>(find(s));
    if (ptr){
      ptr->end = false;
      return true;
    } else
      return false;
  }
  size_t size() const {
    struct Recursion {
      size_t operator()(const Node* node){
        size_t ret = node->end == true;
        for (const Node* child : node->children)
          if (child)
            ret += (*this)(child);
        return ret;
      }
    } recursion;
    return recursion(&mRoot);
  }
};
