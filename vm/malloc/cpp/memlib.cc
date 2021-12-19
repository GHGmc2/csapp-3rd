//
// Created by Gmc2 on 2021/12/18.
//

#include "memlib.h"

#include <cerrno>
#include <cstdlib>
#include <iostream>

#define ALIGNMENT 64

// 20 MB
#define MAX_HEAP (20*(1<<20))

namespace custom_allocator {
// points to first byte of heap
static char *mem_heap;
// points to last byte of heap plus 1
static char *mem_brk;
// max legal heap addr plus 1
static char *mem_max_addr;

void mem_init() {
  mem_heap = static_cast<char *>(aligned_alloc(ALIGNMENT, MAX_HEAP));
  mem_brk = mem_heap;
  mem_max_addr = mem_heap + MAX_HEAP;
}

char *mem_sbrk(int incr) {
  auto old_brk = mem_brk;
  if ((incr < 0) || (mem_brk + incr) > mem_max_addr) {
    errno = ENOMEM;
    std::cout << "ERROR: mem_sbrk failed. Ran out of memory." << std::endl;
    return nullptr;
  }

  mem_brk += incr;
  return old_brk;
}
}  // namespace custom_allocator
