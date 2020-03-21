//
// Created by aryan on 3/20/20.
//

#ifndef GAME_ALLOCATOR_H
#define GAME_ALLOCATOR_H

#include "fury/trace.h"
#include "fury/system.h"


class Allocator {
protected:

    const size m_MemorySize;
    const void *m_MemoryFirstAddress;

    size m_MemoryUsed;
    u64 m_MemoryAllocations;

public:

    Allocator(size memSize, const void *mem);

    virtual ~Allocator();

    virtual void *allocate(size size, u8 alignment) = 0;

    virtual void free(void *p) = 0;

    virtual void clear() = 0;

    static inline void *alignForward(void *address, u8 alignment) {
        return (void *) ((reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment - 1)) &
                         static_cast<uptr>(~(u8)(alignment - 1)));
    }

    static inline u8 getAdjustment(const void *address, u8 alignment) {
        u8 adjustment = alignment - (reinterpret_cast<uptr>(address) & static_cast<uptr>(alignment - 1));

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

    [[nodiscard]] inline size getMemorySize() const {
        return this->m_MemorySize;
    }

    [[nodiscard]] inline const void *getMemoryAddress0() const {
        return this->m_MemoryFirstAddress;
    }

    [[nodiscard]] inline size getUsedMemory() const {
        return this->m_MemoryUsed;
    }

    [[nodiscard]] inline u64 getAllocationCount() const {
        return this->m_MemoryAllocations;
    }

};

#endif