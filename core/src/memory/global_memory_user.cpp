#include "fury/memory/global_memory_user.h"

MemoryManager *g_MemoryManager = new MemoryManager();


GlobalMemoryUser::GlobalMemoryUser() : m_MemoryManager(g_MemoryManager) {}

GlobalMemoryUser::~GlobalMemoryUser() {}

const void *GlobalMemoryUser::Allocate(size_t memSize, const char *user) {
    return m_MemoryManager->Allocate(memSize, user);
}

void GlobalMemoryUser::Free(void *pMem) {
    m_MemoryManager->Free(pMem);
}