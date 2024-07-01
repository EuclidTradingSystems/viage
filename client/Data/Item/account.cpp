#include "qjsonarray.h"
#include "qjsonobject.h"

#include <list.hpp>
#include "contact.hpp"
#include "document.hpp"
#include "exterior.hpp"
#include "habitat.hpp"
#include "owner.hpp"
#include "account.hpp"

namespace Data
{
account::account()
    : state{Initialized}
    , created{QDateTime::currentDateTime()}
    , modified{QDateTime::currentDateTime()}
{}

QHash<int, QByteArray> account::roleNames()
{
    QHash<int, QByteArray> names;

    names[OwnersRole] = "owners";
    names[HabitatRole] = "habitat";
    names[DocumentsRole] = "documents";
    names[StateRole] = "state";
    names[ReceivedRole] = "receivedDate";
    names[TransmitedRole] = "transmitedDate";
    names[ExpertizedRole] = "expertiedDate";
    names[DecidedRole] = "decidedDate";
    names[NotarizedRole] = "notarizedDate";
    names[PaidRole] = "paidDate";
    names[CreatedRole] = "created";
    names[ModifiedRole] = "modified";
    names[AdvisorFirstNameRole] = "advisorFirstName";
    names[AdvisorLastNameRole] = "advisorLastName";
    names[CompanyRole] = "company";
    names[AcronymRole] = "acronym";
    names[IdRole] = "id";

    return names;
}

QVariant account::data(int role) const
{
    switch (role)
    {
    case OwnersRole:
        return owners.toVariantList();
    // case ContactsRole:
    //     return contacts.toVariantList();
    case HabitatRole:
        return habitat;
    // case ExteriorRole:
    //     return exterior;
    case DocumentsRole:
        return documents.toVariantList();
    case StateRole:
        return QVariant(state);
    case ReceivedRole:
        return QVariant(receivedDate);
    case TransmitedRole:
        return QVariant(transmitedDate);
    case ExpertizedRole:
        return QVariant(expertizedDate);
    case DecidedRole:
        return QVariant(decidedDate);
    case NotarizedRole:
        return QVariant(notarizedDate);
    case PaidRole:
        return QVariant(paidDate);
    case CreatedRole:
        return QVariant(created);
    case ModifiedRole:
        return QVariant(modified);
    case AdvisorFirstNameRole:
        return QVariant(advisorFirstName);
    case AdvisorLastNameRole:
        return QVariant(advisorLastName);
    case CompanyRole:
        return QVariant(company);
    case AcronymRole:
        return QVariant(acronym);
    case IdRole:
        return QVariant(id);
    }

    return QVariant{};
}

void account::setData(const QVariant &value, int role)
{
//    switch (role)
//    {
//    case OwnersRole:
//        owners = value.toJsonArray();
//        break;
//    case ContactsRole:
//        contacts = value.toJsonArray();
//        break;
//    case HabitatRole:
//        habitat = value.toJsonObject();
//        break;
//    case ExteriorRole:
//        exterior = value.toJsonObject();
//        break;
//    case DocumentsRole:
//        documents = value.toJsonArray();
//        break;
//    }
}

bool account::update(list<People::owner>* ol)
{
    QJsonArray arr{};
    ol->write(arr);
    if (owners == arr)
        return false;

    owners = arr;
    modified = QDateTime::currentDateTime();
    return true;
}

bool account::update(list<People::contact>* cl)
{
    QJsonArray arr{};
    cl->write(arr);
    // exception for first updating empty contacts
    if (contacts.isEmpty() && arr.isEmpty())
        return true;
    else if (contacts == arr)
        return false;

    contacts = arr;
    modified = QDateTime::currentDateTime();
    return true;
}

bool account::update(Places::habitat* ht)
{
    QJsonObject obj{};
    ht->write(obj);
    if (habitat == obj)
        return false;

    habitat = obj;
    modified = QDateTime::currentDateTime();
    return true;
}

// bool account::update(Places::exterior* er)
// {
//     QJsonObject obj{};
//     er->write(obj);
//     if (exterior == obj)
//         return false;

//     exterior = obj;
//     modified = QDateTime::currentDateTime();
//     return true;
// }

bool account::update(list<document>* ds)
{
    QJsonArray arr{};
    ds->write(arr);
    // exception

    documents = arr;
    modified = QDateTime::currentDateTime();
    return true;
}

QJsonArray account::get(list<People::owner> *ol) const
{
    if (owners.isEmpty())
        return owners;
    else
    {
        if (owners[0].toObject().contains("id"))
            return owners;
        else
            return {};
        // no need to retreive initial "names only" array
    }
}

QJsonArray account::get(list<People::contact> *cl) const
{
    // TODO : differentiate between "unfetched" and empty contacts
    return contacts;
}

QJsonObject account::get(Places::habitat* ht) const
{
    if (habitat.contains("id"))
        return habitat;
    else
        return {};
}

// QJsonObject account::get(Places::exterior* er) const
// {
//     if (exterior.contains("id"))
//         return exterior;
//     else
//         return {};
// }

QJsonArray account::get(list<document>* ds) const
{
    if (documents.isEmpty())
        return documents;
    else
    {
        if (documents[0].toObject().contains("id"))
            return documents;
        else
            return {};
    }
}

void account::read(const QJsonObject& json)
{
    if (json.contains("owners") && json["owners"].isArray())
        owners = json["owners"].toArray();

    if (json.contains("contacts") && json["contacts"].isArray())
        contacts = json["contacts"].toArray();

    if (json.contains("habitat") && json["habitat"].isObject())
        habitat = json["habitat"].toObject();

    // if (json.contains("exterior") && json["exterior"].isObject())
    //     exterior = json["exterior"].toObject();

    if (json.contains("documents") && json["documents"].isArray())
        documents = json["documents"].toArray();

    if (json.contains("created") && json["created"].isString())
        created = QDateTime::fromString(json["created"].toString(), "yyyy-MM-dd hh:mm:ss");

    if (json.contains("modified") && json["modified"].isString())
        modified = QDateTime::fromString(json["modified"].toString(), "yyyy-MM-dd hh:mm:ss");

    if (json.contains("acronym") && json["acronym"].isString())
        acronym = json["acronym"].toString();

    if (json.contains("advisorFirstName") && json["advisorFirstName"].isString())
        advisorFirstName = json["advisorFirstName"].toString();

    if (json.contains("advisorLastName") && json["advisorLastName"].isString())
        advisorLastName = json["advisorLastName"].toString();

    if (json.contains("company") && json["company"].isString())
        company = json["company"].toString();

    if (json.contains("id") && json["id"].isDouble())
        id = json["id"].toInt();

    if (json.contains("state") && json["state"].isDouble())
        state = states(json["state"].toInt());

    if (json.contains("accountState") && json["accountState"].isDouble())
        state = states(json["accountState"].toInt());

    if (json.contains("receivedDate") && json["receivedDate"].isString())
        receivedDate = QDateTime::fromString(json["receivedDate"].toString(), "yyyy-MM-dd hh:mm:ss").date();

    if (json.contains("transmitedDate") && json["transmitedDate"].isString())
        transmitedDate = QDateTime::fromString(json["transmitedDate"].toString(), "yyyy-MM-dd hh:mm:ss").date();

    if (json.contains("expertizedDate") && json["expertizedDate"].isString())
        expertizedDate = QDateTime::fromString(json["expertizedDate"].toString(), "yyyy-MM-dd hh:mm:ss").date();

    if (json.contains("decidedDate") && json["decidedDate"].isString())
        decidedDate = QDateTime::fromString(json["decidedDate"].toString(), "yyyy-MM-dd hh:mm:ss").date();

    if (json.contains("notarizedDate") && json["notarizedDate"].isString())
        notarizedDate = QDateTime::fromString(json["notarizedDate"].toString(), "yyyy-MM-dd hh:mm:ss").date();

    if (json.contains("paidDate") && json["paidDate"].isString())
        paidDate = QDateTime::fromString(json["paidDate"].toString(), "yyyy-MM-dd hh:mm:ss").date();
}

void account::write(QJsonObject& json) const
{
    json["owners"] = owners;
    json["contacts"] = contacts;
    json["habitat"] = habitat;
    // json["exterior"] = exterior;
    json["documents"] = documents;
    json["state"] = state;
    json["receivedDate"] = receivedDate.toString("dd.MM.yyyy");
    json["transmitedDate"] = transmitedDate.toString("dd.MM.yyyy");
    json["expertizedDate"] = expertizedDate.toString("dd.MM.yyyy");
    json["decidedDate"] = decidedDate.toString("dd.MM.yyyy");
    json["notarizedDate"] = notarizedDate.toString("dd.MM.yyyy");
    json["paidDateDate"] = paidDate.toString("dd.MM.yyyy");
    json["created"] = created.toString("yyyy-MM-dd hh:mm:ss");
    json["modified"] = modified.toString("yyyy-MM-dd hh:mm:ss");
    json["advisorFirstName"] = advisorFirstName;
    json["advisorLastName"] = advisorLastName;
    json["company"] = company;
    json["acronym"] = acronym;
    json["id"] = id;
}

bool account::is_completed() const
{
    return (state & Onboarded) == Onboarded;
}

}
