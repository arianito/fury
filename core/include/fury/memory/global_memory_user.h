#pragma once

#include "memory_manager.h"

extern MemoryManager* g_MemoryManager;

class GlobalMemoryUser {
private:
    MemoryManager *m_MemoryManager = nullptr;

public:
    GlobalMemoryUser();
    ~GlobalMemoryUser();
    const void *Allocate(size_t memSize, const char *user);

    void Free(void *pMem);
};