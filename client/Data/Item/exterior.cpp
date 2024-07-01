#include <QJsonDocument>
#include <wobjectimpl.h>

#include "exterior.hpp"
#include <client_utils.hpp>

using namespace client_utils;

namespace Data
{
namespace Places
{
exterior::exterior(QObject* parent)
    : base_data{parent}
    , parkingType{NoParking}
    , terrainSurface{50}
    , equipement{None}
{}

void exterior::read(const QJsonObject& json)
{
    if (json.contains("hasParking") && json["hasParking"].isBool())
        setHasParking(json["hasParking"].toBool());

    if (json.contains("parkingSpots") && json["parkingSpots"].isDouble())
        setParkingSpots(json["parkingSpots"].toInt());

    if (json.contains("parkingType") && json["parkingType"].isDouble())
        setParkingType(parkingTypes(json["parkingType"].toInt()));

    if (json.contains("parkingSurface") && json["parkingSurface"].isDouble())
        setParkingSurface(json["parkingSurface"].toInt());

    if (json.contains("terrainDescription") && json["terrainDescription"].isString())
        setTerrainDescription(json["terrainDescription"].toString());

    if (json.contains("terrainSurface") && json["terrainSurface"].isDouble())
        setTerrainSurface(json["terrainSurface"].toInt());

    if (json.contains("rating") && json["rating"].isDouble())
        setRating(json["rating"].toInt());

    if (json.contains("equipements") && json["equipements"].isDouble())
        setEquipements(equipements(json["equipements"].toInt()));

    emit loaded();
    checkCompleted();
}

// void exterior::read(const QByteArray &bytes)
// {
//     const auto json = QJsonDocument::fromJson(bytes).object();
//     read(json);
// }

void exterior::write(QJsonObject& json) const
{
    json["hasParking"] = hasParking;
    if (hasParking)
    {
        json["parkingSpots"] = parkingSpots;
        json["parkingType"] = parkingType;
        json["parkingSurface"] = parkingSurface;
    }
    json["terrainDescription"] = terrainDescription;
    json["terrainSurface"] = terrainSurface;
    json["equipements"] = equipement;
    json["rating"] = rating;
}

void exterior::clear()
{
    hasParking = false;
    emit hasParkingChanged();
    parkingSpots = 0;
    emit parkingSpotsChanged();
    parkingType = NoParking;
    emit parkingTypeChanged();
    parkingSurface = 0;
    emit parkingSurfaceChanged();
    terrainDescription = "";
    emit terrainDescriptionChanged();
    terrainSurface = 50;
    emit terrainSurfaceChanged();
    equipement = None;
    emit equipementsChanged();
    rating = 0;
    emit ratingChanged();
    completed = false;
    emit completedChanged();
}

int exterior::getParkingSpots() const
{
    return parkingSpots;
}

void exterior::setParkingSpots(int newParkingSpots)
{
    if (parkingSpots == newParkingSpots)
        return;
    parkingSpots = newParkingSpots;
    emit parkingSpotsChanged();
}

exterior::parkingTypes exterior::getParkingType() const
{
    return parkingType;
}

void exterior::setParkingType(const parkingTypes& newParkingType)
{
    if (parkingType == newParkingType)
        return;
    parkingType = newParkingType;
    emit parkingTypeChanged();
}

int exterior::getParkingSurface() const
{
    return parkingSurface;
}

void exterior::setParkingSurface(int newParkingSurface)
{
    if (parkingSurface == newParkingSurface)
        return;
    parkingSurface = newParkingSurface;
    emit parkingSurfaceChanged();
}

const QString exterior::getTerrainDescription() const
{
    return terrainDescription;
}

void exterior::setTerrainDescription(const QString& newTerrainDescription)
{
    if (terrainDescription == newTerrainDescription)
        return;
    terrainDescription = newTerrainDescription;
    emit terrainDescriptionChanged();
}

int exterior::getTerrainSurface() const
{
    return terrainSurface;
}

void exterior::setTerrainSurface(int newTerrainSurface)
{
    if (terrainSurface == newTerrainSurface)
        return;
    terrainSurface = newTerrainSurface;
    emit terrainSurfaceChanged();
}

bool exterior::getHasParking() const
{
    return hasParking;
}

void exterior::setHasParking(bool newHasParking)
{
    if (hasParking == newHasParking)
        return;
    hasParking = newHasParking;
    emit hasParkingChanged();
}

exterior::equipements exterior::getEquipements() const
{
    return equipement;
}

void exterior::setEquipements(const equipements& newEquipement)
{
    if (equipement == newEquipement)
        return;
    equipement = newEquipement;
    emit equipementsChanged();
}

int exterior::getRating() const
{
    return rating;
}

void exterior::setRating(int newRating)
{
    if (rating == newRating)
        return;
    rating = newRating;
    emit ratingChanged();
    checkCompleted();
}

void exterior::checkCompleted()
{
    if(rating == 0)
    {
        setCompleted(false);
        return;
    }

    setCompleted(true);
}

}
}

W_OBJECT_IMPL(Data::Places::exterior)
