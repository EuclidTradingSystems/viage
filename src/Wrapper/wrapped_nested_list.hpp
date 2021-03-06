#ifndef WRAPPED_NESTED_LIST_HPP
#define WRAPPED_NESTED_LIST_HPP

#include "wrapped_nested_item.hpp"

class QQmlContext;

namespace Data
{
class data_notifyer;
}

namespace Wrapper
{
template <typename Inner, typename Outer>
class wrapped_nested_list : public wrapped_nested_item<Inner, Outer>
{
public:
    explicit wrapped_nested_list(Interface::netManager *manager,
                Data::item_list<Outer>* parentList,
                QQmlContext* context = nullptr);
};

}

#include "wrapped_nested_list.cpp"
#endif // WRAPPED_NESTED_LIST_HPP
