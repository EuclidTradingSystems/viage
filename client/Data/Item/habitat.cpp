#include "habitat.hpp"
#include "wobjectimpl.h"

namespace Data
{
namespace Places
{
W_OBJECT_IMPL(habitat)

habitat::habitat(QObject* parent)
    : base_data{parent}
    , habitatType{None}
    , rooms{2}
    , rawSurface{50}
    , surface{50}
    , m2Constructed{25}
    , m2Available{25}
    , m3s{9000}
{}

void habitat::read(const QJsonObject& json)
{
    if (json.contains("address") && json["address"].isObject())
    {
        address.read(json["address"].toObject());
        emit streetChanged();
        emit cityChanged();
        emit zipChanged();
        emit cantonChanged();
    }

    if (json.contains("habitatType") && json["habitatType"].isDouble())
        setHabitatType(habitatTypes(json["habitatType"].toInt()));

    if (json.contains("rooms") && json["rooms"].isDouble())
        setRooms(json["rooms"].toInt());

    if (json.contains("rawSurface") && json["rawSurface"].isDouble())
        setRawSurface(json["rawSurface"].toInt());

    if (json.contains("surface") && json["surface"].isDouble())
        setSurface(json["surface"].toInt());

    if (json.contains("m2Constructed") && json["m2Constructed"].isDouble())
        setM2Constructed(json["m2Constructed"].toInt());

    if (json.contains("m2Available") && json["m2Available"].isDouble())
        setM2Available(json["m2Available"].toInt());

    if (json.contains("m3s") && json["m3s"].isDouble())
        setM3s(json["m3s"].toInt());

    emit loaded();
    checkCompleted();
}

void habitat::write(QJsonObject& json) const
{
    QJsonObject jsonAddress{};
    address.write(jsonAddress);

    json["address"] = jsonAddress;
    json["habitatType"] = habitatType;
    json["rooms"] = rooms;
    json["rawSurface"] = rawSurface;
    json["surface"] = surface;
    json["m2Constructed"] = m2Constructed;
    json["m2Available"] = m2Available;
    json["m3s"] = m3s;
}

void habitat::clear()
{
    address.street = "";
    emit streetChanged();
    address.city = "";
    emit cityChanged();
    address.zip = 9000;
    emit zipChanged();
    address.canton = "Appenzell";
    emit cantonChanged();
    habitatType = None;
    emit habitatTypeChanged();
    rooms = 2;
    emit roomsChanged();
    rawSurface = 50;
    emit rawSurfaceChanged();
    surface = 50;
    emit surfaceChanged();
    m2Constructed = 25;
    emit m2ConstructedChanged();
    m2Available = 25;
    emit m2AvailableChanged();
    m3s = 1000;
    emit m3sChanged();
    completed = false;
    emit completedChanged();
}

const QString habitat::getStreet() const
{
    return address.street;
}

void habitat::setStreet(const QString& newStreet)
{
    if (address.street == newStreet)
        return;
    address.street = newStreet;
    emit streetChanged();
    checkCompleted();
}

int habitat::getZip() const
{
    return address.zip;
}

void habitat::setZip(int newZip)
{
    if (address.zip == newZip)
        return;
    address.zip = newZip;
    emit zipChanged();
}

const QString habitat::getCanton() const
{
    return address.canton;
}

void habitat::setCanton(const QString& newCanton)
{
    if (address.canton == newCanton)
        return;
    address.canton = newCanton;
    emit cantonChanged();
}

const QString habitat::getCity() const
{
    return address.city;
}

void habitat::setCity(const QString& newCity)
{
    if (address.city == newCity)
        return;
    address.city = newCity;
    emit cityChanged();
    checkCompleted();
}

habitat::habitatTypes habitat::getHabitatType() const
{
    return habitatType;
}

void habitat::setHabitatType(const habitatTypes& newHabitatType)
{
    if (habitatType == newHabitatType)
        return;
    habitatType = newHabitatType;
    emit habitatTypeChanged();
    checkCompleted();
}

int habitat::getRooms() const
{
    return rooms;
}

void habitat::setRooms(int newRooms)
{
    if (rooms == newRooms)
        return;
    rooms = newRooms;
    emit roomsChanged();
}

int habitat::getRawSurface() const
{
    return rawSurface;
}

void habitat::setRawSurface(int newRawSurface)
{
    if (rawSurface == newRawSurface)
        return;
    rawSurface = newRawSurface;
    emit rawSurfaceChanged();
}

int habitat::getSurface() const
{
    return surface;
}

void habitat::setSurface(int newSurface)
{
    if (surface == newSurface)
        return;
    surface = newSurface;
    emit surfaceChanged();
}

int habitat::getM2Constructed() const
{
    return m2Constructed;
}

void habitat::setM2Constructed(int newM2Constructed)
{
    if (m2Constructed == newM2Constructed)
        return;
    m2Constructed = newM2Constructed;
    emit m2ConstructedChanged();
}

int habitat::getM2Available() const
{
    return m2Available;
}

void habitat::setM2Available(int newM2Available)
{
    if (m2Available == newM2Available)
        return;
    m2Available = newM2Available;
    emit m2AvailableChanged();
}

int habitat::getM3s() const
{
    return m3s;
}

void habitat::setM3s(int newM3s)
{
    if (m3s == newM3s)
        return;
    m3s = newM3s;
    emit m3sChanged();
}

void habitat::checkCompleted()
{
    setCompleted(is_completed());
}

}
}
