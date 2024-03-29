#include "c_habitat.hpp"
#include "wobjectimpl.h"

#include <client_utils.hpp>

using namespace client_utils;

namespace Data
{
namespace Places
{
W_OBJECT_IMPL(c_habitat)

c_habitat::c_habitat(QObject* parent)
    : c_base_data{parent}
    , habitat_item{}
{
}

void c_habitat::read(const Json::Value& json)
{
    std::string prev_street{address->street};
    std::string prev_city{address->city};
    int prev_zip{address->zip};
    std::string prev_canton{address->canton};

    habitatTypes prev_habitatType{habitatType};
    int prev_rooms{rooms};
    int prev_rawSurface{rawSurface};
    int prev_surface{surface};
    int prev_m2Constructed{m2Constructed};
    int prev_m2Available{m2Available};
    int prev_m3s{m3s};

    habitat_item::read(json);

    if (prev_street != address->street)
        emit streetChanged();

    if (prev_city != address->city)
        emit cityChanged();

    if (prev_zip != address->zip)
        emit zipChanged();

    if (prev_canton != address->canton)
        emit cantonChanged();

    if (prev_habitatType != habitatType)
        emit habitatTypeChanged();

    if (prev_rooms != rooms)
        emit roomsChanged();

    if (prev_rawSurface != rawSurface)
        emit rawSurfaceChanged();

    if (prev_surface != surface)
        emit surfaceChanged();

    if (prev_m2Constructed != m2Constructed)
        emit m2ConstructedChanged();

    if (prev_m2Available != m2Available)
        emit m2AvailableChanged();

    if (prev_m3s != m3s)
        emit m3sChanged();

    emit loaded();
    checkCompleted();
}

void c_habitat::write(Json::Value& json) const
{
    habitat_item::write(json);
}

void c_habitat::clear()
{
    address->street = "";
    emit streetChanged();
    address->city = "";
    emit cityChanged();
    address->zip = 9000;
    emit zipChanged();
    address->canton = "Appenzell";
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

const QString c_habitat::getStreet() const
{
    return to_QString(address->street);
}

void c_habitat::setStreet(const QString& newStreet)
{
    const std::string str{to_string(newStreet)};

    if (address->street == str)
        return;
    address->street = str;
    emit streetChanged();
    checkCompleted();
}

int c_habitat::getZip() const
{
    return address->zip;
}

void c_habitat::setZip(int newZip)
{
    if (address->zip == newZip)
        return;
    address->zip = newZip;
    emit zipChanged();
}

const QString c_habitat::getCanton() const
{
    return to_QString(address->canton);
}

void c_habitat::setCanton(const QString& newCanton)
{
    const std::string str{to_string(newCanton)};

    if (address->canton == str)
        return;
    address->canton = str;
    emit cantonChanged();
}

const QString c_habitat::getCity() const
{
    return to_QString(address->city);
}

void c_habitat::setCity(const QString &newCity)
{
    const std::string str{to_string(newCity)};

    if (address->city == str)
        return;
    address->city = str;
    emit cityChanged();
    checkCompleted();
}

const c_habitat::habitatTypes& c_habitat::getHabitatType() const
{
    return habitatType;
}

void c_habitat::setHabitatType(const habitatTypes& newHabitatType)
{
    if (habitatType == newHabitatType)
        return;
    habitatType = newHabitatType;
    emit habitatTypeChanged();
    checkCompleted();
}

int c_habitat::getRooms() const
{
    return rooms;
}

void c_habitat::setRooms(int newRooms)
{
    if (rooms == newRooms)
        return;
    rooms = newRooms;
    emit roomsChanged();
}

int c_habitat::getRawSurface() const
{
    return rawSurface;
}

void c_habitat::setRawSurface(int newRawSurface)
{
    if (rawSurface == newRawSurface)
        return;
    rawSurface = newRawSurface;
    emit rawSurfaceChanged();
}

int c_habitat::getSurface() const
{
    return surface;
}

void c_habitat::setSurface(int newSurface)
{
    if (surface == newSurface)
        return;
    surface = newSurface;
    emit surfaceChanged();
}

int c_habitat::getM2Constructed() const
{
    return m2Constructed;
}

void c_habitat::setM2Constructed(int newM2Constructed)
{
    if (m2Constructed == newM2Constructed)
        return;
    m2Constructed = newM2Constructed;
    emit m2ConstructedChanged();
}

int c_habitat::getM2Available() const
{
    return m2Available;
}

void c_habitat::setM2Available(int newM2Available)
{
    if (m2Available == newM2Available)
        return;
    m2Available = newM2Available;
    emit m2AvailableChanged();
}

int c_habitat::getM3s() const
{
    return m3s;
}

void c_habitat::setM3s(int newM3s)
{
    if (m3s == newM3s)
        return;
    m3s = newM3s;
    emit m3sChanged();
}

void c_habitat::checkCompleted()
{
    setCompleted(habitat_item::is_completed());
}

}
}
