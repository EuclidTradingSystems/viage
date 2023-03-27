#include <drogon/HttpController.h>

#include <s_list.hpp>
#include <server.hpp>

using namespace drogon;

namespace Data
{
template <typename T, typename I>
struct list_ctl : public HttpController<T>
{
    virtual void insert(const HttpRequestPtr& req,
                        std::function<void (const HttpResponsePtr &)>&& callback) const
    {
        LOG_INFO << "insert " << I::key;

        I item{};
        item.read(*req->jsonObject());

        server::server::get().insert(req,
                                     callback,
                                     item);
    }

    virtual void select(const HttpRequestPtr& req,
                        std::function<void (const HttpResponsePtr &)>&& callback,
                        int id) const
    {
        LOG_INFO << "select " << I::key;

        I item{};
        item.id = id;

        server::server::get().select(req,
                                     callback,
                                     item);
    }

    virtual void update(const HttpRequestPtr& req,
                        std::function<void (const HttpResponsePtr &)>&& callback) const
    {
        LOG_INFO << "update " << I::key;

        Json::Value val{*req->jsonObject()};
        I item{};

        if (val.isMember(I::key))
            item.read(val[I::key]);
        else
            item.read(val);

        if (item.id == 0)
        {
            server::server::get().error_reply(callback);
            return;
        }

        server::server::get().update(req,
                                     callback,
                                     item);
    }

    virtual void search(const HttpRequestPtr& req,
                        std::function<void (const HttpResponsePtr &)>&& callback) const
    {
        LOG_INFO << "search " << I::key;

        s_list<I> list{};

        server::server::get().search(req,
                                     callback,
                                     list);
    }
};

}
