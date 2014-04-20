/* Circular Bidirectional Intrusive List */
#include <cassert>
#include <cstddef>

//TODO: add the following:
//  1) redesign iterators, and add reverse iterators
//  2) add non-static member functions for BDIntrLst

#ifndef __BIDIRECTIONAL_INTRUSIVE_LIST__
#define __BIDIRECTIONAL_INTRUSIVE_LIST__

template <class Node, int N> class BDIntrLstNd;
template <class Node, int N> class BDIntrLst;
template <class Node, int N> class BDIntrLstFIter;

/** BDIntrLstNd
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
class BDIntrLstNd {
  typedef BDIntrLstNd<Node, N> tLstNd;
  typedef BDIntrLstFIter<Node, N> tLstFIter;
  friend class BDIntrLst<Node, N>;
  friend class BDIntrLstFIter<Node, N>;

  tLstNd* mNxt;
  tLstNd* mPrv;
public:
  BDIntrLstNd() : mNxt(nullptr), mPrv(nullptr) {}
  ~BDIntrLstNd(){ assert(mNxt == nullptr); }
};

/** Intrusive List Forward Iterator
 */
template <class Node, int N = 0>
class BDIntrLstFIter {
  typedef BDIntrLstNd<Node, N> tLstNd;
  typedef BDIntrLstFIter<Node, N> tLstIter;

  tLstNd* mEnd;
  tLstNd* mNxt;
public:
  static tLstIter end(){ return tLstIter(); }

  BDIntrLstFIter() : mEnd(nullptr), mNxt(nullptr) {}
  BDIntrLstFIter(tLstNd& start, size_t steps = 0){
    if (start.mNxt == nullptr){
      mEnd = mNxt = nullptr;
      return;
    }
    mEnd = mNxt = &start;
    for (size_t i = 0; i < steps; ++i){
      if (mNxt->mNxt == mEnd){
        mNxt = mEnd = nullptr;
        break;
      }
      mNxt = mNxt->mNxt;
    }
  }
  BDIntrLstFIter(const tLstIter& other) : mEnd(other.mEnd), mNxt(other.mNxt) {}
  tLstIter& operator=(const tLstIter& other){
    mEnd = other.mEnd;
    mNxt = other.mNxt;
    return *this;
  }
  inline bool operator==(const tLstIter& other) const {
    return mEnd == other.mEnd && mNxt == other.mNxt;
  }
  inline bool operator!=(const tLstIter& other) const {
    return !(this->operator==(other));
  }
  tLstIter& operator++(){
    assert(mNxt != nullptr);
    if (mNxt->mNxt == mEnd)
      mNxt = mEnd = nullptr;
    else
      mNxt = mNxt->mNxt;
    return *this;
  }
  tLstIter operator++(int){
    tLstIter tmp(*this);
    if (mNxt->mNxt == mEnd)
      mNxt = mEnd = nullptr;
    else
      mNxt = mNxt->mNxt;
    return tmp;
  }
  Node& operator*() const {
    assert(mNxt != nullptr);
    return static_cast<Node&>(*mNxt);
  }
};

/** Intrusive List
 *    contains all operations on intrusive nodes, but does not have any meta
 *    information on the list - providing a separation between data and
 *    operation, allowing subclass of intrusive list node not having to
 *    override functionalities by accident.
 *  Note: because the list class does not have any meta information, we
 *  currently cannot use for each loop on intrusive list nodes
 *
 *  Interface operations:
 *    length:        obtain the number of nodes connected
 *    create_list:   create a list of 1 element from a node
 *    release:       release a node from a list, node not deallocated
 *    release_all:   release all nodes from a list, node not deallocated
 *    remove:        release and delete a node from a list
 *    remove_all:    release and delete all nodes from a list
 *    insert_before: put node 'insertNd' before node 'lstNd', forming a list
 *    insert_after:  put node 'insertNd' after node 'lstNd, forming a list
 */
template <class Node, int N = 0>
class BDIntrLst {
  typedef BDIntrLstNd<Node, N> tLstNd;
public:
  static size_t length(tLstNd& start){
    if (start.mNxt == nullptr) return 0;
    size_t count = 1;
    for (tLstNd* begin = &start, *next = start.mNxt;
         next != begin;
         next = next->mNxt, ++count);
    return count;
  }
  static void create_list(tLstNd& root){
    if (root.mNxt != nullptr){
      root.mPrv->mNxt = root.mNxt;
      root.mNxt->mPrv = root.mPrv;
    }
    root.mNxt = root.mPrv = &root;
  }
  static void release(tLstNd& node){
    if (node.mNxt == nullptr) return;
    node.mPrv->mNxt = node.mNxt;
    node.mNxt->mPrv = node.mPrv;
    node.mPrv = node.mNxt = nullptr;
  }
  static void release_all(tLstNd& node){
    if (node.mNxt == nullptr) return;
    tLstNd* cur = node.mNxt;
    tLstNd* prv = &node;
    do {
      prv->mPrv = prv->mNxt = nullptr;
      prv = cur;
      cur = cur->mNxt;
    } while (cur);
  }
  static void remove(tLstNd& node){
    if (node.mNxt == nullptr){
      delete &node;
      return;
    }
    node.mPrv->mNxt = node.mNxt;
    node.mNxt->mPrv = node.mPrv;
    node.mPrv = node.mNxt = nullptr;
    delete &node;
  }
  static void remove_all(tLstNd& node){
    if (node.mNxt == nullptr){
      delete &node;
      return;
    }
    //TODO: this is bad implementation; you're assuming the ptr after deletion is not reallocated immediately
    node.mPrv->mNxt = nullptr;
    tLstNd* ptr = node.mNxt;
    tLstNd* prv = ptr;
    do {
      ptr->mPrv->mPrv = ptr->mPrv->mNxt = nullptr;
      delete ptr->mPrv;
      prv = ptr;
      ptr = ptr->mNxt;
    } while (ptr);
    delete prv;
  }
  static void insert_before(tLstNd& lstNd, tLstNd& insertNd){
    release(insertNd);
    //TODO: may not need the if
    if (lstNd.mNxt == nullptr){
      insertNd.mNxt = insertNd.mPrv = &lstNd;
      lstNd.mPrv = lstNd.mNxt = &insertNd;
    } else {
      lstNd.mPrv->mNxt = &insertNd;
      insertNd.mPrv = lstNd.mPrv;
      lstNd.mPrv = &insertNd;
      insertNd.mNxt = &lstNd;
    }
  }
  static void insert_after(tLstNd& lstNd, tLstNd& insertNd){
    release(insertNd);
    //TODO: may not need the if
    if (lstNd.mNxt == nullptr){
      insertNd.mNxt = insertNd.mPrv = &lstNd;
      lstNd.mNxt = lstNd.mPrv = &insertNd;
    } else {
      insertNd.mPrv = &lstNd;
      insertNd.mNxt = lstNd.mNxt;
      lstNd.mNxt->mPrv = &insertNd;
      lstNd.mNxt = &insertNd;
    }
  }
};

#endif //__BIDIRECTIONAL_INTRUSIVE_LIST__
