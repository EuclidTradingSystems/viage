#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QString>
#include <QTemporaryDir>

#include <Item/account.hpp>
#include <Item/owner.hpp>
#include <Item/user.hpp>
#include <Item/company.hpp>
#include <Item/team.hpp>

namespace Calculator
{
class wrapped_calculator;
}

namespace Wrapper
{
template <typename T>
class wrapped_list;

template <typename Inner, typename Outer>
class wrapped_nested_list;

template <typename Inner, typename Outer>
class wrapped_nested_item;
}

class client final
{
public:
    static client& instance();
    void init();

    client(client const&) = delete;
    void operator = (client const&) = delete;

    Data::list<Data::account>* get_accounts() const;
    Data::list<Data::document>* get_documents() const;
    Data::list<Data::People::user>* get_users() const;

    static const QTemporaryDir tempDir;
    static const QString get_tempPath();

private:
    client() {};

    Calculator::wrapped_calculator* calculator;

    Wrapper::wrapped_list<Data::list<Data::account>>* accounts;
    Wrapper::wrapped_nested_list<Data::list<Data::People::owner>, Data::account>* owners;
    Wrapper::wrapped_nested_list<Data::list<Data::People::contact>, Data::account>* contacts;
    Wrapper::wrapped_nested_item<Data::Places::habitat, Data::account>* habitat;
    Wrapper::wrapped_nested_item<Data::Places::exterior, Data::account>* exterior;
    Wrapper::wrapped_nested_list<Data::list<Data::document>, Data::account>* documents;

    Wrapper::wrapped_list<Data::list<Data::People::user>>* users;
    Wrapper::wrapped_list<Data::list<Data::company>>* companies;
    Wrapper::wrapped_list<Data::list<Data::team>>* teams;
};


#endif // CLIENT_HPP
