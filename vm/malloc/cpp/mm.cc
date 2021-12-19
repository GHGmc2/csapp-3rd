//
// Created by Gmc2 on 2021/12/18.
//

#include "mm.h"
#include "memlib.h"

#include <algorithm>
#include <cstddef>

namespace custom_allocator {
// pointer to first block
static char *heap_listp = nullptr;

namespace {
void *extend_heap(size_t words);
void place(void *bp, size_t asize);
void *find_fit(size_t asize);
void *coalesce(void *bp);
}  // namespace

int mm_init() {
  if ((heap_listp = mem_sbrk(4 * WSIZE)) == nullptr) {
    return -1;
  }

  // alignment padding
  Put(heap_listp, 0);
  // prologue header
  Put(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
  // prologue footer
  Put(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
  // epilogue header
  Put(heap_listp + (3 * WSIZE), PACK(0, 1));
  heap_listp += (2 * WSIZE);

  // extend the empty heap with a free block of CHUNKSIZE bytes
  if (extend_heap(CHUNKSIZE / WSIZE) == nullptr) {
    return -1;
  }

  return 0;
}

void *mm_malloc(size_t size) {
  // adjusted block size
  size_t a_size;
  // amount to extend heap if no fit
  size_t extend_size;
  void *bp;

  if (size == 0) {
    return nullptr;
  }

  // 8 bytes for alignment, 8 bytes for header and footer
  if (size <= DSIZE) {
    a_size = 2 * DSIZE;
  } else {
    a_size = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);
  }

  if ((bp = find_fit(a_size)) != nullptr) {
    place(bp, a_size);
    return bp;
  }

  // no fit found, get more memory and place the block
  extend_size = a_size > CHUNKSIZE ? a_size : CHUNKSIZE;
  if ((bp = extend_heap(extend_size / WSIZE)) == nullptr) {
    return nullptr;
  }
  place(bp, a_size);
  return bp;
}

void mm_free(void *bp) {
  size_t size = GetSize(HdrP(bp));

  Put(HdrP(bp), PACK(size, 0));
  Put(FtrP(bp), PACK(size, 0));
  coalesce(bp);
}

namespace {
void *coalesce(void *bp) {
  size_t prev_alloc = GetAlloc(FtrP(PrevBlkP(bp)));
  size_t next_alloc = GetAlloc(HdrP(NextBlkP(bp)));
  size_t size = GetSize(HdrP(bp));

  if (prev_alloc && next_alloc) {
    return bp;
  } else if (prev_alloc && !next_alloc) {
    size += GetSize(HdrP(NextBlkP(bp)));
    Put(HdrP(bp), PACK(size, 0));
    Put(FtrP(bp), PACK(size, 0));
  } else if (!prev_alloc && next_alloc) {
    size += GetSize(HdrP(PrevBlkP(bp)));
    Put(FtrP(bp), PACK(size, 0));
    Put(HdrP(PrevBlkP(bp)), PACK(size, 0));
    bp = PrevBlkP(bp);
  } else {
    size += GetSize(HdrP(PrevBlkP(bp))) + GetSize(FtrP(NextBlkP(bp)));
    Put(HdrP(PrevBlkP(bp)), PACK(size, 0));
    Put(FtrP(NextBlkP(bp)), PACK(size, 0));
    bp = PrevBlkP(bp);
  }

  return bp;
}

void *extend_heap(size_t words) {
  char *bp = nullptr;
  size_t size = 0;

  // allocate an even number of words to maintain alignment
  size = (words % 2 != 0) ? (words + 1) * WSIZE : words * WSIZE;
  if ((bp = mem_sbrk(size)) == nullptr) {
    return nullptr;
  }

  // prologue header and footer
  Put(HdrP(bp), PACK(size, 0));
  Put(FtrP(bp), PACK(size, 0));
  // epilogue header
  Put(HdrP(NextBlkP(bp)), PACK(0, 1));

  // coalesce if the previous block was free
  return coalesce(bp);
}

void place(void *bp, size_t a_size) {
  size_t c_size = GetSize(HdrP(bp));

  // split if remainder would be at least minimum block size
  if ((c_size - a_size) >= (2 * DSIZE)) {
    Put(HdrP(bp), PACK(a_size, 1));
    Put(FtrP(bp), PACK(a_size, 1));
    bp = NextBlkP(bp);
    Put(HdrP(bp), PACK(c_size - a_size, 0));
    Put(FtrP(bp), PACK(c_size - a_size, 0));
  } else {
    Put(HdrP(bp), PACK(c_size, 1));
    Put(FtrP(bp), PACK(c_size, 1));
  }
}

void *find_fit(size_t a_size) {
  void *bp;
  // first-fit search
  for (bp = heap_listp; GetSize(HdrP(bp)) > 0; bp = NextBlkP(bp)) {
    if (!GetAlloc(HdrP(bp)) && a_size <= GetSize(HdrP(bp))) {
      return bp;
    }
  }

  return nullptr;
}
}  // namespace
}  // namespace custom_allocator