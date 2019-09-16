#pragma once

#include <variant>

namespace exercise
{

template <class ValueType, class ErrorType>
class Expected
{
public:
    using MyVariant = std::variant<ValueType, ErrorType>; 

    Expected(ValueType const &v);
    explicit Expected(ErrorType const &e);
    virtual ~Expected() noexcept(false) {
        if (valueErrorVariant.index() == 1)
        {
            // in this exersize code is missing that checks
            // if this error was polled somewhen/somewhere
            // because if not, this error needs to be thrown, 
            // so the user knows, that something did go wrong here 
            // Don't hide the error if i cannot handle it myself
            throw std::get<1>(valueErrorVariant);
        }
    }; 

    bool hasError() const;
    bool hasValue() const;

    ValueType* operator->();
    ValueType& operator*();

private:
    MyVariant valueErrorVariant;
};

}
