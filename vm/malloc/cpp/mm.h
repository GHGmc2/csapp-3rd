//
// Created by Gmc2 on 2021/12/18.
//

#ifndef CSAPP3E_ALLOCATOR_VM_MALLOC_CPP_MM_H_
#define CSAPP3E_ALLOCATOR_VM_MALLOC_CPP_MM_H_

#include <cstddef>

namespace custom_allocator {
// word and header/footer size (bytes)
#define WSIZE 4

// double word size (bytes)
#define DSIZE 8

// extend heap by this amount
#define CHUNKSIZE 1<<12

// pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// read a word at address p
inline unsigned int Get(void *p) {
  return *static_cast<unsigned int *>(p);
}
// write a word ate address p
inline void Put(void *p, unsigned int val) {
  *(static_cast<unsigned int *>(p)) = val;
}
// read the size from address p
inline unsigned int GetSize(void *p) {
  return Get(p) & ~0x7;
}
// read the allocated bit from address p
inline unsigned int GetAlloc(void *p) {
  return Get(p) & 0x1;
}
// compute header address of given block pointer bp
inline char *HdrP(void *bp) {
  return static_cast<char *>(bp) - WSIZE;
}
// compute footer address of given block pointer bp
inline char *FtrP(void *bp) {
  return static_cast<char *>(bp) + GetSize(HdrP(bp)) - DSIZE;
}
// compute address of next block of given block pointer bp
inline char *NextBlkP(void *bp) {
  return static_cast<char *>(bp) + GetSize(static_cast<char *>(bp) - WSIZE);
}
// compute address of previous block of given block pointer bp
inline char *PrevBlkP(void *bp) {
  return static_cast<char *>(bp) - GetSize(static_cast<char *>(bp) - DSIZE);
}

int mm_init();
void *mm_malloc(size_t size);
void mm_free(void *ptr);
}  // namespace custom_allocator

#endif // CSAPP3E_ALLOCATOR_VM_MALLOC_CPP_MM_H_
