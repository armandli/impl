#include <initializer_list>
#include <array>
#include <iterator>
#include <algorithm>
#include <random>
#include <limits>

// implementation of skip list

constexpr size_t L1CacheSize = 32768UL;

//naive implementation of skip list
template <typename T, size_t L = 32UL>
class skip_list1 {
protected:
  size_t random_level(){
    std::mt19937 mt(mDev());
    int64_t token = mDist(mt);

    token &= (1ULL << L) - 1ULL;
    size_t level = __builtin_ffsll(token);
    return level == 0 ? L - 1 : level - 1;
  }

  struct Node {
    std::array<Node*, L> lvs;
    T v;

    Node(const T& v): v(v) {
      for (auto& lvl : lvs) lvl = nullptr;
    }
    Node(T&& v): v(std::move(v)) {
      for (auto& lvl : lvs) lvl = nullptr;
    }
    ~Node() = default;

    void set_level(size_t lvl, Node* node){ lvs[lvl] = node; }
    void reset(){ for (auto& l : lvs) l = nullptr; }
  private:
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
  };

  Node* search(const T& v) const {
    class SearchRecursion {
      const T& v;
    public:
      SearchRecursion(const T& v): v(v) {}
      Node* operator()(const std::array<Node*, L>& lvls, size_t lvl){
        // boundary case 1: level is empty
        while (lvls[lvl] == nullptr && lvl > 0) --lvl;
        if (lvls[lvl] == nullptr) return nullptr;


        Node& node = *lvls[lvl];
        if (node.v < v)
          return (*this)(node.lvs, lvl);
        else if (lvl == 0)
          return &node;
        else
          return (*this)(lvls, lvl - 1);
      }
    } search(v);

    return search(mLevels, L - 1);
  }

public:
  skip_list1(): mDist(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()) {
    for (auto& lvl : mLevels) lvl = nullptr;
  }
  skip_list1(std::initializer_list<T> il): skip_list1() {
    for (auto v : il) insert(v);
  }
  skip_list1(skip_list1&& o) noexcept :
    mDev(std::move(o.mDev)), mDist(std::move(o.mDist)), mLevels(std::move(o.mLevels)), mSize(o.mSize) {
    for (auto& l : o.mLevels) l = nullptr;
    o.mSize = 0;
  }

  skip_list1& operator=(skip_list1&& o) noexcept {
    mDev = o.mDev;
    mDist = o.mDist;
    std::array<Node*, L> tmp(mLevels);
    mLevels = std::move(o.mLevels);

    Node* prev = nullptr;
    Node* next = tmp[0];
    while (next){
      prev = next;
      next = next->lvs[0];
      delete prev;
    }
    mSize = o.mSize;
    if (&o == this) mSize = 0;
    return *this;
  }

  ~skip_list1(){
    Node* prev = nullptr;
    Node* next = mLevels[0];
    while (next){
      prev = next;
      next = next->lvs[0];
      delete prev;
    }
  }

  class iterator {
  public:
    iterator() = default;
    iterator(Node* n): ptr(n) {}

    T& operator*(){
      assert(ptr != nullptr);
      return ptr->v;
    }

    const T& operator*() const {
      assert(ptr != nullptr);
      return ptr->v;
    }

    void operator++() const { if (ptr) ptr = ptr->lvs[0]; }

    bool operator==(const iterator& o) const {
      return ptr == o.ptr;
    }
    bool operator!=(const iterator& o) const {
      return not this->operator==(o);
    }
  private:
    Node* ptr;
  };

  iterator begin(){ return iterator(mLevels[0]); }
  iterator end(){ return iterator(); }

  bool empty() const {
    return mSize == 0;
  }

  size_t size() const {
    return mSize;
  }

  iterator find(const T& v) const {
    Node* node = search(v);
    if (node && node->v != v)
      node = nullptr;
    return iterator(node);
  }

  iterator lower_bound(const T& v){
    Node* node = search(v);
    return iterator(node);
  }

  iterator insert(const T& v){
    size_t new_level = random_level();
    Node* new_node = new Node(v);

    class InsertRecursion {
      Node&                 new_node;
      size_t                new_level;
    public:
      InsertRecursion(Node& n, size_t nl):
        new_node(n), new_level(nl) {}
      void operator()(std::array<Node*, L>& lvls, size_t lvl){
        while (lvls[lvl] == nullptr && lvl > 0) --lvl;
        // base case: list is empty
        if (lvls[lvl] == nullptr){
          for (size_t i = 0; i <= new_level; ++i)
            lvls[i] = &new_node;
          return;
        }

        Node& node = *lvls[lvl];
        if (node.v < new_node.v){
          (*this)(node.lvs, lvl);
        } else if (lvl == 0){
          new_node.set_level(lvl, lvls[lvl]);
          lvls[lvl] = &new_node;
        } else {
          if (lvl <= new_level){
            new_node.set_level(lvl, lvls[lvl]);
            lvls[lvl] = &new_node;
          }
          (*this)(lvls, lvl - 1);
        }
      }
    } recursion(*new_node, new_level);

    recursion(mLevels, L - 1);
    mSize++;
    return iterator(new_node);
  }

  void erase(const T& v){
    Node* node = search(v);
    if (node == nullptr || node->v != v) return;

    class EraseRecursion {
      Node& deleted;
    public:
      EraseRecursion(Node& n): deleted(n) {}
      void operator()(std::array<Node*, L>& lvls, size_t lvl){
        while (lvls[lvl] == nullptr && lvl > 0) --lvl;
        if (lvls[lvl] == nullptr) return;

        Node& node = *lvls[lvl];
        if (node.v < deleted.v){
          (*this)(node.lvs, lvl);
        } else if (lvl == 0){
          if (lvls[lvl] == &deleted)
            lvls[lvl] = deleted.lvs[lvl];
        } else {
          if (lvls[lvl] == &deleted)
            lvls[lvl] = deleted.lvs[lvl];
          (*this)(lvls, lvl - 1);
        }
      }
    } recursion(*node);

    recursion(mLevels, L - 1);
    node->reset();
    --mSize;
    delete node;
  }

  void clear(){
    Node* prev = nullptr;
    Node* next = mLevels[0];
    while (next){
      prev = next;
      next = next->lvs[0];
      delete prev;
    }
    for (auto& n : mLevels)
      n = nullptr;
    mSize = 0;
  }

private:
  std::random_device                     mDev;
  std::uniform_int_distribution<int64_t> mDist;
  std::array<Node*, L>                   mLevels;
  size_t                                 mSize;
};

//optimization1: try to allocate nodes batched together
//optimization2: try sharing levels with a batch of nodes, and do only linear search within the batch (use optimal batch of course)
