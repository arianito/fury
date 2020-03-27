#pragma once

#include <list>
#include "pool_allocator.h"
#include "global_memory_user.h"

template<class OBJECT_TYPE, size_t T>
class MemoryChunkAllocator : protected GlobalMemoryUser {
    static const size_t MAX_OBJECTS = T;
    static const size_t ALLOC_SIZE = (sizeof(OBJECT_TYPE) + alignof(OBJECT_TYPE)) * MAX_OBJECTS;
    const char *m_AllocatorTag;

public:
    using Allocator  = PoolAllocator;
    using ObjectList = std::list<OBJECT_TYPE *>;

    class MemoryChunk {
    public:

        Allocator *allocator;
        ObjectList objects;

        std::uintptr_t chunkStart{};
        std::uintptr_t chunkEnd{};

        explicit MemoryChunk(Allocator *allocator) :
                allocator(allocator) {
            this->chunkStart = reinterpret_cast<std::uintptr_t>(allocator->GetMemoryAddress0());
            this->chunkEnd = this->chunkStart + ALLOC_SIZE;
            this->objects.clear();
        }

    };

    using MemoryChunks = std::list<MemoryChunk *>;

    class iterator : public std::iterator<std::forward_iterator_tag, OBJECT_TYPE> {
        typename MemoryChunks::iterator m_CurrentChunk;
        typename MemoryChunks::iterator m_End;

        typename ObjectList::iterator m_CurrentObject;


    public:

        iterator(typename MemoryChunks::iterator begin, typename MemoryChunks::iterator end) :
                m_CurrentChunk(begin),
                m_End(end) {
            if (begin != end) {
                assert((*m_CurrentChunk) != nullptr);
                m_CurrentObject = (*m_CurrentChunk)->objects.begin();
            } else {
                m_CurrentObject = (*std::prev(m_End))->objects.end();
            }
        }


        inline iterator &operator++() {
            m_CurrentObject++;
            if (m_CurrentObject == (*m_CurrentChunk)->objects.end()) {
                m_CurrentChunk++;
                if (m_CurrentChunk != m_End) {
                    assert((*m_CurrentChunk) != nullptr);
                    m_CurrentObject = (*m_CurrentChunk)->objects.begin();
                }
            }

            return *this;
        }

        inline OBJECT_TYPE &operator*() const { return *m_CurrentObject; }

        inline OBJECT_TYPE *operator->() const { return *m_CurrentObject; }

        inline bool operator==(iterator &other) {
            return ((this->m_CurrentChunk == other.m_CurrentChunk) && (this->m_CurrentObject == other.m_CurrentObject));
        }

        inline bool operator!=(iterator &other) {
            return ((this->m_CurrentChunk != other.m_CurrentChunk) && (this->m_CurrentObject != other.m_CurrentObject));
        }
    };

protected:

    MemoryChunks m_Chunks;

public:

    explicit MemoryChunkAllocator(const char *allocatorTag) : m_AllocatorTag(allocatorTag) {
        auto *allocator = new Allocator(ALLOC_SIZE, Allocate(ALLOC_SIZE, allocatorTag), sizeof(OBJECT_TYPE),
                                        alignof(OBJECT_TYPE));
        this->m_Chunks.push_back(new MemoryChunk(allocator));
    }

    ~MemoryChunkAllocator() {
        for (auto chunk : this->m_Chunks) {
            for (auto obj : chunk->objects)
                ((OBJECT_TYPE *) obj)->~OBJECT_TYPE();

            chunk->objects.clear();

            // free allocated allocator memory
            Free((void *) chunk->allocator->GetMemoryAddress0());
            delete chunk->allocator;
            chunk->allocator = nullptr;

            // delete helper chunk object
            delete chunk;
            chunk = nullptr;
        }
    }

    void *CreateObject() {
        void *slot = nullptr;

        // get next free slot
        for (auto chunk : this->m_Chunks) {
            if (chunk->objects.size() > MAX_OBJECTS)
                continue;

            slot = chunk->allocator->Allocate(sizeof(OBJECT_TYPE), alignof(OBJECT_TYPE));
            if (slot != nullptr) {
                chunk->objects.push_back((OBJECT_TYPE *) slot);
                break;
            }
        }

        // all chunks are full... allocate a new one
        if (slot == nullptr) {
            auto *allocator = new Allocator(ALLOC_SIZE, Allocate(ALLOC_SIZE, this->m_AllocatorTag),
                                            sizeof(OBJECT_TYPE), alignof(OBJECT_TYPE));
            auto *newChunk = new MemoryChunk(allocator);
            // put new chunk in front
            this->m_Chunks.push_front(newChunk);

            slot = newChunk->allocator->Allocate(sizeof(OBJECT_TYPE), alignof(OBJECT_TYPE));

            assert(slot != nullptr && "Unable to create new object. Out of memory?!");
            newChunk->objects.clear();
            newChunk->objects.push_back((OBJECT_TYPE *) slot);
        }

        return slot;
    }

    void DestroyObject(void *object) {
        std::uintptr_t adr = reinterpret_cast<std::uintptr_t>(object);
        for (auto chunk : this->m_Chunks) {
            if (chunk->chunkStart <= adr && adr < chunk->chunkEnd) {
                chunk->objects.remove((OBJECT_TYPE *) object);
                chunk->allocator->Free(object);
                return;
            }
        }
    }


    inline iterator begin() { return iterator(this->m_Chunks.begin(), this->m_Chunks.end()); }

    inline iterator end() { return iterator(this->m_Chunks.end(), this->m_Chunks.end()); }

};