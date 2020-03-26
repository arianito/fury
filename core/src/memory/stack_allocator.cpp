//
// Created by aryan on 3/20/20.
//

#include "fury/memory/stack_allocator.h"

StackAllocator::StackAllocator(size_t memSize, const void *mem) :
        Allocator(memSize, mem) {}

StackAllocator::~StackAllocator() {
    this->clear();
}

void *StackAllocator::allocate(size_t memSize, u8 alignment) {
    assert(memSize > 0 && "allocate called with memSize = 0.");

    union {
        void *asVoidPtr;
        std::uintptr_t asUptr;
        AllocMetaInfo *asMeta;
    };

    asVoidPtr = (void *) this->m_MemoryFirstAddress;

    // current address
    asUptr += this->m_MemoryUsed;

    u8 adjustment = getAdjustment(asVoidPtr, alignment, sizeof(AllocMetaInfo));

    // check if there is enough memory available
    if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize) {
        // not enough memory
        return nullptr;
    }

    // store alignment in allocation meta info
    asMeta->adjustment = adjustment;

    // determine aligned memory address
    asUptr += adjustment;

    // update book keeping
    this->m_MemoryUsed += memSize + adjustment;
    this->m_MemoryAllocations++;

    // return aligned memory address
    return asVoidPtr;
}

void StackAllocator::free(void *mem) {
    union {
        void *asVoidPtr;
        std::uintptr_t asUptr;
        AllocMetaInfo *asMeta;
    };

    asVoidPtr = mem;

    // get meta info
    asUptr -= sizeof(AllocMetaInfo);

    // free used memory
    this->m_MemoryUsed -= ((std::uintptr_t) this->m_MemoryFirstAddress + this->m_MemoryUsed) - ((std::uintptr_t) mem + asMeta->adjustment);

    // decrement allocation count
    this->m_MemoryAllocations--;
}

void StackAllocator::clear() {
    // simply reset memory
    this->m_MemoryUsed = 0;
    this->m_MemoryAllocations = 0;
}