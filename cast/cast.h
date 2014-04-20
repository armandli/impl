/** Cast function template collection
 *    due to non-optimal implementation in C++ for object model and interaction
 *    with casting for optimized C++ programming, we introduce an alternative
 *    to do casting without using runtime type information and avoid
 *    dynamic cast; Note the following casting methods only word for down-cast;
 *
 *  Use:
 *    in order to use the following casting methods, you need to inject the
 *    following implementations into the class heiarchy:
 *    1) embed enum ClassId int the class heirarchy from base class
 *    2) public getType() method returning the enum classId for each class
 *       and subclass
 *    3) classOf(C*c) public static method that returns true if c is sub-class
 *       of C or is C, false otherwise
 */

#include <cassert>

template <class To, class From>
bool isa(const From* from){
  return To::classOf(from);
}

template <class To, class From>
bool isa(From* from){
  return To::classOf(from);
}

template <class To, class From>
bool isa(const From& from){
  return To::classOf(&from);
}

template <class To, class From>
bool isa(From& from){
  return To::classOf(&from);
}

template <class To, class From>
const To* cast(const From* from){
  assert(To::classOf(from));
  return static_cast<const To*>(from);
}

template <class To, class From>
To* cast(From* from){
  assert(To::classOf(from));
  return static_cast<To*>(from);
}

template <class To, class From>
const To& cast(const From& from){
  assert(To::classOf(&from));
  return static_cast<const To&>(from);
}

template <class To, class From>
To& cast(From& from){
  assert(To::classOf(&from));
  return static_cast<To&>(from);
}

template <class To, class From>
const To* cast_or_null(const From* from){
  if (To::classOf(from)) return static_cast<const To*>(from);
  else                   return nullptr;
}

template <class To, class From>
To* cast_or_null(From* from){
  if (To::classOf(from)) return static_cast<To*>(from);
  else                   return nullptr;
}
