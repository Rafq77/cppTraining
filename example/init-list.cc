#include <initializer_list>
#include <memory>
#include <functional>
#include <iostream>

// StaticVector w/ ctor taking initializer_list (and delegated ctor)
// not movable
template <class T>
class StaticVector
{
public:
    explicit StaticVector(int size)
      : dataSize{size}
      , data{new uint8_t[size*sizeof(T)]
             , [this](uint8_t *ptr)
             {
                 // destruct all elements
                 for (int i = 0; i != realSize; ++i)
                 {
                     pData[i].~T();
                 }
                 // free memory
                 delete [] ptr;
             }}
      , pData{static_cast<T *>((void *)(data.get()))}
    {}

    StaticVector(std::initializer_list<T> il)
      : StaticVector(il.size())
    {
        for (T const &i: il)
        {
            push_back(i);
        }
    }

    StaticVector(StaticVector const &) = delete;
    StaticVector(StaticVector &&) = delete;
    StaticVector &operator=(StaticVector const &) = delete;
    StaticVector &operator=(StaticVector &&) = delete;

    T *begin()
    {
        return pData;
    }

    T *end()
    {
        return pData + realSize;
    }

    template <typename U>
    T &push_back(U &&t)
    {
        // no checks
        new (pData + realSize) T(std::forward<U>(t));
        return pData[realSize++];
    }

    void pop()
    {
        pData[--realSize].~T();
    }

    T &operator[](int idx)
    {
        return pData[idx];
    }

    int size() const { return realSize; }

private:
    int dataSize;
    int realSize = 0;
    std::unique_ptr<uint8_t, std::function<void(uint8_t *)>> data;
    T *pData;
};


int main()
{
    StaticVector<int> c = { 4, 2, 6, 17, 88, 3};

    for (auto i: c)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    return 0;
}
