#pragma once
#include <wobjectimpl.h>

#include "list_model.hpp"
#include "base_model.hpp"
#include "../Lists/simple_item_list.hpp"

namespace Data
{
W_OBJECT_IMPL(list_model<T>, template <typename T>)

template <typename T>
list_model<T>::list_model(QObject *parent)
    : base_model<simple_item_list<T>, T>{parent}
{
}

}
