/*
 * Copyright (c) 2014-2015 Detlef Vollmann, vollmann engineering gmbh
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ALLOCMOCK_HH_SEEN_
#define ALLOCMOCK_HH_SEEN_
#include "trompeloeil.hpp"
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <set>


namespace exerciseTest
{
// GCC 5.1 doesn't implement correct use of allocator_traits in
// standard containers, so we need a full-fledged allocator :-(
// VC++ 14.0 gives unsolved crashes :-(
#ifndef _WIN32
template <typename T, size_t N>
class StackAllocator
{
public:
    typedef T value_type;
    typedef T *pointer;
    typedef T const *const_pointer;
    typedef T &reference;
    typedef T const &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    template <typename U>
    struct rebind
    {
        typedef StackAllocator<U, N> other;
    };

    StackAllocator()
      : hwm(0)
    {
    }

    // as all our allocators compare equal, copying and moving is fine
    // even if we neither copy nor move...
    // but we need to reset the size
    StackAllocator(StackAllocator &&)
      : hwm(0)
    {
    }
    StackAllocator(StackAllocator const &)
      : hwm(0)
    {
    }
    template <typename U, size_t M>
    StackAllocator(StackAllocator<U, M> const &)
      : hwm(0)
    {
    }
    template <typename U, size_t M>
    StackAllocator(StackAllocator<U, M> &&)
      : hwm(0)
    {
    }

    ~StackAllocator() = default;

    // assignment is not required, so we don't define them


    T* allocate(size_t n)
    {
        size_t oldHwm = hwm;
        hwm += n;
        if (hwm > N)
        {
            throw std::bad_alloc{};
        }
        return &(buffer[oldHwm]);
    }
    void deallocate(T*, size_t)
    {
        // space is only released by destructing the whole allocator
    }

    void construct(T *p, T const &v)
    {
        ::new (p) T{v};
    }
    void destroy(T *p)
    {
        p->~T();
    }

    template <typename U, size_t M>
    bool equals(const StackAllocator<U, M>&)
    {
        // we don't release anything, so all StackAllocators compare equal
        return true;
    }

private:
    template <typename U, size_t M> friend class StackAllocator;
    T buffer[N];
    size_t hwm;
};
template <typename T, size_t N, typename U, size_t M>
bool operator==(const StackAllocator<T, N>&l, const StackAllocator<U, M>&r)
{
    return l.equals(r);
}
template <typename T, size_t N, typename U, size_t M>
bool operator!=(const StackAllocator<T, N>&l, const StackAllocator<U, M>&r)
{
    return !(l == r);
}


class AllocTracer
{
public:
    virtual ~AllocTracer() = default;

    virtual void trackNew(void *, size_t) = 0;
    virtual void trackDelete(void *) = 0;

    //static AllocTracer globalObj;
};

class AllocMock
{
public:
    AllocMock(AllocTracer *t = nullptr)
      : pointers{}
      , tracer{t}
      , count{0}
      , good{true}
    {
        allocs.push_back(this);
    }
    ~AllocMock()
    {
        allocs.pop_back();
    }

    void *alloc(size_t n)
    {
        void *p{malloc(n)};
        // avoid recursion by allocs caused by tracer
        auto oldTracer = tracer;
        tracer = nullptr;
        if (oldTracer)
        {
            pointers.insert(p);
            oldTracer->trackNew(p, n);
        }
        tracer = oldTracer;
        ++count;
        return p;
    }
    void dealloc(void *p)
    {
        if (!p) return;
        auto oldTracer = tracer;
        tracer = nullptr;
        if (oldTracer)
        {
            oldTracer->trackDelete(p);
            auto found = pointers.find(p);
            if (found != pointers.end())
            {
                pointers.erase(found);
            } else {
                good = false;
            }
        }
        tracer = oldTracer;
        --count;
        free(p);
    }

    bool ok() const
    {
        return good;
    }

    int getCount() const
    {
        return count;
    }

    void setTracer(AllocTracer *t)
    {
        tracer = t;
    }

    static AllocMock &getCurrent()
    {
        return *(allocs.back());
    }

    // just a dummy to set vector's capacity
    static int reserveAllocs()
    {
        allocs.reserve(8);
        return 0;
    }

private:
    static std::vector<AllocMock *, StackAllocator<AllocMock *, 10>> allocs;

#if 0 // unordered_set doesn't work well with StackAllocator...
    std::unordered_set<void *
                       , std::hash<void *>
                       , std::equal_to<void *>
                       , StackAllocator<void *, 50>> pointers;
#else
    std::set<void *
             , std::less<void *>
             , StackAllocator<void *, 50>> pointers;
#endif
    AllocTracer *tracer;
    int count;
    bool good;
};

struct AllocTracerMock : public AllocTracer
{
    MAKE_MOCK2(trackNew, void(void *, size_t));
    MAKE_MOCK1(trackDelete, void(void *));
};

struct TraceSetter
{
    TraceSetter(AllocMock &a, AllocTracer *t)
      : alloc(&a)
    {
        a.setTracer(t);
    }
    ~TraceSetter()
    {
        alloc->setTracer(nullptr);
    }

    AllocMock *alloc;
};
struct TraceDeleter
{
    explicit TraceDeleter(AllocMock &a)
      : alloc(&a)
    {
    }
    ~TraceDeleter()
    {
        alloc->setTracer(nullptr);
    }

    AllocMock *alloc;
};
#endif /* _WIN32 */
} // namespace exerciseTest
#endif /* ALLOCMOCK_HH_SEEN_ */
