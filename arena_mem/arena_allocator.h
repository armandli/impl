#include "arena.h"

//STL allocator interface for arena allocator

template <typename T>
class ArenaAllocator {
  Arena* mArena; /* ArenaAllocator does not own the arena */
public:
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T value_type;

  typedef std::true_type propagate_on_container_move_assignment;

  template <typename U> struct rebind {
    typedef ArenaAllocator<U> other;
  };

  ArenaAllocator() noexcept : mArena(nullptr) {}
  ArenaAllocator(Arena& a) noexcept : mArena(&a) {}
  ArenaAllocator(Arena* a) noexcept : mArena(a) {}

  ArenaAllocator(const ArenaAllocator& other) : mArena(other.mArena) {}
  template <typename U> ArenaAllocator(const ArenaAllocator<U>& other)
    : mArena(other.mArena) {}

  ~ArenaAllocator() noexcept { /*do nothing*/ }

  pointer address(reference r) const noexcept {
    return std::__addressof(r);
  }

  const_pointer address(const_reference r) const noexcept {
    return std::__addressof(r);
  }

  size_type max_size() const noexcept {
    return size_t(-1L) / sizeof(T);
  }

  pointer allocate(size_type n, const void* = 0){
    assert(mArena);
    mArena->alloc(n);
  }

  void deallocate(pointer, size_type){ /* do nothing */ }
  
  template <typename U, typename... Args>
  void construct(U* p, Args&&... args){
    assert(mArena);
    ::new((void*)p) U(std::forward<Args>(args)...);
//    mArena->regDtor(p, 1); //BUG: arena destructor deallocation have serious problems, need more investigation
  }

  template <typename U>
  void destroy(U* p){ /*do nothing*/ }
};

template <typename T>
inline bool operator==(const ArenaAllocator<T>&, const ArenaAllocator<T>&){
  return true;
}

template <typename T>
inline bool operator!=(const ArenaAllocator<T>&, const ArenaAllocator<T>&){
  return false;
}

