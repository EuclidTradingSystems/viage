#ifndef WRAPPED_NESTED_ITEM_HPP
#define WRAPPED_NESTED_ITEM_HPP

#include "wrapped_list.hpp"

namespace Data
{
class data_notifyer;
}

namespace Wrapper
{
template <typename Inner, typename Outer>
class wrapped_nested_item : public wrapped_list<Inner>
{
public:
    explicit wrapped_nested_item();

    void makeConnections(Data::list<Outer>* parentList);

protected:
    std::string makeKey(Data::list<Outer>* parentList);
    std::string makeKey(Data::list<Outer>* parentList, int id);
};

}

#include "wrapped_nested_item.cpp"
#endif // WRAPPED_NESTED_ITEM_HPP
