#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include "qnamespace.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qdatetime.h"

#include "base_item.hpp"

namespace Data
{
namespace People
{
struct owner;
struct contact;
}

namespace Places
{
struct habitat;
struct exterior;
}

struct document;

template <typename T>
struct list;

struct account final : public base_item<account>
{
    account();

    static consteval auto key() { return "account"; };
    static consteval auto qmlName() { return "Account"; };
    static consteval auto uri() { return "Data"; };

    enum states
    {
        Initialized = 0,
        OwnersCompleted = 1,
        ContactsCompleted = 2,
        HabitatCompleted = 4,
        ExteriorCompleted = 8,
        DocumentsCompleted = 16,
        Onboarded = 31,
        Received = 32,
        Transmited = 64,
        Expertized = 128,
        Decided = 256,
        Notarized = 512,
        Paid = 1024
    };

    enum roles
    {
        OwnersRole = Qt::UserRole,
        HabitatRole,
        DocumentsRole,
        StateRole,
        ReceivedRole,
        TransmitedRole,
        ExpertizedRole,
        DecidedRole,
        NotarizedRole,
        PaidRole,
        CreatedRole,
        ModifiedRole,
        AdvisorFirstNameRole,
        AdvisorLastNameRole,
        CompanyRole,
        AcronymRole,
        IdRole
    };

    static QHash<int, QByteArray> roleNames();

    QVariant data(int role) const;
    void setData(const QVariant& value, int role);

    bool update(list<People::owner>* ol);
    bool update(list<People::contact>* cl);
    bool update(Places::habitat* ht);
    // bool update(Places::exterior* er);
    bool update(list<document>* ds);

    QJsonArray get(list<People::owner>* ol) const;
    QJsonArray get(list<People::contact>* cl) const;
    QJsonObject get(Places::habitat* ht) const;
    // QJsonObject get(Places::exterior* er) const;
    QJsonArray get(list<document>* ds) const;

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) const override;

    bool is_completed() const override;

    int id;
    states state;
    bool ppe;
    QDate receivedDate;
    QDate transmitedDate;
    QDate expertizedDate;
    QDate decidedDate;
    QDate notarizedDate;
    QDate paidDate;

private:
    QJsonArray owners;
    QJsonArray contacts;
    QJsonObject habitat;
    // QJsonObject exterior;
    QJsonArray documents;


    QDateTime created;
    QDateTime modified;
    QString advisorFirstName;
    QString advisorLastName;
    QString company;

    QString acronym;
};
}

#endif // ACCOUNT_HPP
