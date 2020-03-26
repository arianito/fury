#pragma once

#include "fury/trace.h"
#include "fury/system.h"


class Allocator {
protected:

    const size_t m_MemorySize;
    const void *m_MemoryFirstAddress;

    size_t m_MemoryUsed;
    u64 m_MemoryAllocations;

public:

    Allocator(size_t memSize, const void *mem);

    virtual ~Allocator();

    virtual void *allocate(size_t size, u8 alignment) = 0;

    virtual void free(void *p) = 0;

    virtual void clear() = 0;

    static inline void *alignForward(void *address, u8 alignment) {
        return (void *) ((reinterpret_cast<std::uintptr_t>(address) + static_cast<std::uintptr_t>(alignment - 1)) &
                         static_cast<std::uintptr_t>(~(u8) (alignment - 1)));
    }

    static inline u8 getAdjustment(const void *address, u8 alignment) {
        u8 adjustment = alignment - (reinterpret_cast<std::uintptr_t>(address) & static_cast<std::uintptr_t>(alignment - 1));

        return adjustment == alignment ? 0 : adjustment;
    }

    static inline u8 getAdjustment(const void *address, u8 alignment, u8 extra) {
        u8 adjustment = getAdjustment(address, alignment);

        u8 neededSpace = extra;

        if (adjustment < neededSpace) {
            neededSpace -= adjustment;
            adjustment += alignment * (neededSpace / alignment);

            if (neededSpace % alignment > 0)
                adjustment += alignment;
        }

        return adjustment;
    }

    inline size_t getMemorySize() const {
        return this->m_MemorySize;
    }

    inline const void *getMemoryAddress0() const {
        return this->m_MemoryFirstAddress;
    }

    inline size_t getUsedMemory() const {
        return this->m_MemoryUsed;
    }

    inline u64 getAllocationCount() const {
        return this->m_MemoryAllocations;
    }

};