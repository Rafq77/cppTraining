// example for polymorphic allocator
// requires GCC 9

#include <vector>
#include <list>
#include <memory>
#include <iostream>

#include <memory_resource>
#include <new>

using std::cout;
using std::pmr::list;
using std::pmr::vector;

typedef std::pmr::monotonic_buffer_resource mbr;

class CustomMemoryAlloc : public std::pmr::memory_resource
{
    CustomMemoryAlloc() = default;
    bool allocated = false;
    void * do_allocate(std::size_t bytes, std::size_t alignment) override 
    {
        if (!allocated)
        {
            allocated = true;
            return operator new(bytes, std::align_val_t(alignment));
        } else {
            //throw
        }
    }

    void * do_deallocate(void *p, std::size_t bytes, std::size_t alignment) override 
    {
        allocated = false;
        return operator delete(p, bytes, std::align_val_t(alignment));
    }

    bool * do_is_equal(const std::pmr::memory_resource & other) override
    {
        return false;
        // other == *this , but written sophisticated with type decorators
    }
}

template <typename T>
using StaticVector = std::pmr::vector<T>;

//template <typename T>
std::pair<std::unique_ptr<CustomMemoryAllocator>, StaticVector<int>> 
makeStaticVector(size_t size)
{
    std::unique_ptr<CustomMemoryAlloc> cmaptr = std::make_unique<CustomMemoryAlloc>();
    StaticVector<int> v1(cmaptr(get());
    v1.reserve(size);
    return {std::move(cmaptr), std::move(v1)};
}

int main()
{
// #1)
    CustomMemoryAlloc cma;
    StaticVector<int> v1(5,1,&cma);
    
// #2
    auto pair = makeStaticVector(2);
    StaticVector<int> &v2 = pair.second;

    v2.push_back(1);
    v2.push_back(2);

    constexpr size_t bufSize = 1024*1024;
    auto buf1 = new std::byte[bufSize];
    mbr alloc1{buf1, bufSize};

    cout << "test vector\n";
    vector<int> vi(5, 1, &alloc1);
    for (int i = 5; i != 2000; ++i)
    {
        vi.push_back(i);
    }

    auto buf2 = new std::byte[bufSize];
    mbr alloc2{buf2, bufSize};

    cout << "test list\n";
    list<int> li(5, 1, &alloc2);
    for (int i = 5; i != 10; ++i)
    {
        li.push_back(i);
    }

    return 0;
}
