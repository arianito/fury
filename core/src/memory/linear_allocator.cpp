#include "fury/memory/linear_allocator.h"

LinearAllocator::LinearAllocator(size_t memSize, const void *mem) :
        Allocator(memSize, mem) {}

LinearAllocator::~LinearAllocator() {
    this->Clear();
}

void *LinearAllocator::Allocate(size_t memSize, u8 alignment) {
    assert(memSize > 0 && "allocate called with memSize = 0.");
    union {
        void *asVoidPtr;
        std::uintptr_t asUptr;
    };
    asVoidPtr = (void *) this->m_MemoryFirstAddress;
    asUptr += this->m_MemoryUsed;
    u8 adjustment = GetAdjustment(asVoidPtr, alignment);
    if (this->m_MemoryUsed + memSize + adjustment > this->m_MemorySize) {
        return nullptr;
    }
    asUptr += adjustment;
    this->m_MemoryUsed += memSize + adjustment;
    this->m_MemoryAllocations++;
    return asVoidPtr;
}

void LinearAllocator::Free(void *mem) {
    static_assert(true, "Lineaer allocators do not support free operations. Use clear instead.");
}

void LinearAllocator::Clear() {
    this->m_MemoryUsed = 0;
    this->m_MemoryAllocations = 0;
}
