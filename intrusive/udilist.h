/* Circular Unidirectional Intrusive List */
#include <cassert>
#include <cstddef>

#ifndef __UNIDIRECTIONAL_INTRUSIVE_LIST__
#define __UNIDIRECTIONAL_INTRUSIVE_LIST__

template <class Node, int N> class UDIntrLstNd;   //node definition
template <class Node, int N> class UDIntrLst;     //method collection class
template <class Node, int N> class UDIntrLstCItr; //const iterator
template <class Node, int N> class UDIntrLstItr;  //iterator

/** UDIntrLstNd
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
class UDIntrLstNd {
  typedef UDIntrLstNd<Node, N> tLstNd;
  typedef UDIntrLstCItr<Node, N> tLstIter;
  friend class UDIntrLst<Node, N>;
  friend class UDIntrLstCItr<Node, N>;

  tLstNd* mNxt;
public:
  UDIntrLstNd() : mNxt(nullptr) {}
  ~UDIntrLstNd(){ assert(mNxt == nullptr); }
};

/** Intrusive List Const Iterator
 */
template <class Node, int N = 0>
class UDIntrLstCItr {
  typedef UDIntrLstNd<Node, N> tLstNd;
  typedef UDIntrLstCItr<Node, N> tLstItr;

  const tLstNd* mEnd;
  const tLstNd* mCur;

  void init(const tLstNd* start, size_t steps){
    if (!start || start->mNxt == nullptr){
      mCur = mEnd = nullptr;
      return;
    }
    mEnd = mCur = start;
    for (size_t i = 0; i < steps; ++i){
      if (mCur->mNxt == mEnd){
        mCur = mEnd = nullptr;
        break;
      }
      mCur = mCur->mNxt;
    }
  }
public:
  static tLstItr end(){ return tLstItr(); }

  UDIntrLstCItr() : mEnd(nullptr), mCur(nullptr) {}
  UDIntrLstCItr(const tLstNd& start, size_t steps = 0){ init(&start, steps); }
  UDIntrLstCItr(const tLstNd* start, size_t steps = 0){ init(start, steps); }
  UDIntrLstCItr(const tLstItr& other) : mEnd(other.mEnd), mCur(other.mNxt) {}
  tLstItr& operator=(const tLstItr& other){
    mEnd = other.mEnd;
    mCur = other.mCur;
    return *this;
  }
  bool operator==(const tLstItr& other) const {
    return mEnd == other.mEnd && mCur == other.mCur;
  }
  bool operator!=(const tLstItr& other) const {
    return !(this->operator==(other));
  }
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
class UDIntrLstItr {
  typedef UDIntrLstNd<Node, N> tLstNd;
  typedef UDIntrLstItr<Node, N> tLstItr;

  UDIntrLstCItr<Node, N> mItr;
public:
  static tLstItr end(){ return tLstItr(); }

  UDIntrLstItr() : mItr() {}
  UDIntrLstItr(const tLstNd& start, size_t steps = 0) : mItr(start, steps) {}
  UDIntrLstItr(const tLstNd* start, size_t steps = 0) : mItr(start, steps) {}
  UDIntrLstItr(const tLstItr& other) : mItr(other.mItr) {}
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
 *    length:  obtain the number of nodes connected
 *    create:  create a list of 1 element from a node
 *    release: release all nodes from a list, node not deallocated
 *    remove:  release and delete all nodes from a list
 *    insert:  put node 'insertNd' after node 'lstNd, forming a list
 *    begin:   return an iterator object starting from its given param
 *    cbegin:  return a const iterator object starting from its given param
 *    end:     return the end iterator
 *    cend:    return the const end iterator
 */
template <class Node, int N = 0>
class UDIntrLst {
  typedef UDIntrLstNd<Node, N> tLstNd;
  typedef UDIntrLst<Node, N> tLst;

  const tLstNd* mRoot;
public:
  typedef UDIntrLstItr<Node, N> iterator;
  typedef UDIntrLstCItr<Node, N> const_iterator;

  /* static way of using Intrusive List Methods */
  static size_t length(const tLstNd& start){
    if (start.mNxt == nullptr) return 0;
    size_t count = 1;
    for (tLstNd* begin = &start, *next = start.mNxt;
         next != begin;
         next = next->mNxt, ++count);
    return count;
  }
  static size_t length(const tLstNd* start){
    if (!start) return 0;
    return length(*start);
  }
  static void create(tLstNd& root){
    assert(root.mNxt == nullptr);
    root.mNxt = &root;
  }
  static void create(tLstNd* root){ if (!root) return; create(*root); }
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
  static void release(tLstNd* root){ if (!root) return; release(*root); }
  static void remove(tLstNd& root){
    if (root.mNxt == nullptr){
      delete &root;
      return;
    }
    tLstNd* cur = root.mNxt;
    tLstNd* prv = &root;
    for (; cur != &root; prv = cur, cur = cur->mNxt){
      prv->mNxt = nullptr;
      delete prv;
    }
    cur->mNxt = nullptr;
    delete cur;
  }
  static void remove(tLstNd* root){ if (!root) return; remove(*root); }
  static void insert(tLstNd& lstNd, tLstNd& insertNd){
    assert(insertNd.mNxt == nullptr);
    insertNd.mNxt = lstNd.mNxt;
    lstNd.mNxt = &insertNd;
  }
  static void insert(tLstNd* lstNd, tLstNd* insertNd){
    if (!lstNd || !insertNd) return;
    insert(*lstNd, *insertNd);
  }
  static iterator begin(tLstNd& node){ return iterator(node); }
  static iterator begin(tLstNd* node){ return iterator(*node); }
  static const_iterator cbegin(const tLstNd& node){return const_iterator(node);}
  static const_iterator cbegin(const tLstNd* node){return const_iterator(node);}
  static iterator end(){ return iterator::end(); }
  static const_iterator cend(){ return const_iterator::end(); }

  /* Object instance way of using Intrusive List */
  UDIntrLst() : mRoot(nullptr) {}
  UDIntrLst(tLstNd& node) : mRoot(&node) {}
  UDIntrLst(tLstNd* node) : mRoot(node) {}
  UDIntrLst(const tLst& other) : mRoot(other.mRoot) {}
  tLst& operator=(const tLst& other){ mRoot = other.mRoot; return *this; }
  ~UDIntrLst(){}

  size_t length(){ assert(mRoot); return length(*mRoot); }
  void create(){ assert(mRoot); create(*mRoot); }
  void release(){ assert(mRoot); release(*mRoot); }
  void remove(){ assert(mRoot); remove(*mRoot); mRoot = nullptr; }
  void insert(tLstNd& insertNd){ assert(mRoot); insert(*mRoot, insertNd); }
  void insert(tLstNd* insertNd){ assert(mRoot); insert(*mRoot, *insertNd); }
  iterator begin(){ assert(mRoot); return begin(*mRoot); }
  const_iterator cbegin(){ assert(mRoot); return cbegin(*mRoot); }
};

#endif //__UNIDIRECTIONAL_INTRUSIVE_LIST__
