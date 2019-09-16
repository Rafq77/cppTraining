#include <iostream>
#include <string>

using std::cout;

// end of recursion: print the rest
void myPrintfImpl(char const *rest)
{
    while (*rest)
    {
        if (*rest == '%')
        {
            ++rest;
        }
        cout << *rest++;
    }    
}

template<typename T, typename... Ts>
void myPrintfImpl(char const *fmt, T val, Ts... args)
{
    while (*fmt)
    {
        if (*fmt == '%')
        {
            ++fmt;
            if (*fmt != '%')
            {
                cout << val;
                myPrintfImpl(fmt, args...);
                return;
            }
        }
        cout << *fmt++;
    }    
}

template<typename... Ts>
inline void myPrintf(const std::string &fmt, Ts... args)
{
    myPrintfImpl(fmt.c_str(), args...);
}

int main()
{
    std::string s{"Hello!"};
    myPrintf("string: %, char *: %, double: %, int: % and a bare %%\n",
             s, "C++", 3.14159, 42);

    return 0;
}
