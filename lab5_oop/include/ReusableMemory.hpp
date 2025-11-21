#ifndef REUSABLE_MEMORY_HPP
#define REUSABLE_MEMORY_HPP

#include <map>
#include <vector>
#include <memory_resource>
#include <new>

class ReusableMemory : public std::pmr::memory_resource {
    struct Block { void* ptr; std::size_t size; std::size_t alignment; };
    std::map<void*, Block> allocations;
    std::map<std::size_t, std::vector<Block>> free_lists;

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        auto it = free_lists.lower_bound(bytes);
        while (it != free_lists.end()) {
            auto& vec = it->second;
            for (auto vit = vec.begin(); vit != vec.end(); ++vit) {
                if (vit->alignment >= alignment) {
                    Block b = *vit;
                    vec.erase(vit);
                    if (vec.empty()) free_lists.erase(it);
                    allocations[b.ptr] = b;
                    return b.ptr;
                }
            }
            ++it;
        }
        void* p = ::operator new(bytes, std::align_val_t(alignment));
        allocations[p] = Block{ p, bytes, alignment };
        return p;
    }

    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        auto it = allocations.find(p);
        if (it != allocations.end()) {
            Block b = it->second;
            allocations.erase(it);
            free_lists[b.size].push_back(b);
            return;
        }
        free_lists[bytes].push_back(Block{ p, bytes, alignment });
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

public:
    ~ReusableMemory() {
        for (auto& kv : allocations)
            ::operator delete(kv.second.ptr, kv.second.size);
        for (auto& kv : free_lists)
            for (auto& b : kv.second)
                ::operator delete(b.ptr, b.size);
    }
};

#endif 
