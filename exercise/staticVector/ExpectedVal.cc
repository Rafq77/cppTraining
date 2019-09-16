#include "ExpectedVal.hh"

namespace exercise
{

template <class ValueType, class ErrorType>
Expected<ValueType, ErrorType>::Expected(ValueType const &v)
    : valueErrorVariant(v)
{
}

template <class ValueType, class ErrorType>
Expected<ValueType, ErrorType>::Expected(ErrorType const &v)
    : valueErrorVariant(v)
{
}

template <class ValueType, class ErrorType>
bool Expected<ValueType, ErrorType>::hasError() const
{
    return std::holds_alternative<ErrorType>(valueErrorVariant);
}

template <class ValueType, class ErrorType>
bool Expected<ValueType, ErrorType>::hasValue() const
{
    return valueErrorVariant.index() == 1;
}

template <class ValueType, class ErrorType>
ValueType* Expected<ValueType, ErrorType>::operator->()
{
    if (hasValue())
    {
        return &(std::get<0>(valueErrorVariant));
    }
    // else throw
}

template <class ValueType, class ErrorType>
ValueType& Expected<ValueType, ErrorType>::operator*()
{
    return *(operator->());
}

}
