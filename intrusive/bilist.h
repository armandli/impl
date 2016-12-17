/* Circular Bidirectional Intrusive List */
#include <cassert>
#include <cstddef>

#ifndef __BIDIRECTIONAL_INTRUSIVE_LIST__
#define __BIDIRECTIONAL_INTRUSIVE_LIST__

template <class Node, int N> class BIntrLstNd;    //node definition
template <class Node, int N> class BIntrLst;      //method collection class
template <class Node, int N> class BIntrLstBaseItr; //iterator base class
template <class Node, int N> class BIntrLstCFItr; //const forward iterator
template <class Node, int N> class BIntrLstFItr;  //forward iterator
template <class Node, int N> class BIntrLstCRItr; //const reverse iterator
template <class Node, int N> class BIntrLstRItr;  //reverse iterator
template <class Node, int N> class BIntrLstCBItr; //const bidirectional iter
template <class Node, int N> class BIntrLstBItr;  //bidirectional iterator


/** BIntrLstNd
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
class BIntrLstNd {
  friend class BIntrLst<Node, N>;
  friend class BIntrLstBaseItr<Node, N>;
  friend class BIntrLstCFItr<Node, N>;
  friend class BIntrLstCRItr<Node, N>;
  friend class BIntrLstCBItr<Node, N>;

  BIntrLstNd* mNxt;
  BIntrLstNd* mPrv;
public:
  BIntrLstNd() : mNxt(nullptr), mPrv(nullptr) {}
  ~BIntrLstNd(){ assert(mNxt == nullptr && mPrv == nullptr); }
};

/* Iterator Base Class
 */
template <class Node, int N = 0>
class BIntrLstBaseItr {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLstItr = BIntrLstBaseItr;

  void init(const tLstNd* start){
    if (!start || start->mNxt == nullptr) mCur = mEnd = nullptr;
    else                                  mCur = mEnd = start;
  }
protected:
  const tLstNd* mEnd;
  const tLstNd* mCur;

  BIntrLstBaseItr() : mEnd(nullptr), mCur(nullptr) {}
  explicit BIntrLstBaseItr(const tLstNd& start){ init(&start); }
  explicit BIntrLstBaseItr(const tLstNd* start){ init(start); }
  BIntrLstBaseItr(const tLstItr& o) : mEnd(o.mEnd), mCur(o.mCur) {}
  tLstItr& operator=(const tLstItr& o){
    mEnd = o.mEnd;
    mCur = o.mCur;
    return *this;
  }
public:
  bool operator==(const tLstItr& o) const {
    return mEnd == o.mEnd && mCur == o.mCur;
  }
  bool operator!=(const tLstItr& o) const {
    return not this->operator==(o);
  }
  const Node& operator*() const {
    assert(mCur != nullptr);
    return static_cast<const Node&>(*mCur);
  }
};

/** Const Forward Iterator
 */
template <class Node, int N = 0>
class BIntrLstCFItr : public BIntrLstBaseItr<Node,N> {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLstItr = BIntrLstCFItr;
public:
  static tLstItr end(){ return tLstItr(); }

  BIntrLstCFItr() : BIntrLstBaseItr<Node,N>() {}
  explicit BIntrLstCFItr(const tLstNd& s) : BIntrLstBaseItr<Node,N>(s) {}
  explicit BIntrLstCFItr(const tLstNd* s) : BIntrLstBaseItr<Node,N>(s) {}
  BIntrLstCFItr(const BIntrLstCFItr& o) : BIntrLstBaseItr<Node,N>(o) {}
  tLstItr& operator=(const tLstItr& o){
    static_cast<BIntrLstBaseItr<Node,N>*>(this)->operator=(o);
  }

  tLstItr& operator++(){
    assert(this->mCur != nullptr);
    if (this->mCur->mNxt == this->mEnd) this->mCur = this->mEnd = nullptr;
    else                                this->mCur = this->mCur->mNxt;
    return *this;
  }
  tLstItr& operator++(int){ tLstItr tmp(*this); this->operator++(); return tmp; }
};

/** Forward Iterator (implemented using Const Forward Iterator)
 */
template <class Node, int N = 0>
class BIntrLstFItr {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLstItr = BIntrLstFItr;
  
  BIntrLstCFItr<Node, N> mItr;
public:
  static tLstItr end(){ return tLstItr(); }

  BIntrLstFItr() : mItr() {}
  explicit BIntrLstFItr(tLstNd& s) : mItr(s) {}
  explicit BIntrLstFItr(tLstNd* s) : mItr(s) {}
  BIntrLstFItr(const tLstItr& o) : mItr(o.mItr) {}
  tLstItr& operator=(const tLstItr& o){ mItr = o.mItr; return *this; }
  bool operator==(const tLstItr& o) const { return mItr.operator==(o.mItr); }
  bool operator!=(const tLstItr& o) const { return mItr.operator!=(o.mItr); }
  tLstItr& operator++(){ ++mItr; return *this; }
  tLstItr& operator++(int){ tLstItr tmp(*this); mItr++; return tmp; }
  Node& operator*() const { return const_cast<Node&>(mItr.operator*()); }
};

/** Const Reverse Iterator
 */
template <class Node, int N = 0>
class BIntrLstCRItr : public BIntrLstBaseItr<Node,N> {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLstItr = BIntrLstCRItr;
public:
  static tLstItr end(){ return tLstItr(); }

  BIntrLstCRItr() : BIntrLstBaseItr<Node,N>() {}
  explicit BIntrLstCRItr(const tLstNd& s) : BIntrLstBaseItr<Node,N>(s) {}
  explicit BIntrLstCRItr(const tLstNd* s) : BIntrLstBaseItr<Node,N>(s) {}
  BIntrLstCRItr(const BIntrLstCRItr& o) : BIntrLstBaseItr<Node,N>(o) {}
  tLstItr& operator=(const tLstItr& o){
    static_cast<BIntrLstBaseItr<Node,N>*>(this)->operator=(o);
  }

  tLstItr& operator++(){
    assert(this->mCur != nullptr);
    if (this->mCur->mPrv == this->mEnd) this->mCur = this->mEnd = nullptr;
    else                                this->mCur = this->mCur->mPrv;
    return *this;
  }
  tLstItr& operator++(int){ tLstItr tmp(*this); this->operator++(); return tmp; }
};

/** Reverse Iterator (implemented using Const Reverse Iterator)
 */
template <class Node, int N = 0>
class BIntrLstRItr {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLstItr = BIntrLstRItr;

  BIntrLstCRItr<Node, N> mItr;
public:
  static tLstItr end(){ return tLstItr(); }

  BIntrLstRItr() : mItr() {}
  explicit BIntrLstRItr(tLstNd& s) : mItr(s) {}
  explicit BIntrLstRItr(tLstNd* s) : mItr(s) {}
  BIntrLstRItr(const tLstItr& o) : mItr(o.mItr) {}
  tLstItr& operator=(const tLstItr& o){ mItr = o.mItr; return *this; }
  bool operator==(const tLstItr& o) const { return mItr.operator==(o.mItr); }
  bool operator!=(const tLstItr& o) const { return mItr.operator!=(o.mItr); }
  tLstItr& operator++(){ ++mItr; return *this; }
  tLstItr& operator++(int){ tLstItr tmp(*this); mItr++; return tmp; }
  Node& operator*() const { return const_cast<Node&>(mItr.operator*()); }
};

/** Const Bidirectional Iterator
 *  Because of the bidirectional nature, unless the starting node is not
 *  currently in any list, the iterator loops forever;
 *  work on the user end is required to determine when the bidirectional
 *  iterator should end
 */
template <class Node, int N = 0>
class BIntrLstCBItr : public BIntrLstBaseItr<Node,N> {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLstItr = BIntrLstCBItr;
public:
  static tLstItr end(){ return tLstItr(); }

  BIntrLstCBItr() : BIntrLstBaseItr<Node,N>() {}
  explicit BIntrLstCBItr(const tLstNd& s) : BIntrLstBaseItr<Node,N>(s) {}
  explicit BIntrLstCBItr(const tLstNd* s) : BIntrLstBaseItr<Node,N>(s) {}
  BIntrLstCBItr(const BIntrLstCBItr& o) : BIntrLstBaseItr<Node,N>(o) {}
  tLstItr& operator=(const tLstItr& o){
    static_cast<BIntrLstBaseItr<Node,N>*>(this)->operator=(o);
  }

  tLstItr& operator++(){
    assert(this->mCur != nullptr);
    this->mCur = this->mCur->mNxt;
    return *this;
  }
  tLstItr& operator++(int){ tLstItr tmp(*this); this->operator++(); return tmp; }

  tLstItr& operator--(){
    assert(this->mCur != nullptr);
    this->mCur = this->mCur->mPrv;
    return *this;
  }
  tLstItr& operator--(int){ tLstItr tmp(*this); this->operator--(); return tmp; }
};

/** Intrusive List Bidirectional Iterator
 *    (implemented using the const version)
 */
template <class Node, int N = 0>
class BIntrLstBItr {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLstItr = BIntrLstBItr;
  
  BIntrLstCBItr<Node, N> mItr;
public:
  static tLstItr end(){ return tLstItr(); }

  BIntrLstBItr() : mItr() {}
  explicit BIntrLstBItr(tLstNd& s) : mItr(s) {}
  explicit BIntrLstBItr(tLstNd* s) : mItr(s) {}
  BIntrLstBItr(const tLstItr& o) : mItr(o.mItr) {}
  tLstItr& operator=(const tLstItr& o){ mItr = o.mItr; return *this; }
  bool operator==(const tLstItr& o) const { return mItr.operator==(o.mItr); }
  bool operator!=(const tLstItr& o) const { return mItr.operator!=(o.mItr); }
  tLstItr& operator++(){ ++mItr; return *this; }
  tLstItr& operator++(int){ tLstItr tmp(*this); mItr++; return tmp; }
  tLstItr& operator--(){ --mItr; return *this; }
  tLstItr& operator--(int){ tLstItr tmp(*this); mItr--; return tmp; }
  Node& operator*() const { return const_cast<Node&>(mItr.operator*()); }
};

/** Intrusive List
 *    Two Usages:
 *      1) use as static wrapper class for all intrusive list functionalities
 *      2) use as virtual container of list if initialized with a list node.
 *         which will be used as root node for the lifetime of this object
 *
 *  Interface operations:
 *    size:         obtain the number of nodes connected
 *    create:       create a list of 1 element from a node
 *    release:      release all nodes from a list, nodes not deallocated
 *    destroy:      release and delete all nodes from list, may supply custom
 *                  destructor function
 *    insert_front: put node nb before node na, assume nb does not belong to
 *                  a list larger than size 1, and na is a list
 *    insert_back:  put node nb after node na, assume nb does not belong to a
 *                  list larger than size 1, and na is a list
 *    remove_front: remove the node nb linked before node n, if nb == na, na is
 *                  released
 *    remove_back:  remove the node nb linked after node n, if nb == na, na is
 *                  released
 *    merge:        merge list nb before list na; assume both na and nb are lists
 *    begin:        return an iterator object starting from its given param
 *    cbegin:       return a const iterator object starting from its given param
 *    end:          return the end iterator
 *    cend:         return the const end iterator
 *    rbegin:       return a reverse iterator object starting from given param
 *    crbegin:      return a const reverse iterator object starting from given
 *                  param
 *    rend:         return the reverse end iterator
 *    crend:        return the const reverse end iterator
 *    bbegin:       return a bidirectional iterator object starting from its
 *                  given param
 *    cbbegin:      return a const bidirectional iterator object starting from
 *                  its given param
 *    bend:         return the bidirectional end iterator
 *    cbend:        return the const bidirectional end iterator
 *    setRoot:      only available in object instance mode, set root of list
 */
template <class Node, int N = 0>
class BIntrLst {
  using tLstNd = BIntrLstNd<Node, N>;
  using tLst = BIntrLst;

  tLstNd* mRoot;
public:
  using iterator = BIntrLstFItr<Node, N>;
  using const_iterator = BIntrLstCFItr<Node, N>;
  using reverse_iterator = BIntrLstRItr<Node, N>;
  using const_reverse_iterator = BIntrLstCRItr<Node, N>;
  using bidirectional_iterator = BIntrLstBItr<Node, N>;
  using const_bidirectional_iterator = BIntrLstCBItr<Node, N>;

  /* Static way of using Intrusive List */
  static size_t size(tLstNd& start){
    if (start.mNxt == nullptr) return 0;
    size_t count = 1;
    for (tLstNd* begin = &start, *next = start.mNxt;
         next != begin;
         next = next->mNxt, ++count);
    return count;
  }
  static size_t size(const tLstNd* start){
    if (!start) return 0;
    return size(*start);
  }
  static void create(tLstNd& root){
    assert(root.mNxt == nullptr && root.mPrv == nullptr);
    root.mNxt = root.mPrv = &root;
  }
  static void create(tLstNd* root){ if (!root) return; create(*root); }
  static void release(tLstNd& node){
    if (node.mNxt == nullptr) return;
    tLstNd* cur = node.mNxt;
    tLstNd* prv = &node;
    do {
      prv->mPrv = prv->mNxt = nullptr;
      prv = cur;
      cur = cur->mNxt;
    } while (cur);
  }
  static void release(tLstNd* root){ if (!root) return; release(*root); }
  template <class Func>
  static void destroy(tLstNd& root, Func dtorFn){
    if (root.mNxt == nullptr){
      dtorFn(static_cast<Node*>(*root));
      return;
    }
    tLstNd* cur = root.mNxt;
    tLstNd* prv = &root;
    for (; cur != &root; prv = cur, cur = cur->mNxt){
      prv->mNxt = prv->mPrv = nullptr;
      dtorFn(static_cast<Node*>(prv));
    }
    prv->mNxt = prv->mPrv = nullptr;
    dtorFn(static_cast<Node*>(prv));
  }
  template <class Func>
  static void destroy(tLstNd* root, Func dtorFn){
    if (root) destroy(*root, dtorFn);
  }
  static void insert_front(tLstNd& na, tLstNd& nb){
    assert(na.mNxt && na.mPrv &&
           (nb.mNxt == nullptr || nb.mNxt == &nb) &&
           (nb.mPrv == nullptr || nb.mPrv == &nb));
    nb.mPrv = na.mPrv;
    nb.mNxt = &na;
    na.mPrv = nb.mPrv->mNxt = &nb;
  }
  static void insert_front(tLstNd* na, tLstNd* nb){
    if (na && nb) insert_front(*na, *nb);
  }
  static void insert_back(tLstNd& na, tLstNd& nb){
    assert(na.mNxt && na.mPrv &&
           (nb.mNxt == nullptr || nb.mNxt == &nb) &&
           (nb.mPrv == nullptr || nb.mPrv == &nb));
    nb.mNxt = na.mNxt;
    nb.mPrv = &na;
    na.mNxt = nb.mNxt->mPrv = &nb;
  }
  static void insert_back(tLstNd* na, tLstNd* nb){
    if (na && nb) insert_back(*na, *nb);
  }
  static void remove_front(tLstNd& n){
    assert(n.mNxt && n.mPrv);
    if (n.mPrv == &n){
      release(n);
      return;
    }
    tLstNd* prv = n.mPrv;
    n.mPrv = prv->mPrv;
    prv->mPrv->mNxt = &n;
    prv->mPrv = prv->mNxt = nullptr;
  }
  static void remove_front(tLstNd* n){
    if (n) remove_front(*n);
  }
  static void remove_back(tLstNd& n){
    assert(n.mNxt && n.mPrv);
    if (n.mNxt == &n){
      release(n);
      return;
    }
    tLstNd* nxt = n.mNxt;
    n.mNxt = nxt->mNxt;
    nxt->mNxt->mPrv = &n;
    nxt->mPrv = nxt->mNxt = nullptr;
  }
  static void remove_back(tLstNd* n){
    if (n) remove_back(*n);
  }
  static void merge(tLstNd& na, tLstNd& nb){
    assert(na.mNxt && na.mPrv && nb.mNxt && nb.mPrv);
    nb.mPrv->mNxt = &na;
    na.mPrv->mNxt = &nb;
    tLstNd* tmp = nb.mPrv;
    nb.mPrv = na.mPrv;
    na.mPrv = tmp;
  }
  static void merge(tLstNd* na, tLstNd* nb){
    if (!na || !nb) return; merge_front(*na, *nb);
  }
  static iterator begin(tLstNd& node){
    return iterator(node);
  }
  static iterator begin(tLstNd* node){
    return iterator(node);
  }
  static const_iterator cbegin(const tLstNd& node){
    return const_iterator(node);
  }
  static const_iterator cbegin(const tLstNd* node){
    return const_iterator(node);
  }
  static reverse_iterator rbegin(tLstNd& node){
    return reverse_iterator(node);
  }
  static reverse_iterator rbegin(tLstNd* node){
    return reverse_iterator(node);
  }
  static const_reverse_iterator crbegin(const tLstNd& node){
    return const_reverse_iterator(node);
  }
  static const_reverse_iterator crbegin(const tLstNd* node){
    return const_reverse_iterator(node);
  }
  static bidirectional_iterator bbegin(tLstNd& node){
    return bidirectional_iterator(node);
  }
  static bidirectional_iterator bbegin(tLstNd* node){
    return bidirectional_iterator(node);
  }
  static const_bidirectional_iterator cbbegin(const tLstNd& node){
    return const_bidirectional_iterator(node);
  }
  static const_bidirectional_iterator cbbegin(const tLstNd* node){
    return const_bidirectional_iterator(node);
  }
  static iterator end(){
    return iterator::end();
  }
  static const_iterator cend(){
    return const_iterator::end();
  }
  static reverse_iterator rend(){
    return reverse_iterator::end();
  }
  static const_reverse_iterator crend(){
    return const_reverse_iterator::end();
  }
  static bidirectional_iterator bend(){
    return bidirectional_iterator::end();
  }
  static const_bidirectional_iterator cbend(){
    return const_bidirectional_iterator::end();
  }

  /* Object instance way of using Intrusive List */
  BIntrLst() : mRoot(nullptr) {}
  explicit BIntrLst(tLstNd& node) : mRoot(&node) {
    if (!mRoot->mNxt) create(*mRoot);
  }
  explicit BIntrLst(tLstNd* node) : mRoot(node) {
    if (mRoot && !mRoot->mNxt) create(*mRoot);
  }
  BIntrLst(const tLst& other) : mRoot(other.mRoot) {}
  tLst& operator=(const tLst& other){ mRoot = other.mRoot; return *this; }
  ~BIntrLst(){}

  void setRoot(tLstNd& r){
    mRoot = &r;
    if (!mRoot->mNxt) create(*mRoot);
  }
  void setRoot(tLstNd* r){
    if (r) setRoot(*r);
  }
  size_t size(){ assert(mRoot); return size(*mRoot); }
  void release(){ assert(mRoot); release(*mRoot); }
  template <class Func> void destroy(Func dtorFn){
    assert(mRoot); destroy(*mRoot, dtorFn); mRoot = nullptr;
  }
  void destroy(){ destroy([](Node* n){ delete n; }); }
  void insert_front(tLstNd& nb){ assert(mRoot); insert_front(*mRoot, nb); }
  void insert_front(tLstNd* nb){ assert(mRoot); insert_front(mRoot, nb); }
  void insert_back(tLstNd& nb){ assert(mRoot); insert_back(*mRoot, nb); }
  void insert_back(tLstNd* nb){ assert(mRoot); insert_back(mRoot, nb); }
  void remove_front(){ assert(mRoot); remove_front(*mRoot); }
  void remove_back(){ assert(mRoot); remove_back(*mRoot); }
  void merge(tLstNd& nb){ assert(mRoot); merge(*mRoot, nb); }
  void merge(tLstNd* nb){ assert(mRoot); merge(mRoot, nb); }
  iterator begin(){ assert(mRoot); return begin(*mRoot); }
  const_iterator cbegin(){ assert(mRoot); return cbegin(*mRoot); }
  reverse_iterator rbegin(){ assert(mRoot); return rbegin(*mRoot); }
  const_reverse_iterator crbegin(){ assert(mRoot); return crbegin(*mRoot); }
  bidirectional_iterator bbegin(){ assert(mRoot); return bbegin(*mRoot); }
  const_bidirectional_iterator cbbegin(){assert(mRoot); return cbbegin(*mRoot);}
};

#endif //__BIDIRECTIONAL_INTRUSIVE_LIST__
