#include <fstream>

#include <drogon/HttpAppFramework.h>
#include <drogon/utils/Utilities.h>

#include "document_ctl.hpp"
#include <s_user.hpp>
#include <server.hpp>

namespace Data
{
void document_ctl::update(const HttpRequestPtr& req,
                          std::function<void (const HttpResponsePtr&)>&& callback) const
{
    LOG_INFO << "update " << s_document::key;

    Json::Value val{*req->jsonObject()};

    if (!(val.isMember("id") && val["id"].isInt()))
    {
        server::instance().error_reply(callback);
        return;
    }

    s_document item{};
    item.read(val);

    if (val.isMember("body") && val["body"].isString())
    {
        auto result{server::instance().execute(
            "SELECT AccountId FROM Document WHERE Id = "
            + std::to_string(item.id))};

        if (result.empty())
        {
            server::instance().error_reply(callback);
            return;
        }

        s_account acnt{};
        acnt.id = result.front()["AccountId"].as<int>();

        using namespace drogon::utils;
        item.set_directory(acnt.id);
        std::filesystem::create_directories(item.localPath);

        std::ofstream file(item.get_path(), std::ios::binary);
        if (!file.is_open())
        {
            server::instance().error_reply(callback);
            return;
        }

        const auto decoded{base64Decode(val["body"].asString())};
        if (!file.write(decoded.data(), decoded.size()))
        {
            server::instance().error_reply(callback);
            return;
        }
        file.close();

        item.state = document_item::Uploaded;

        server::instance().update(req,
                                  callback,
                                  item,
                                  &acnt,
                                  &item);
    }
    else
        server::instance().update(req,
                                  callback,
                                  item);
}

void document_ctl::remove(const HttpRequestPtr& req,
                          std::function<void (const HttpResponsePtr&)>&& callback) const
{
    LOG_INFO << "remove " << s_document::key;

    Json::Value val{*req->jsonObject()};

    if (!(val.isMember("id") && val["id"].isInt()))
    {
        server::instance().error_reply(callback);
        return;
    }

    s_document item{};
    item.id = val["id"].asInt();

    auto result{server::instance().execute(
        "SELECT RelativePath, FileName, Extension FROM Document WHERE Id = "
        + std::to_string(item.id))};

    item.set(result);

    const auto path{item.get_path()};
    if (!path.empty()) std::filesystem::remove(path);

    server::instance().remove(req,
                              callback,
                              item);
}
}
