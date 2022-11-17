#ifndef C_SIMPLE_LIST_HPP
#define C_SIMPLE_LIST_HPP

#include "c_base_list.hpp"

namespace Data
{
template <typename T>
class c_simple_list : public c_base_list<T>
{
public:
    explicit c_simple_list(QObject* parent = nullptr);

    const char* qmlName;
    static const constexpr auto uri{T::uri};
};
}

#include "c_simple_list.cpp"
#endif // C_SIMPLE_LIST_HPP