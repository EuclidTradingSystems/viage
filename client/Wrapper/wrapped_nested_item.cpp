#pragma once
#include "wrapped_nested_item.hpp"
#include <client_utils.hpp>

namespace Wrapper
{
template <typename Inner, typename Outer>
wrapped_nested_item<Inner, Outer>::wrapped_nested_item()
    : wrapped_list<Inner>{}
{
}

template<typename Inner, typename Outer>
std::string wrapped_nested_item<Inner, Outer>::makeKey(Data::list<Outer>* parentList)
{
    std::string newkey = parentList->key();
    newkey.append("/");
    newkey.append(this->inner->key());

    return newkey;
}

template<typename Inner, typename Outer>
std::string wrapped_nested_item<Inner, Outer>::makeKey(Data::list<Outer>* parentList, int id)
{
    std::string newkey = parentList->key();
    newkey.append("/");
    newkey.append(std::to_string(id));
    newkey.append("/");
    newkey.append(this->inner->key());

    return newkey;
}

template<typename Inner, typename Outer>
void wrapped_nested_item<Inner, Outer>::makeConnections(Data::list<Outer>* parentList)
{
    this->connect(this->inner,
                  &Inner::validate,
                  this,
                  [this, parentList] (int id)
    {
        Outer outer{parentList->item_at_id(id)};

        if (outer.update(this->inner))
        {
            parentList->setItemAtId(id, outer);

            Interface::netManager::instance().putToKey(makeKey(parentList).str(),
                this->inner->toData(id),
                [=](const Json::Value& rep)
            {
                Json::Value json;
                Outer updated{};
                outer.write(json);
                updated.read(json);
                updated.read(rep);
                parentList->setItemAtId(id, updated);
            },
            "Validate error");
        }
    });

    this->connect(this->inner,
                  &Inner::loadFrom,
                  this,
                  [this, parentList] (int id)
    {
        Outer outer{parentList->item_at_id(id)};
        const auto json = outer.get(this->inner);

        this->inner->clear(); // clean previous load

        if (json.empty())
        {
            Interface::netManager::instance().getFromKey(makeKey(parentList, id).str(),
                                                         [this](const QByteArray& rep)
            {
                if(!rep.isEmpty())
                    this->inner->read(to_Json(rep));
            });
        }
        else
            this->inner->read(json);
    });

}

}
