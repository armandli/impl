#include "arena.h"

#define MAX(a, b) (a) > (b) ? (a) : (b)

extern "C++" void* operator new(size_t, void*) throw();

struct Arena::MmryBlk : UDIntrLstNd<Arena::MmryBlk> {
  size_t mBlkSz;

  MmryBlk(size_t sz) : mBlkSz(sz) {}
  ~MmryBlk(){}
};

size_t Arena::alignedSize(size_t sz){
  //raise size to minimal 4 byte aligned value
  return (sz & ((size_t)-1 ^ 3)) + (sz & 2 << 1 | sz & 1 << 2);
}

size_t Arena::alignedAddr(size_t addr, size_t sz){
  //assume addr is already 4 byte aligned, and sz is 4 or 8 byte aligned
  if (sz & 4) return  addr;
  else        return (addr & ((size_t)-1 ^ 7)) + (addr & 4 << 1);
}

void Arena::allocBlk(size_t blksz){
  size_t alloc_sz = MAX(blksz, MIN_BLK_SZ);
  void* nb = new char[alloc_sz];
  MmryBlk* pblk = new (nb) MmryBlk(alloc_sz);
  if (mCurBlk)
    BlockList::insert(mCurBlk, pblk);
  else {
    mCurBlk = pblk;
    BlockList::create(pblk);
  }
  mBlkNxt = sizeof(MmryBlk);
}

bool Arena::isContainable(size_t sz){
  assert(mBlkNxt != 0);
  size_t asz = alignedSize(sz);
  size_t nxt = alignedAddr((size_t)mCurBlk + mBlkNxt, asz) - (size_t)mCurBlk;
  if (mCurBlk->mBlkSz - nxt >= asz) return true;
  else                              return false;
}

void* Arena::allocObj(size_t sz){
  size_t asz = alignedSize(sz);
  size_t nad = alignedAddr((size_t)mCurBlk + mBlkNxt, asz);
  mBlkNxt = nad - (size_t)mCurBlk + asz;
  return (void*)nad;
}

void* Arena::alloc(size_t sz){
  size_t rsz = MAX(sz, 4); //minimal allocating size assumption
  if (!mCurBlk || !isContainable(rsz))
    allocBlk(MAX(BLK_SZ, alignedSize(rsz) + sizeof(MmryBlk)));
  return allocObj(sz);
}

void Arena::freeBlks(){ BlockList::remove(mCurBlk); }

Arena::Arena() : mCurBlk(nullptr), mBlkNxt(0) {}
Arena::~Arena(){ freeBlks(); }
Arena::Arena(size_t sz) : mCurBlk(nullptr) { allocBlk(sz); }

void* operator new(size_t sz, Arena& arena){
  return arena.alloc(sz);
}
void* operator new(size_t sz, Arena* arena){
  assert(arena != nullptr);
  return arena->alloc(sz);
}
