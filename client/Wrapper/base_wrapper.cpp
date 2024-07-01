#pragma once

#include "qqml.h"
#include <QQmlContext>

#include <wobjectimpl.h>

#include <Interface/bridge.hpp>
#include "base_wrapper.hpp"

namespace Wrapper
{
W_OBJECT_IMPL(base_wrapper<Inner>, template <typename Inner>)

template <typename Inner>
base_wrapper<Inner>::base_wrapper()
    : QObject{}
    , inner{new Inner{}}
{
    registerToQml();
}

template<typename Inner>
void base_wrapper<Inner>::registerToQml() const
{
    qmlRegisterUncreatableType<Inner>(Inner::uri(), 1, 0, Inner::qmlName(), "");
    Interface::bridge::instance().context()->setContextProperty(Inner::key(), inner);
}

template<typename Inner>
Inner* base_wrapper<Inner>::get_inner() const
{
    return inner;
}
} // namespace Wrapper
