#include <drogon/HttpController.h>

#include <server.hpp>

using namespace drogon;

namespace Data
{
template <typename T, typename I, typename F, typename C = I>
struct base_controller : public HttpController<T>
{
    virtual void select(const HttpRequestPtr& req,
                        std::function<void (const HttpResponsePtr&)>&& callback,
                        int foreign_id) const
    {
        C item{};

        LOG_INFO << "select " << item.key;

        F foreign{};
        foreign.id = foreign_id;

        server::instance().select(req,
                                  callback,
                                  item,
                                  &foreign);
    }

    virtual void search(const HttpRequestPtr& req,
                        std::function<void (const HttpResponsePtr&)>&& callback,
                        int foreign_id) const
    {
        I item{};

        LOG_INFO << "search " << item.key;

        F foreign{};
        foreign.id = foreign_id;

        server::instance().search(req,
                                  callback,
                                  item,
                                  &foreign);
    }

    virtual void update_from(const HttpRequestPtr& req,
                             std::function<void (const HttpResponsePtr&)>&& callback) const
    {
        I item{};

        LOG_INFO << "update from " << item.key;

        Json::Value val{*req->jsonObject()};

        if (!(val.isMember("Id") && val["Id"].isInt()))
        {
            server::instance().error_reply(callback);
            return;
        }

        F foreign{};
        foreign.id = val["Id"].asInt();
        item.read(val[item.key]);

        server::instance().update(req,
                                  callback,
                                  item,
                                  &foreign);
    }
};

}
