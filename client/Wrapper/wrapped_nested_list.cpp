#pragma once

#include "qjsondocument.h"

#include "wrapped_nested_list.hpp"

namespace Wrapper
{
template <typename Inner, typename Outer>
wrapped_nested_list<Inner, Outer>::wrapped_nested_list(Data::list<Outer>* parentList)
    : wrapped_nested_item<Inner, Outer>{}
    , key{this->makeKey(parentList)}
{
    this->inner->complitionChecks();

    this->makeConnections(parentList);
    this->connectRemove();

    this->connect(this->inner,
                  &Inner::addIn,
                  this,
                  [this] (int id)
                  { add_in_with(id); });

    this->connect(this->inner,
                  &Inner::addInWith,
                  this,
                  [this, parentList] (int id, const QJsonObject& obj)
                  { add_in_with(id, obj, parentList); });
}

template<typename Inner, typename Outer>
void wrapped_nested_list<Inner, Outer>::add_in_with(int id,
                                                    const QJsonObject& obj,
                                                    Data::list<Outer>* parentList)
{
    auto json{obj};
    json[Outer::foreign_key] = id;
    QJsonDocument data{json};

    Interface::netManager::instance().postToKey(key.c_str(),
        data.toJson(),
        [this, obj, parentList, id] (const QJsonObject& res)
    {
        auto map{res.toVariantMap()};
        map.insert(obj.toVariantMap());

        const auto json{QJsonObject::fromVariantMap(map)};
        this->inner->appendWith(json);

        if (parentList)
        {
            Outer outer{parentList->item_at_id(id)};
            outer.update(this->inner);
            parentList->setItemAtId(id, outer);
        }
    },
    "addInWith error");
}

}
