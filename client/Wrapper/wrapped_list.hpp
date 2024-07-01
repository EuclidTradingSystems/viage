#ifndef WRAPPED_LIST_HPP
#define WRAPPED_LIST_HPP

#include <base_wrapper.hpp>

class Outer;

namespace Data
{
template <typename Outer>
class c_list;
}

namespace Wrapper
{

template <typename Inner>
class wrapped_list : public base_wrapper<Inner>
{
public:
    explicit wrapped_list();

    void makeConnections() const;
    void get() const;

    void registerToQml() const override;

protected:
    void connectRemove() const;
};

}

#include "wrapped_list.cpp"
#endif // WRAPPED_LIST_HPP
