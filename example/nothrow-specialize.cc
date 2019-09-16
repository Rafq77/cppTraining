// testing specialization on nothrow
#include <type_traits>
#include <iostream>

#ifdef NO_ENABLE_IF
template <typename T, bool>
struct fooImpl
{
    static void foo(T &)
    {
        std::cout << "throwing variant" << std::endl;
    }
};

template <typename T>
struct fooImpl<T, true>
{
    static void foo(T &)
    {
        std::cout << "non-throwing variant" << std::endl;
    }
};

template <typename T>
void foo(T &t)
{
    fooImpl<T, std::is_nothrow_copy_constructible<T>::value>::foo(t);
}
#else
template <typename T>
typename std::enable_if<std::is_nothrow_copy_constructible<T>::value>::type
foo(T &t)
{
    std::cout << "non-throwing variant" << std::endl;
}

template <typename T>
typename std::enable_if<!std::is_nothrow_copy_constructible<T>::value>::type
foo(T &t)
{
    std::cout << "throwing variant" << std::endl;
}
#endif

class X
{
public:
    X(X const &rhs)
      : i{rhs.i}
    {
        if (i == 0) throw 0;
    }
    X(int i_)
      : i{i_}
    {}

private:
    int i;
};

int main()
{
    int i{15};
    X x{22};

    foo(i);
    foo(x);

    return 0;
}
