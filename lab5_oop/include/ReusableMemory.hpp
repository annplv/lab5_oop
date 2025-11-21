#ifndef REUSABLE_MEMORY_HPP
#define REUSABLE_MEMORY_HPP

#include <map>
#include <vector>
#include <memory_resource>
#include <new>

class ReusableMemory : public std::pmr::memory_resource {
    struct Block {
        void* ptr;
        size_t size;
        size_t alignment;
    };

    std::map<void*, Block> used;
    std::map<size_t, std::vector<Block>> freeBlocks;

protected:
    void* do_allocate(size_t bytes, size_t align) override {
        auto it = freeBlocks.lower_bound(bytes);
        while (it != freeBlocks.end()) {
            auto& vec = it->second;
            for (size_t i = 0; i < vec.size(); ++i) {
                if (vec[i].alignment >= align) {
                    Block b = vec[i];
                    vec.erase(vec.begin() + i);
                    if (vec.empty()) freeBlocks.erase(it);
                    used[b.ptr] = b;
                    return b.ptr;
                }
            }
            ++it;
        }
        void* p = ::operator new(bytes, std::align_val_t(align));
        used[p] = Block{ p, bytes, align };
        return p;
    }

    void do_deallocate(void* p, size_t bytes, size_t align) override {
        auto it = used.find(p);
        if (it != used.end()) {
            Block b = it->second;
            used.erase(it);
            freeBlocks[b.size].push_back(b);
        }
        else {
            freeBlocks[bytes].push_back(Block{ p, bytes, align });
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

public:
    ~ReusableMemory() {
        for (auto& kv : used) ::operator delete(kv.first, kv.second.size);
        for (auto& kv : freeBlocks)
            for (auto& b : kv.second) ::operator delete(b.ptr, b.size);
    }
};

#endif 

