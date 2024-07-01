#pragma once
#include <QQmlContext>

#include "qjsondocument.h"
#include "wrapped_list.hpp"

#include <netManager.hpp>
#include <list.hpp>
#include <client_utils.hpp>

using namespace client_utils;

namespace Wrapper
{
template <typename Inner>
wrapped_list<Inner>::wrapped_list()
    : base_wrapper<Inner>{}
{}

template<typename Inner>
void wrapped_list<Inner>::registerToQml() const
{
    std::string str{Inner::qmlName()};
    str += "List";

    qmlRegisterUncreatableType<Inner>(Inner::uri(), 1, 0, str.c_str(), "");
    Interface::bridge::instance().context()->setContextProperty(Inner::key(), this->inner);
}

template<typename Inner>
void wrapped_list<Inner>::get() const
{
    Interface::netManager::instance().getFromKey(Inner::key(),
                                                 [this](const QByteArray& bytes)
    {
        this->inner->read(to_Json(bytes));
    });
}

template<typename Inner>
void wrapped_list<Inner>::makeConnections() const
{
    this->connect(this->inner,
                  &Inner::validate,
                  this,
                  [this] (int id)
    {
        QJsonObject obj{};
        auto item{this->inner->item_at_id(id)};
        item.write(obj);

        const auto key{Inner::key()};

        QJsonObject json{{ key, obj }};

        Interface::netManager::instance().putToKey(key,
            QJsonDocument{json}.toJson(),
            [this] (const QJsonObject& rep)
        {},
        "Validate error");
    });

    this->connect(this->inner,
                  &Inner::add,
                  this,
                  [this] ()
    {
        Interface::netManager::instance().postToKey(Inner::key(),
                          QByteArray{},
                          [this](const QJsonObject& rep)
        { this->inner->appendWith(rep); },
        "Add error");
    });

    this->connect(this->inner,
                  &Inner::addWith,
                  this,
                  [this] (const QJsonObject& obj)
    {
            Interface::netManager::instance().postToKey(Inner::key(),
            QJsonDocument{obj}.toJson(),
            [this, obj] (const QJsonObject& res)
        {
            auto map{res.toVariantMap()};
            map.insert(obj.toVariantMap());

            const auto json{QJsonObject::fromVariantMap(map)};
            this->inner->appendWith(json);
        },
        "addWith error");
    });

    connectRemove();
}

template<typename Inner>
void wrapped_list<Inner>::connectRemove() const
{
    this->connect(this->inner,
                  &Inner::remove,
                  this,
                  [this] (int id)
    {
        this->inner->erase(id);
        const QJsonObject json{{"id", id}};

        Interface::netManager::instance().deleteToKey(Inner::key(),
            QJsonDocument{json}.toJson(),
            [this, id](const QJsonObject&) {},
        "Remove Error");
    });
}
}
