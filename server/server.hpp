#ifndef SERVER_HPP
#define SERVER_HPP

#include <drogon/orm/Result.h>
#include <json/value.h>
#include <drogon/utils/Utilities.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

#include <Data/Item/s_user.hpp>
#include <s_list.hpp>

class server final
{
public:
    static server& instance();
    void init(const Json::Value& usr_config);

    server(server const&) = delete;
    void operator = (server const&) = delete;

    drogon::orm::Result execute(const std::string& query);
    void execute(const std::vector<std::string>& queries);

    void error_reply(std::function<void (const drogon::HttpResponsePtr& )>& callback);

    void handle_raw_query(const drogon::HttpRequestPtr& req,
                          std::function<void (const drogon::HttpResponsePtr&)>& callback,
                          const std::function<bool (Json::Value&, const Data::People::s_user&)>& handler);

    template <typename T, typename ...Foreign>
    void handle_query(const drogon::HttpRequestPtr& req,
                      std::function<void (const drogon::HttpResponsePtr&)>& callback,
                      const std::function<bool (Json::Value&, const Data::People::s_user&)>& handler,
                      T& item,
                      Foreign*... foreign)
    {
        drogon::HttpResponsePtr resp;
        auto session_id{req->session()->sessionId()};

        try
        {
            auto response{execute(Data::People::s_user::from_session_id(session_id))};

            if (response.empty())
            {
                resp = drogon::HttpResponse::newHttpResponse();
                resp->setStatusCode(drogon::k511NetworkAuthenticationRequired);
            }
            else
            {
                Data::People::s_user usr{};
                usr.set(response);

                Json::Value json;

                if (handler(json, usr))
                {
                    resp = drogon::HttpResponse::newHttpJsonResponse(json);
                }
                else
                {
                    if (json.empty())
                        resp = drogon::HttpResponse::newHttpResponse();
                    else
                        resp = drogon::HttpResponse::newHttpJsonResponse(json);

                    resp->setStatusCode(drogon::k404NotFound);
                }
            }
        }
        catch (...)
        {
            resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k500InternalServerError);
        }

        callback(resp);
    }

    template <typename T, typename ...Foreign>
    void insert(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr&)>& callback,
                T& item,
                Foreign*... foreign)
    {
        handle_query(req,
            callback,
            [this, &item, ... args = foreign]
            (Json::Value& json, const Data::People::s_user& usr)
            {
                const auto query{item.insert(usr, args...)};

                if (query.empty())
                {
                    json["success"] = false;
                    return false;
                }

                auto result{execute(query)};
                json["id"] = result.front()["Id"].template as<int>();
                json["success"] = true;

                return true;
            },
            item,
            foreign...);
    }

    template <typename T, typename ...Foreign>
    void search(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr&)>& callback,
                T& item,
                Foreign*... foreign)
    {
        handle_query(req,
            callback,
            [this, &item, ... args = foreign]
            (Json::Value& json, const Data::People::s_user& usr)
            {
                const auto query{T::search(usr, args...)};

                if (query.empty()) return false;

                auto result{execute(query)};

                item.set(result);
                item.write(json);

                return true;
            },
            item,
            foreign...);
    }

    template <typename T, typename ...Foreign>
    void select(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr&)>& callback,
                T& item,
                Foreign*... foreign)
    {
        handle_query(req,
            callback,
            [this, &item, ... args = foreign]
            (Json::Value& json, const Data::People::s_user& usr)
            {
                const auto query{item.select(usr, args...)};

                if (query.empty()) return false;

                auto result{execute(query)};

                item.set(result);
                item.write(json);

                return true;
            },
            item,
            foreign...);
    }

    template <typename T, typename ...Foreign>
    void update(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr&)>& callback,
                T& item,
                Foreign*... foreign)
    {
        handle_query(req,
            callback,
            [this, &item, ... args = foreign]
            (Json::Value& json, const Data::People::s_user& usr)
            {
                auto query{item.update(usr, args...)};

                std::string foreign_query{};
                T::foreign_update(foreign_query,
                                  item.is_completed(),
                                  args...);

                auto update_query{combine(query, foreign_query)};

                if (update_query.empty()) return false;

                try
                {
                    execute(update_query);
                }
                catch (...)
                {
                    json["success"] = false;
                    return true;
                }

                foreign_query.clear();
                T::select_updated(foreign_query, args...);

                if (foreign_query.empty()) return false;

                try
                {
                    auto result{execute(foreign_query)};
                    T::update_reply(result, json, args...);
                    json["success"] = true;
                }
                catch (...)
                {
                    json["success"] = false;
                }

                return true;
            },
            item,
            foreign...);
    }

    template <typename T, typename ...Foreign>
    void remove(const drogon::HttpRequestPtr& req,
                std::function<void (const drogon::HttpResponsePtr&)>& callback,
                T& item,
                Foreign*... foreign)
    {
        handle_query(req,
            callback,
            [this, &item, ... args = foreign]
            (Json::Value& json, const Data::People::s_user& usr)
            {
                auto query{item.remove(usr, args...)};

                if (query.empty()) return false;

                try
                {
                    execute(query);
                    json["success"] = true;
                }
                catch (...)
                {
                    json["success"] = false;
                }

                return true;
            },
            item,
            foreign...);
    }

private:
    server() {}

    std::vector<std::string> combine(const std::string& q1, const std::string& q2);
    std::vector<std::string> combine(std::vector<std::string>& q1, const std::string& q2);

    template <typename T, typename Foreign, typename ...Rest>
    bool select_all(T& item,
                    const Data::People::s_user& usr,
                    Foreign* foreign,
                    Rest*... rest)
    {
        const auto query{foreign->select(usr)};
        if (query.empty()) return false;

        auto result{execute(query)};
        if (result.empy()) return false;

        foreign->set(result);

        return selcet_recusive(item, usr, foreign, rest...);
    }

    template <typename T, typename Foreign, typename ...Rest>
    bool select_recursive(T& item,
                          const Data::People::s_user& usr,
                          Foreign* foreign,
                          Rest*... rest)
    {
        return select_all(item, usr, rest...);
    }

    template <typename T, typename Foreign>
    bool select_recursive(T& item,
                          const Data::People::s_user& usr,
                          Foreign* foreign)
    {
        const auto query{item.condition(usr, foreign)};
        if (query.empy()) return false;

        auto result{execute(query)};
        if (result.empy()) return false;

        item.set(result);
        return true;
    }

    template <typename T, typename ...Foreign>
    bool select_recusive(const Data::People::s_user& usr,
                         T* item,
                         Foreign*... foreign)
    {
        return select_all(*item, usr, foreign...);
    }
};

#endif // SERVER_HPP
