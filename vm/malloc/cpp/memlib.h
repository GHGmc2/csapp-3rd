//
// Created by Gmc2 on 2021/12/18.
//

#ifndef CSAPP3E_ALLOCATOR_VM_MALLOC_CPP_MEMLIB_H_
#define CSAPP3E_ALLOCATOR_VM_MALLOC_CPP_MEMLIB_H_

namespace custom_allocator {
// initialize the memory system model
void mem_init();

// simple model of the sbrk function, the heap cannot be shrunk in this model.
// extends the heap by incr bytes and returns the start address of the new area.
char *mem_sbrk(int incr);
}  // namespace custom_allocator

#endif // CSAPP3E_ALLOCATOR_VM_MALLOC_CPP_MEMLIB_H_
