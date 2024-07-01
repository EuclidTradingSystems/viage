#include "client.hpp"
#include <bridge.hpp>
#include <user_filter_model.hpp>
#include <wrapped_calculator.hpp>
#include <wrapped_nested_list.hpp>
#include <account_filter_model.hpp>

#include <Data/Item/contact.hpp>
#include <Data/Item/habitat.hpp>
#include <Data/Item/exterior.hpp>
#include <Data/Item/document.hpp>

client& client::instance()
{
    static client instance;
    return instance;
}

void client::init()
{
    using namespace Data;
    using namespace People;

    // calculator
    calculator = new Calculator::wrapped_calculator{};

    using namespace Wrapper;

    // accounts
    accounts = new wrapped_list<list<account>>{};
    accounts->makeConnections();

    // owners
    owners = new wrapped_nested_list<list<owner>, account>{accounts->get_inner()};

    // contacts
    contacts = new wrapped_nested_list<list<contact>, account>{accounts->get_inner()};

    // habitat
    habitat = new wrapped_nested_item<Places::habitat, account>{};
    habitat->makeConnections(accounts->get_inner());

    // exterior
    exterior = new wrapped_nested_item<Places::exterior, account>{};
    exterior->makeConnections(accounts->get_inner());

    // documents
    documents = new wrapped_nested_list<list<document>, account>{accounts->get_inner()};

    // users
    users = new wrapped_list<list<user>>{};
    users->makeConnections();

    // companies
    companies = new wrapped_list<list<company>>{};
    companies->makeConnections();

    // teams
    teams = new wrapped_nested_list<list<team>, company>{companies->get_inner(), };

    using namespace Interface;

    // bridge
    bridge::instance().registerQml();

    QObject::connect(&netManager::instance(),
                     &netManager::loggedIn,
                     [this]
                     (bool success)
                     {
                         if (success)
                         {
                             accounts->get();

                             if (bridge::instance().getClearance()
                                 == user::Administrator)
                             {
                                 users->get();
                                 companies->get();
                             }
                         }
                     });

    QObject::connect(&bridge::instance(),
                     &bridge::logout,
                     [this]
                     ()
                     {
                         owners->get_inner()->clear();
                         contacts->get_inner()->clear();
                         habitat->get_inner()->clear();
                         exterior->get_inner()->clear();
                         documents->get_inner()->clear();
                     });

    // Onboarding
    QObject::connect(&bridge::instance(),
                     &bridge::preOnboarding,
                     [this]
                     (int id)
                     {
                         owners->get_inner()->loadFrom(id);
                         habitat->get_inner()->clear();
                         exterior->get_inner()->clear();
                     });

    // Update Owner
    QObject::connect(owners->get_inner(),
                     &list<owner>::postItemsAppended,
                     [owners = owners->get_inner()] ()
                     {
                         const auto items{owners->items()};
                         qsizetype s{items.size()};

                         if (s < 2) return;

                         const auto previous{items.at(s - 2)};
                         if (previous.civilStatus == owner::Maried)
                         {
                             auto item{items.at(s - 1)};

                             item.sex = previous.sex == senior_citizen::M
                                            ? senior_citizen::F
                                            : senior_citizen::M;

                             item.address->street = previous.address->street;
                             item.address->city = previous.address->city;
                             item.address->zip = previous.address->zip;
                             item.address->canton = previous.address->canton;
                             item.civilStatus = previous.civilStatus;
                             item.lastName = previous.lastName;

                             owners->setItemAt(s - 1, item);
                         }
                     });
}

Data::list<Data::account>* client::get_accounts() const
{
    return accounts->get_inner();
}

Data::list<Data::document>* client::get_documents() const
{
    return documents->get_inner();
}

Data::list<Data::People::user>* client::get_users() const
{
    return users->get_inner();
}

const QString client::get_tempPath()
{
    return tempDir.path();
}

const QTemporaryDir client::tempDir{};

