#pragma once

#include "stack_allocator.h"

#define GLOBAL_MEMORY_CAPACITY 134217728 // 128 MB

class MemoryManager {
    friend class GlobalMemoryUser;

public:
    static constexpr size_t MEMORY_CAPACITY = GLOBAL_MEMORY_CAPACITY;

    void *m_GlobalMemory;
    StackAllocator *m_MemoryAllocator;
    std::vector<std::pair<const char *, void *>> m_PendingMemory;
    std::list<void *> m_FreedMemory;


public:

    MemoryManager(const MemoryManager &) = delete;

    MemoryManager &operator=(MemoryManager &) = delete;

    MemoryManager();

    ~MemoryManager();


    inline void *Allocate(size_t memSize, const char *user = nullptr) {
        log_info("%s allocated %d bytes of global memory.", user != nullptr ? user : "Unknown", memSize);
        void *pMemory = m_MemoryAllocator->Allocate(memSize, alignof(u8));

        this->m_PendingMemory.emplace_back(user, pMemory);

        return pMemory;
    }

    inline void Free(void *pMem) {
        if (pMem == this->m_PendingMemory.back().second) {
            this->m_MemoryAllocator->Free(pMem);
            this->m_PendingMemory.pop_back();

            bool bCheck = true;
            while (bCheck) {
                bCheck = false;

                // do not report already freed memory blocks.
                for (auto it : this->m_FreedMemory) {
                    if (it == this->m_PendingMemory.back().second) {
                        this->m_MemoryAllocator->Free(pMem);
                        this->m_PendingMemory.pop_back();
                        this->m_FreedMemory.remove(it);

                        bCheck = true;
                        break;
                    }
                }
            };

        } else {
            this->m_FreedMemory.push_back(pMem);
        }
    }

    void CheckMemoryLeaks();

};