#pragma once

#include "memory_manager.h"

extern MemoryManager* g_MemoryManager;

class GlobalMemoryUser {
public:
    MemoryManager *m_MemoryManager = nullptr;
    GlobalMemoryUser();
    ~GlobalMemoryUser();
    const void *Allocate(size_t memSize, const char *user);

    void Free(void *pMem);
};