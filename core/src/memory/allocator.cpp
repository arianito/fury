#include "fury/memory/allocator.h"

Allocator::Allocator(const size_t memSize, const void *mem) :
        m_MemorySize(memSize),
        m_MemoryFirstAddress(mem),
        m_MemoryUsed(0),
        m_MemoryAllocations(0) {}

Allocator::~Allocator() {}