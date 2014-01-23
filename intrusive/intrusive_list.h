/* circular bidirectional intrusive list */
#include <cassert>
#include <cstddef>

#ifndef __INTRUSIVE_LIST__ 
#define __INTRUSIVE_LIST__

//TODO: define other iterators

template <class Node, int N> class IntrLstNode;
template <class Node, int N> class IntrList;
template <class Node, int N> class IntrLstFIter;

/** IntrLstNode
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
class IntrLstNode {
  typedef IntrLstNode<Node, N> tLstNode;
  typedef IntrLstFIter<Node, N> tLstFIter;
  friend class IntrList<Node, N>;
  friend class IntrLstFIter<Node, N>;

  tLstNode* mNxt;
  tLstNode* mPrv;
public:
  IntrLstNode() : mNxt(nullptr), mPrv(nullptr) {}
  ~IntrLstNode(){ assert(mNxt == nullptr); }
};

/** Intrusive List Forward Iterator
 */
template <class Node, int N = 0>
class IntrLstFIter {
  typedef IntrLstNode<Node, N> tLstNode;
  typedef IntrLstFIter<Node, N> tLstIter;

  tLstNode* mEnd;
  tLstNode* mNxt;
public:
  static tLstIter end(){ return tLstIter(); }

  IntrLstFIter() : mEnd(nullptr), mNxt(nullptr) {}
  IntrLstFIter(tLstNode& start, size_t steps = 0){
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
  IntrLstFIter(const tLstIter& other) : mEnd(other.mEnd), mNxt(other.mNxt) {}
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
 *    length: obtain the number of nodes connected
 *    create_list: create a list of 1 element from a node
 *    release: release a node from a list, node not deallocated
 *    release_all: release all nodes from a list, node not deallocated
 *    remove: release and delete a node from a list
 *    remove_all: release and delete all nodes from a list
 *    insert_before: put node 'insertNode' before node 'lstNode', forming a list
 *    insert_after: put node 'insertNode' after node 'lstNode, forming a list
 */
template <class Node, int N = 0>
class IntrList {
  typedef IntrLstNode<Node, N> tLstNode;
public:
  static size_t length(tLstNode& start){
    if (start.mNxt == nullptr) return 0;
    size_t count = 1;
    tLstNode* begin = &start;
    tLstNode* next = start.mNxt;
    for (tLstNode* begin = &start, *next = start.mNxt;
         next != begin;
         next = next->mNxt, ++count);
    return count;
  }
  static void create_list(tLstNode& root){
    if (root.mNxt != nullptr){
      root.mPrv->mNxt = root.mNxt;
      root.mNxt->mPrv = root.mPrv;
    }
    root.mNxt = root.mPrv = &root;
  }
  static void release(tLstNode& node){
    if (node.mNxt == nullptr) return;
    node.mPrv->mNxt = node.mNxt;
    node.mNxt->mPrv = node.mPrv;
    node.mPrv = node.mNxt = nullptr;
  }
  static void release_all(tLstNode& node){
    if (node.mNxt == nullptr) return;
    tLstNode* cur = node.mNxt;
    tLstNode* prv = &node;
    do {
      prv->mPrv = prv->mNxt = nullptr;
      prv = cur;
      cur = cur->mNxt;
    } while (cur);
  }
  static void remove(tLstNode& node){
    if (node.mNxt == nullptr){
      delete &node;
      return;
    }
    node.mPrv->mNxt = node.mNxt;
    node.mNxt->mPrv = node.mPrv;
    node.mPrv = node.mNxt = nullptr;
    delete &node;
  }
  static void remove_all(tLstNode& node){
    if (node.mNxt == nullptr){
      delete &node;
      return;
    }
    node.mPrv->mNxt = nullptr;
    tLstNode* ptr = node.mNxt;
    tLstNode* prv = ptr;
    do {
      ptr->mPrv->mPrv = ptr->mPrv->mNxt = nullptr;
      delete ptr->mPrv;
      prv = ptr;
      ptr = ptr->mNxt;
    } while (ptr);
    delete prv;
  }
  static void insert_before(tLstNode& lstNode, tLstNode& insertNode){
    release(insertNode);
    if (lstNode.mNxt == nullptr){
      insertNode.mNxt = insertNode.mPrv = &lstNode;
      lstNode.mPrv = lstNode.mNxt = &insertNode;
    } else {
      lstNode.mPrv->mNxt = &insertNode;
      insertNode.mPrv = lstNode.mPrv;
      lstNode.mPrv = &insertNode;
      insertNode.mNxt = &lstNode;
    }
  }
  static void insert_after(tLstNode& lstNode, tLstNode& insertNode){
    release(insertNode);
    if (lstNode.mNxt == nullptr){
      insertNode.mNxt = insertNode.mPrv = &lstNode;
      lstNode.mNxt = lstNode.mPrv = &insertNode;
    } else {
      insertNode.mPrv = &lstNode;
      insertNode.mNxt = lstNode.mNxt;
      lstNode.mNxt->mPrv = &insertNode;
      lstNode.mNxt = &insertNode;
    }
  }
};

#endif //__INTRUSIVE_LIST__
