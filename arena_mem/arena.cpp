#include <new>
#include "arena.h"

#define MAX(a, b) (a) > (b) ? (a) : (b)

struct Arena::MmryBlk : UIntrLstNd<Arena::MmryBlk> {
  size_t mBlkSz;

  MmryBlk(size_t sz) : mBlkSz(sz) {}
  ~MmryBlk(){}

  //make sure we cannot copy memory blocks
  MmryBlk(const MmryBlk&) = delete;
  MmryBlk& operator=(const MmryBlk&) = delete;
};

size_t Arena::alignedSize(size_t sz){
  //raise size to minimal 4 byte aligned value
  return sz + 3UL & ~3UL;
}

size_t Arena::alignedAddr(size_t addr, size_t sz){
  //assume addr is already 4 byte aligned, and sz is 4 or 8 byte aligned
  if (sz & 4) return addr;
  else        return addr + 7UL & ~7UL;
}

void Arena::allocBlk(size_t blksz){
  size_t alloc_sz = MAX(blksz, MIN_BLK_SZ);
  void* nb = ::new char[alloc_sz];
  MmryBlk* pblk = ::new (nb) MmryBlk(alloc_sz);
  if (mCurBlk && mCurBlk != mSBlk) BlockList::insert(mCurBlk, pblk);
  else                             BlockList::create(pblk);
  mCurBlk = pblk;
  mBlkNxt = sizeof(MmryBlk);
}

bool Arena::isContainable(size_t sz){
  assert(mBlkNxt);
  size_t asz = alignedSize(sz);
  size_t nxt = alignedAddr((size_t)mCurBlk + mBlkNxt, asz) - (size_t)mCurBlk;

  assert(mCurBlk->mBlkSz >= nxt);

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
  return allocObj(rsz);
}

void Arena::freeBlks(){
  if (mCurBlk && mCurBlk != mSBlk)
    BlockList::destroy(
      mCurBlk, 
      [](MmryBlk* blk){ blk->~MmryBlk(); delete[] (char*) blk; }
    );
}

void Arena::makeDtorCalls(){
  if (mCurDtorRcd){
    DtorList list(mCurDtorRcd);
    for (auto& rec : list) rec.dtorCall();
  }
}

Arena::Arena() :
  mCurBlk(nullptr), mSBlk(nullptr), mBlkNxt(0),
  mFstDtorRcd(nullptr), mCurDtorRcd(nullptr) {}
Arena::Arena(void* bp, size_t bs) : Arena() {
  mCurBlk = mSBlk = ::new (bp) MmryBlk(bs);
  mBlkNxt = sizeof(MmryBlk);
}
Arena::~Arena(){ makeDtorCalls(); freeBlks(); }

void* operator new(size_t sz, Arena& arena){
  return arena.alloc(sz);
}
void* operator new(size_t sz, Arena* arena){
  assert(arena != nullptr);
  return arena->alloc(sz);
}
void* operator new[](size_t sz, Arena& arena){
  return arena.alloc(sz);
}
void* operator new[](size_t sz, Arena* arena){
  //TODO: bug array new needs to have additional size in the front to save size,
  //this size is compiler defined, and have to be reverse engineered.
  assert(arena != nullptr);
  return arena->alloc(sz);
}
