//
// Created by aryan on 3/20/20.
//

#include "fury/memory/memory_manager.h"


MemoryManager::MemoryManager() {
    log_info("Initialize MemoryManager!");

    // allocate global memory
    this->m_GlobalMemory = malloc(MemoryManager::MEMORY_CAPACITY);
    if (this->m_GlobalMemory != nullptr) {
        log_info("%d bytes of memory allocated.", MemoryManager::MEMORY_CAPACITY);
    } else {
        log_fatal("Failed to allocate %d bytes of memory!", MemoryManager::MEMORY_CAPACITY);
        assert(this->m_GlobalMemory != nullptr && "Failed to allocate global memory.");
    }

    // create allocator
    this->m_MemoryAllocator = new StackAllocator(MemoryManager::MEMORY_CAPACITY, this->m_GlobalMemory);
    assert(this->m_MemoryAllocator != nullptr && "Failed to create memory allocator!");

    this->m_PendingMemory.clear();
    this->m_FreedMemory.clear();
}

MemoryManager::~MemoryManager() {
    log_error("Releasing MemoryManager!");

    this->m_MemoryAllocator->clear();

    delete this->m_MemoryAllocator;
    this->m_MemoryAllocator = nullptr;

    free(this->m_GlobalMemory);
    this->m_GlobalMemory = nullptr;
}

void MemoryManager::checkMemoryLeaks() {
    assert(!(this->m_FreedMemory.size() > 0 && this->m_PendingMemory.size() == 0) && "Implementation failure!");

    if (this->m_PendingMemory.size() > 0) {
        log_error("!!!  M E M O R Y   L E A K   D E T E C T E D  !!!");
        log_error("!!!  M E M O R Y   L E A K   D E T E C T E D  !!!");
        log_error("!!!  M E M O R Y   L E A K   D E T E C T E D  !!!");

        for (auto i : this->m_PendingMemory) {
            bool isFreed = false;

            for (auto j : this->m_FreedMemory) {
                if (i.second == j) {
                    isFreed = true;
                    break;
                }
            }

            if (!isFreed) {
                log_error("\'%s\' memory user didn't release allocated memory %p!", i.first, i.second);
            }
        }
    } else {
        log_info("No memory leaks detected.");
    }
}