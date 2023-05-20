#include "fury/memory/pool_allocator.h"

PoolAllocator::PoolAllocator(size_t memSize, const void *mem, size_t objectSize, u8 objectAlignment) :
        Allocator(memSize, mem),
        OBJECT_SIZE(objectSize),
        OBJECT_ALIGNMENT(objectAlignment) {
    this->Clear();
}

PoolAllocator::~PoolAllocator() {
    this->freeList = nullptr;
}

void *PoolAllocator::Allocate(size_t memSize, u8 alignment) {
    assert(memSize > 0 && "allocate called with memSize = 0.");
    assert(memSize == this->OBJECT_SIZE && alignment == this->OBJECT_ALIGNMENT);
    if (this->freeList == nullptr)
        return nullptr;
    void *p = this->freeList;
    this->freeList = (void **) (*this->freeList);
    this->m_MemoryUsed += this->OBJECT_SIZE;
    this->m_MemoryAllocations++;

    return p;
}

void PoolAllocator::Free(void *mem) {
    *((void **) mem) = this->freeList;
    this->freeList = (void **) mem;
    this->m_MemoryUsed -= this->OBJECT_SIZE;
    this->m_MemoryAllocations--;
}

void PoolAllocator::Clear() {
    u8 adjustment = GetAdjustment(this->m_MemoryFirstAddress, this->OBJECT_ALIGNMENT);
    size_t numObjects = (size_t) floor((this->m_MemorySize - adjustment) / this->OBJECT_SIZE);
    union {
        void *asVoidPtr;
        std::uintptr_t asUptr;
    };
    asVoidPtr = (void *) this->m_MemoryFirstAddress;
    asUptr += adjustment;
    this->freeList = (void **) asVoidPtr;
    void **p = this->freeList;
    for (int i = 0; i < (numObjects - 1); ++i) {
        *p = (void *) ((std::uintptr_t) p + this->OBJECT_SIZE);
        p = (void **) *p;
    }
    *p = nullptr;
}