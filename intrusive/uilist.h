/* Circular Unidirectional Intrusive List */
#include <cassert>
#include <cstddef>

#ifndef __UNIDIRECTIONAL_INTRUSIVE_LIST__
#define __UNIDIRECTIONAL_INTRUSIVE_LIST__

template <class Node, int N> class UIntrLstNd;   //node definition
template <class Node, int N> class UIntrLst;     //method collection class
template <class Node, int N> class UIntrLstCItr; //const iterator
template <class Node, int N> class UIntrLstItr;  //iterator

/** UIntrLstNd
 *    Usage:  sub-class this to become an intrusive list node type; a class can
 *            be part of multiple intrusive lists by using different N template
 *            parameter.
 *    Safety: intrusive list node cannot be deconstructed until it is released
 *            from the list
 *    Note:   An object of this type is not part of any list if mNxt is nullptr;
 *            An iterator will not iterate through objects not belonging to any
 *            list.
 */
template <class Node, int N = 0>
class UIntrLstNd {
  using tLstNd = UIntrLstNd<Node, N>;
  friend class UIntrLst<Node, N>;
  friend class UIntrLstCItr<Node, N>;

  tLstNd* mNxt;
public:
  UIntrLstNd() : mNxt(nullptr) {}
  ~UIntrLstNd(){ assert(mNxt == nullptr); }
};

/** Intrusive List Const Iterator
 */
template <class Node, int N = 0>
class UIntrLstCItr {
  using tLstNd  = UIntrLstNd<Node, N>;
  using tLstItr = UIntrLstCItr;

  const tLstNd* mEnd;
  const tLstNd* mCur;

  void init(const tLstNd* start){
    if (!start || start->mNxt == nullptr) mCur = mEnd = nullptr;
    else                                  mEnd = mCur = start;
  }
public:
  static tLstItr end(){ return tLstItr(); }

  UIntrLstCItr() : mEnd(nullptr), mCur(nullptr) {}
  explicit UIntrLstCItr(const tLstNd& s){ init(&s); }
  explicit UIntrLstCItr(const tLstNd* s){ init(s); }
  explicit UIntrLstCItr(const tLstItr& o) : mEnd(o.mEnd), mCur(o.mCur) {}
  tLstItr& operator=(const tLstItr& o){
    mEnd = o.mEnd;
    mCur = o.mCur;
    return *this;
  }
  bool operator==(const tLstItr& o) const {
    return mEnd == o.mEnd && mCur == o.mCur;
  }
  bool operator!=(const tLstItr& o) const { return !(this->operator==(o)); }
  tLstItr& operator++(){
    assert(mCur != nullptr);
    if (mCur->mNxt == mEnd) mCur = mEnd = nullptr;
    else                    mCur = mCur->mNxt;
    return *this;
  }
  tLstItr operator++(int){ tLstItr tmp(*this); this->operator++(); return tmp; }
  const Node& operator*() const {
    assert(mCur != nullptr);
    return static_cast<const Node&>(*mCur);
  }
};

/** Intrusive List Iterator
 *    (implemented using Intrusive List Const Iterator)
 */
template <class Node, int N = 0>
class UIntrLstItr {
  using tLstNd  = UIntrLstNd<Node, N>;
  using tLstItr = UIntrLstItr;

  UIntrLstCItr<Node, N> mItr;
public:
  static tLstItr end(){ return tLstItr(); }

  UIntrLstItr() : mItr() {}
  explicit UIntrLstItr(const tLstNd& start) : mItr(start) {}
  explicit UIntrLstItr(const tLstNd* start) : mItr(start) {}
  UIntrLstItr(const tLstItr& other) : mItr(other.mItr) {}
  tLstItr& operator=(const tLstItr& other){ mItr = other.mItr; return *this; }
  bool operator==(const tLstItr& other) const {
    return mItr.operator==(other.mItr);
  }
  bool operator!=(const tLstItr& other) const {
    return mItr.operator!=(other.mItr);
  }
  tLstItr& operator++(){ ++mItr; return *this; }
  tLstItr operator++(int){ tLstItr tmp(*this); mItr++; return tmp; }
  Node& operator*() const { return const_cast<Node&>(mItr.operator*()); }
};

/** Intrusive List
 *    Two Usages:
 *      1) use as static wrapper class for all intrusive list functionalities
 *      2) use as virtual container of list if initialized with a list node,
 *         which will be used as root node for the lifetime of this object
 *
 *  Interface operations:
 *    size:    obtain the number of nodes connected
 *    create:  create a list of 1 element from a node
 *    release: release all nodes from a list, node not deallocated
 *    destroy: release and delete all nodes from list, may supply custom
 *             destructor function
 *    insert:  put node nb after node na, assume nb does not belong to a list
 *             larger than size 1, and na is a list
 *    remove:  remove the node nb linked after node na, if nb == na, na
 *             is released
 *    merge:   merge list nb after list na; assume both na and nb are lists
 *    begin:   return an iterator object starting from its given param
 *    cbegin:  return a const iterator object starting from its given param
 *    end:     return the end iterator
 *    cend:    return the const end iterator
 *    setRoot: only available in object instance mode, set root of list
 */
template <class Node, int N = 0>
class UIntrLst {
  using tLstNd = UIntrLstNd<Node, N>;
  using tLst   = UIntrLst;

  tLstNd* mRoot;
public:
  using iterator       = UIntrLstItr<Node, N>;
  using const_iterator = UIntrLstCItr<Node, N>;

  /* static way of using Intrusive List */
  static size_t size(const tLstNd& start){
    if (start.mNxt == nullptr) return 0;
    size_t count = 1;
    for (const tLstNd* begin = &start, *next = start.mNxt;
         next != begin;
         next = next->mNxt, ++count);
    return count;
  }
  static size_t size(const tLstNd* start){
    if (!start) return 0;
    return size(*start);
  }
  static void create(tLstNd& root){
    assert(root.mNxt == nullptr);
    root.mNxt = &root;
  }
  static void create(tLstNd* root){ if (root) create(*root); }
  static void release(tLstNd& root){
    if (root.mNxt == nullptr) return;
    tLstNd* cur = root.mNxt;
    tLstNd* prv = &root;
    do {
      prv->mNxt = nullptr;
      prv = cur;
      cur = cur->mNxt;
    } while (cur);
  }
  static void release(tLstNd* root){ if (root) release(*root); }
  template <class Func>
  static void destroy(tLstNd& root, Func dtorFn){
    if (root.mNxt == nullptr){
      dtorFn(static_cast<Node*>(&root));
      return;
    }
    tLstNd* cur = root.mNxt;
    tLstNd* prv = &root;
    for (; cur != &root; prv = cur, cur = cur->mNxt){
      prv->mNxt = nullptr;
      dtorFn(static_cast<Node*>(prv));
    }
    prv->mNxt = nullptr;
    dtorFn(static_cast<Node*>(prv));
  }
  template <class Func>
  static void destroy(tLstNd* root, Func dtorFn){
    if (root) destroy<Func>(*root, dtorFn);
  }
  static void insert(tLstNd& na, tLstNd& nb){
    assert(na.mNxt && (nb.mNxt == nullptr || nb.mNxt == &nb));
    nb.mNxt = na.mNxt;
    na.mNxt = &nb;
  }
  static void insert(tLstNd* na, tLstNd* nb){
    if (na && nb) insert(*na, *nb);
  }
  static void remove(tLstNd& node){
    assert(node.mNxt);
    if (node.mNxt == &node){
      release(node);
      return;
    }
    tLstNd* nxt = node.mNxt;
    node.mNxt = nxt->mNxt;
    nxt->mNxt = nullptr;
  }
  static void remove(tLstNd* node){
    if (node) remove(*node);
  }
  static void merge(tLstNd& na, tLstNd& nb){
    assert(na.mNxt && nb.mNxt);
    tLstNd* lnk = &na, * end = &nb;
    for (; lnk->mNxt != &na; lnk = lnk->mNxt);
    for (; end->mNxt != &nb; end = end->mNxt);
    lnk->mNxt = &nb;
    end->mNxt = &na;
  }
  static void merge(tLstNd* na, tLstNd* nb){
    if (na && nb) merge(*na, *nb);
  }
  static iterator begin(tLstNd& node){ return iterator(node); }
  static iterator begin(tLstNd* node){ return iterator(node); }
  static const_iterator cbegin(const tLstNd& node){return const_iterator(node);}
  static const_iterator cbegin(const tLstNd* node){return const_iterator(node);}
  static iterator end(){ return iterator::end(); }
  static const_iterator cend(){ return const_iterator::end(); }

  /* Object instance way of using Intrusive List */
  UIntrLst() : mRoot(nullptr) {}
  explicit UIntrLst(tLstNd& node) : mRoot(&node) {
    assert(mRoot);
    if (!mRoot->mNxt) create(*mRoot);
  }
  explicit UIntrLst(tLstNd* node) : mRoot(node) {
    if (mRoot && !mRoot->mNxt) create(*mRoot);
  }
  UIntrLst(const tLst& o) : mRoot(o.mRoot) {}
  tLst& operator=(const tLst& o){ mRoot = o.mRoot; return *this; }
  ~UIntrLst(){}

  void setRoot(tLstNd& r){ mRoot = &r; if (!mRoot->mNxt) create(*mRoot); }
  void setRoot(tLstNd* r){ if (r) setRoot(*r); }
  size_t size(){ assert(mRoot); return size(*mRoot); }
  void release(){ assert(mRoot); release(*mRoot); }
  template <class Func> void destroy(Func dtorFn){
    assert(mRoot); destroy(*mRoot, dtorFn); mRoot = nullptr;
  }
  void destroy(){ destroy([](Node* n){ delete n; }); }
  void insert(tLstNd& nb){ assert(mRoot); insert(*mRoot, nb); }
  void insert(tLstNd* nb){ assert(mRoot); insert(mRoot, nb); }
  void remove(){ assert(mRoot); remove(*mRoot); }
  void merge(tLstNd& nb){ assert(mRoot); merge(*mRoot, nb); }
  void merge(tLstNd* nb){ assert(mRoot); merge(mRoot, nb); }
  iterator begin(){ assert(mRoot); return begin(*mRoot); }
  const_iterator cbegin(){ assert(mRoot); return cbegin(*mRoot); }
};

#endif //__UNIDIRECTIONAL_INTRUSIVE_LIST__
