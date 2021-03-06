#include <QJsonDocument>
#include <wobjectimpl.h>

#include "exterior_item.hpp"

namespace Data
{
namespace Places
{
W_OBJECT_IMPL(exterior_item)

exterior_item::exterior_item(QObject *parent)
    : base_item{parent}
{
}

void exterior_item::read(const QJsonObject& json)
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

void exterior_item::read(const QByteArray &bytes)
{
    const auto json = QJsonDocument::fromJson(bytes).object();
    read(json);
}

void exterior_item::write(QJsonObject& json) const
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

void exterior_item::clear()
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

int exterior_item::getParkingSpots() const
{
    return parkingSpots;
}

void exterior_item::setParkingSpots(int newParkingSpots)
{
    if (parkingSpots == newParkingSpots)
        return;
    parkingSpots = newParkingSpots;
    emit parkingSpotsChanged();
}

const exterior_item::parkingTypes &exterior_item::getParkingType() const
{
    return parkingType;
}

void exterior_item::setParkingType(const parkingTypes &newParkingType)
{
    if (parkingType == newParkingType)
        return;
    parkingType = newParkingType;
    emit parkingTypeChanged();
}

int exterior_item::getParkingSurface() const
{
    return parkingSurface;
}

void exterior_item::setParkingSurface(int newParkingSurface)
{
    if (parkingSurface == newParkingSurface)
        return;
    parkingSurface = newParkingSurface;
    emit parkingSurfaceChanged();
}

const QString &exterior_item::getTerrainDescription() const
{
    return terrainDescription;
}

void exterior_item::setTerrainDescription(const QString &newTerrainDescription)
{
    if (terrainDescription == newTerrainDescription)
        return;
    terrainDescription = newTerrainDescription;
    emit terrainDescriptionChanged();
}

int exterior_item::getTerrainSurface() const
{
    return terrainSurface;
}

void exterior_item::setTerrainSurface(int newTerrainSurface)
{
    if (terrainSurface == newTerrainSurface)
        return;
    terrainSurface = newTerrainSurface;
    emit terrainSurfaceChanged();
}

bool exterior_item::getHasParking() const
{
    return hasParking;
}

void exterior_item::setHasParking(bool newHasParking)
{
    if (hasParking == newHasParking)
        return;
    hasParking = newHasParking;
    emit hasParkingChanged();
}

const exterior_item::equipements& exterior_item::getEquipements() const
{
    return equipement;
}

void exterior_item::setEquipements(const equipements& newEquipement)
{
    if (equipement == newEquipement)
        return;
    equipement = newEquipement;
    emit equipementsChanged();
}

int exterior_item::getRating() const
{
    return rating;
}

void exterior_item::setRating(int newRating)
{
    if (rating == newRating)
        return;
    rating = newRating;
    emit ratingChanged();
    checkCompleted();
}

void exterior_item::checkCompleted()
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
