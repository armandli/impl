#include "udilist.h"

/* Arena Memory Allocator
 */

//requirements: (TODO)
//1) arena class type takes a template that determines the size of each page (template could be a power of 2 instead of raw # bytes)
//2) arena do not need heap allocation the first time it is allocated (first page stack allocation)
//3) interface for user to register destructors as an option; if no destructor registered, no destructor is called
//4) unlimited allocation space, allocating more heap pages when run out of space
//5) arena used for default placement new syntax
//6) constructors:
//     SArena<SZ> arena; // stack allocated arena for first page, other pages heap allocated
//     Arena arena(buf, sizeof(buf)); //arena taking a preallocated buffer as option
//     Arena arena(sz); //arena taking a size indicator for first page heap allocated buffer
//     Arena arena; //arena with no preallocated buffer
//7) allocation methods:
//     C* c = new (arena) C(); //placement new syntax support
//     C* c = new (arena) C[]; //placement new for array syntax support; heuristic determine if first buffer in heap or stack
//8) destructor registration:
//     arena.DTOR(c);    //destructor register for allocated c object
//     arena.DTOR(c, n); //destructor register for allocated c array object
//9) beware of alignment, it's serious and real
//10) add performance tuning reports

//TODO: think about how you would able to nauturally use arena allocator for allocation members of a class inside a class 
//operator new call, without having to force change the syntax of default new and delete

class Arena;

// placement new implementations
void* operator new(size_t, Arena&);
void* operator new(size_t, Arena*);
void* operator new[](size_t, Arena&);
void* operator new[](size_t, Arena*);
inline void operator delete(void*, Arena&){}   //do nothing
inline void operator delete(void*, Arena*){}   //do nothing
inline void operator delete[](void*, Arena&){} //do nothing
inline void operator delete[](void*, Arena*){} //do nothing

//barebone arena implementation
class Arena {
  enum : size_t {
    BLK_SZ     = 1U << 12, //default block size, same as page size
    MIN_BLK_SZ = 0x20U,    //minimum size for block is 32 bytes, including MmryBlk
  };

  inline static size_t alignedSize(size_t);
  inline static size_t alignedAddr(size_t addr, size_t sz);

  struct MmryBlk;  //memory block struct
  using BlockList = UDIntrLst<MmryBlk>;

  void allocBlk(size_t);
  void freeBlks();
  bool isContainable(size_t);
  void* allocObj(size_t);

  //destructor call record
  using DtorFnPtrTy = void (*)(void*, size_t);
  template <class T>
  static void DtorFn(void* ptr, size_t cnt){
    T* it = (T*)ptr + cnt - 1;
    for (size_t i = cnt; i > 0; --i, --it) it->~T();
  }

  //destructor call record struct
  struct DtorRcd : UDIntrLstNd<Arena::DtorRcd> {
    void* mPtr;
    size_t mCnt;
    DtorFnPtrTy mDtorFn;
  
    DtorRcd(void* ptr, size_t cnt, DtorFnPtrTy dtor) :
      mPtr(ptr), mCnt(cnt), mDtorFn(dtor) {}
    ~DtorRcd(){}
  
    void dtorCall(){ mDtorFn(mPtr, mCnt); }
  
    DtorRcd(const DtorRcd&) = delete;
    DtorRcd& operator=(const DtorRcd&) = delete;
  };
  using DtorList = UDIntrLst<DtorRcd>;

  void makeDtorCalls();

  Arena(const Arena&) = delete;
  Arena& operator=(const Arena&) = delete;

  MmryBlk* mCurBlk;     //the current memory block used for memory allocation
  MmryBlk* mSBlk;       //custom first memory block that does not need to free
  size_t mBlkNxt;       //offset in the current memory block for next allocation
  DtorRcd* mFstDtorRcd; //the first registered destructor call record
  DtorRcd* mCurDtorRcd; //the last registered destructor call record
protected:
  Arena(void*, size_t); //arena with special allocated first block
public:
  Arena();              //arena without allocating any memory block
  ~Arena();

  void* alloc(size_t);

  template <class T>
  void regDtor(T* ptr, size_t cnt){
    DtorRcd* rec = new (*this) DtorRcd(ptr, cnt, &DtorFn<T>);
    if (mCurDtorRcd)
      DtorList::insert(mFstDtorRcd, rec);
    else {
      DtorList::create(rec);
      mFstDtorRcd = rec;
    }
    mCurDtorRcd = rec;
  }
};

/* Arena with first block of size 1<<P2SZ allocated on stack */
template <unsigned char P2SZ>
class SArena : public Arena {
  char mStBlk[1UL<<P2SZ];

  SArena(const SArena&) = delete;
  SArena& operator=(const SArena&) = delete;
public:
  SArena() : Arena(mStBlk, 1UL<<P2SZ) {}
  ~SArena(){}
};
