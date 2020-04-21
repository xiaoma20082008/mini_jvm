//
// Created by machunxiao on 2020/4/17.
//

#ifndef MINI_JVM__MEMORY_ALLOCATION_H
#define MINI_JVM__MEMORY_ALLOCATION_H
#include "Common.h"
namespace jvm {

#define MEMORY_TYPES_DO(f)                                                                                                                                                  \
  /* Memory type by sub systems. It occupies lower byte.                    */ \
  f(MtJavaHeap,      "Java Heap")   /* Java heap                            */ \
  f(MtClass,         "Class")       /* Java classes                         */ \
  f(MtThread,        "Thread")      /* thread objects                       */ \
  f(MtThreadStack,   "Thread Stack")                                           \
  f(MtCode,          "Code")        /* generated code                       */ \
  f(MtGC,            "GC")                                                     \
  f(MtCompiler,      "Compiler")                                               \
  f(MtJVMCI,         "JVMCI")                                                  \
  f(MtInternal,      "Internal")    /* memory used by VM, but doesn't belong*/ \
                                    /* to any of above categories, and not  */ \
                                    /* used byNMT                           */ \
  f(MtOther,         "Other")       /* memory not used by VM                */ \
  f(MtSymbol,        "Symbol")                                                 \
  f(MtNMT,           "Native Memory Tracking")  /* memory used by NMT       */ \
  f(MtClassShared,   "Shared class space")      /* class data sharing       */ \
  f(MtChunk,         "Arena Chunk") /* chunk that holds content of arenas   */ \
  f(MtTest,          "Test")        /* Test type for verifying NMT          */ \
  f(MtTracing,       "Tracing")                                                \
  f(MtLogging,       "Logging")                                                \
  f(MtStatistics,    "Statistics")                                             \
  f(MtArguments,     "Arguments")                                              \
  f(MtModule,        "Module")                                                 \
  f(MtSafepoint,     "SafePoint")                                              \
  f(MtSynchronizer,  "Synchronization")                                        \
  f(MtNone,          "Unknown")                                                \
  //end

#define MEMORY_TYPE_DECLARE_ENUM(type, human_readable) type,

/**
 *
 */
enum MemoryType {
  MEMORY_TYPES_DO(MEMORY_TYPE_DECLARE_ENUM)
      mt_number_of_types // number of memory types (mtDontTrack
  // is not included as validate type)
};

class ResourceObj {};
class StackObj {
private:
  void* operator new(size_t size);
  void* operator new[](size_t size);
  void operator delete(void* p);
  void operator delete[](void* p);
};
class MetaspaceObj {};

template <MemoryType type> class Allocator {
public:
  MemoryType GetType() { return type; }
};
class HeapAllocator : public Allocator<MtJavaHeap> {};
class ThreadAllocator : public Allocator<MtThread> {};
class ClassAllocator : public Allocator<MtClass> {};

} // namespace jvm
#endif // MINI_JVM__MEMORY_ALLOCATION_H
