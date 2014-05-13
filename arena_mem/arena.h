#include "udilist.h"

//TODO:
//5) arena used for default placement new syntax
//9) beware of alignment, it's serious and real
//10) add performance tuning reports
////TODO: think about how you would able to nauturally use arena allocator for allocation members of a class inside a class 
//operator new call, without having to force change the syntax of default new and delete

/* Arena Memory Allocator */
class Arena;              //heap arena allocator
template <size_t> SArena; //stack arena allocator

/* placement new implementations */
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

/* Arena with first block of size SZ allocated on stack;
 *   NOTE: SZ is regulated, it will be promoted to byte size 32 if given
 *         smaller, and rounded up to a number divisible by 4
 */
template <size_t SZ>
class SArena : public Arena {
  enum : size_t { REAL_SZ = SZ < 32UL ? 32UL : SZ + ((SZ & 2) << 1 | (SZ & 1) << 2) };
  char mStBlk[REAL_SZ];

  SArena(const SArena&) = delete;
  SArena& operator=(const SArena&) = delete;
public:
  SArena() : Arena(mStBlk, REAL_SZ) {}
  ~SArena(){}
};
