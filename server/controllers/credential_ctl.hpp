#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace Data
{
namespace People
{
class credential_ctl : public HttpController<credential_ctl>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(credential_ctl::auth, "/auth?userName={}&password={}", Get);
    ADD_METHOD_TO(credential_ctl::auth, "/auth?userName={}&password={}&format={}&jsconfig={}&rememberMe={}", Get);
    METHOD_LIST_END

    void auth(const HttpRequestPtr& req,
              std::function<void (const HttpResponsePtr &)>&& callback,
              std::string&& userName,
              const std::string& password,
              const std::string& format = "",
              const std::string& jsonconfig = "",
              bool remeberMe = false);
};
}
}
