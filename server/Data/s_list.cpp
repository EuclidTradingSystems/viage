#pragma once
#include "s_list.hpp"
#include <drogon/orm/ResultIterator.h>

namespace Data
{
template <typename T>
s_list<T>::s_list()
    : item_list<T>{}
{
}

template<typename T>
void s_list<T>::set(const Result& res)
{
    std::vector<T> vec{};

    for (const auto& row : res)
    {
        T item{};
        item.set(row);
        vec.push_back(item);
    }

    this->set_list(vec);
}

template<typename T>
template<typename ...Foreign>
std::vector<std::string> s_list<T>::update(const People::s_user& usr, const Foreign*... f) const
{
    std::vector<std::string> vec{};

//    if (item_list<T>::size() == 0) // handle empty lists
//        vec.emplace_back(" "); // prevent from returnig empty string;
//    else
        for (const auto& item : item_list<T>::m_items)
            vec.emplace_back(item.update(usr, f...));

    return vec;
}
} // namespace Data
