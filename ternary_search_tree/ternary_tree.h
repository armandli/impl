//Implementation of ternary search tree as map for illustration purposes
#include <iostream>
#include <string>
#include <vector>

class TTMap {
public:
  TTMap();
  TTMap(const TTMap&);
  ~TTMap();
  void add(const std::string& key);
  bool remove(const std::string& key);
  bool find(const std::string& key);
  std::vector<std::string> print(void);
  size_t size(void);
private:
  struct node{
    char k;
    bool val;
    node* eq, * lo, * hi;
    explicit node(char);
    node(const node& other);
    node();
    ~node();
  };
  node* root;
  size_t sz;

  node* nfind(const std::string& key);
};

