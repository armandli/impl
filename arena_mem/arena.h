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
//9) now thinking about it, it's better to have 3 kinds of arena allocators, one automatically register destructors, another
//   just plain not going to call destructor and assume safe, and then the other one is versatile, but require user manual
//   registration
//10) beware of alignment, it's serious and real
//11) add performance tuning reports

//barebone arena implementation
class Arena {
  enum : size_t {
    BLK_SZ     = 1U << 12, //default block size, same as page size
    MIN_BLK_SZ = 0x20U,    //minimum size for block is 32 bytes, including MmryBlk
  };

  inline static size_t alignedSize(size_t);
  inline static size_t alignedAddr(size_t addr, size_t sz);

  struct MmryBlk;  //memory block struct

  typedef UDIntrLst<MmryBlk> BlockList;

  MmryBlk* mCurBlk; //the current memory block used for memory allocation
  size_t mBlkNxt;   //offset in the current memory block for next allocation

  void allocBlk(size_t);
  void freeBlks();
  bool isContainable(size_t);
  void* allocObj(size_t);
public:
  Arena();       //arena without allocating any memory block
  Arena(size_t); //arena with first block allocated with size on heap
  ~Arena();

  void* alloc(size_t);
};


// placement new implementations
void* operator new(size_t, Arena&);
void* operator new(size_t, Arena*);
inline void  operator delete(void*, Arena&){} //do nothing
inline void  operator delete(void*, Arena*){} //do nothing
//void* operator new[](size_t size, Arena& arena);
//void operator delete[](void* p, Arena& arena);
