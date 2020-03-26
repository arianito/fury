#include "fury/memory/stack_allocator.h"

StackAllocator::StackAllocator(size_t memSize, const void *mem) :
        Allocator(memSize, mem) {}

StackAllocator::~StackAllocator() {
    this->Clear();
}

void *StackAllocator::Allocate(size_t memSize, u8 alignment) {
    assert(memSize > 0 && "allocate called with memSize = 0.");
    union {
        void *asVoidPtr;
        std::uintptr_t asUptr;
        AllocMetaInfo *asMeta;
    };
    asVoidPtr = (void *) this->m_MemoryFirstAddress;
    asUptr += this->m_MemoryUsed;
    u8 adjustment = GetAdjustment(asVoidPtr, alignment, sizeof(AllocMetaInfo));
    if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize) {
        return nullptr;
    }
    asMeta->adjustment = adjustment;
    asUptr += adjustment;
    this->m_MemoryUsed += memSize + adjustment;
    this->m_MemoryAllocations++;
    return asVoidPtr;
}

void StackAllocator::Free(void *mem) {
    union {
        void *asVoidPtr;
        std::uintptr_t asUptr;
        AllocMetaInfo *asMeta;
    };
    asVoidPtr = mem;
    asUptr -= sizeof(AllocMetaInfo);
    this->m_MemoryUsed -= ((std::uintptr_t) this->m_MemoryFirstAddress + this->m_MemoryUsed) -
                          ((std::uintptr_t) mem + asMeta->adjustment);
    this->m_MemoryAllocations--;
}

void StackAllocator::Clear() {
    this->m_MemoryUsed = 0;
    this->m_MemoryAllocations = 0;
}