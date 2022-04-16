#ifndef WRAPPED_NESTED_LIST_HPP
#define WRAPPED_NESTED_LIST_HPP

#include <wobjectdefs.h>

#include <Wrapper/wrapped_nested_item.hpp>

class QQmlContext;

namespace Data
{
class data_notifyer;

namespace Wrapper
{

template <typename Inner, typename Outer>
class wrapped_nested_list : protected wrapped_nested_item<Inner, Outer>
{
    W_OBJECT(wrapped_nested_list)

public:
    wrapped_nested_list(Service::access *srv,
                item_list<Outer>* parentList,
                QQmlContext* context);
};

}
}

#include "wrapped_nested_list.cpp"
#endif // WRAPPED_NESTED_LIST_HPP