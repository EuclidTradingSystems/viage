#ifndef EXTERIOR_HPP
#define EXTERIOR_HPP

#include <wobjectdefs.h>

#include "base_item.hpp"
#include <base_data.hpp>

namespace Data
{
namespace Places
{
class exterior final : virtual public base_item<exterior>
                     , public base_data
{
    W_OBJECT(exterior)

public:
    explicit exterior(QObject* parent = nullptr);

    static consteval auto key() { return "exterior"; }
    static consteval auto qmlName() { return "ExteriorItem"; }
    static consteval auto uri() { return "Places"; }


    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) const override;

    void clear() override;

    enum parkingTypes
    {
        NoParking = 0,
        Bike = 1,
        Car = 2,
        Indoor = 4,
        Outdoor = 8,
        individual = 16,
        Colective = 32
    };

    enum equipements
    {
        None = 0,
        Health = 1,
        Food = 2,
        Education = 4,
        Leasure = 8,
        Shops = 16,
        Transports = 32
    };

    bool getHasParking() const;
    void setHasParking(bool newHasParking);

    int getParkingSpots() const;
    void setParkingSpots(int newParkingSpots);

    parkingTypes getParkingType() const;
    void setParkingType(const parkingTypes& newParkingType);

    int getParkingSurface() const;
    void setParkingSurface(int newParkingSurface);

    const QString getTerrainDescription() const;
    void setTerrainDescription(const QString& newTerrainDescription);

    int getTerrainSurface() const;
    void setTerrainSurface(int newTerrainSurface);

    equipements getEquipements() const;
    void setEquipements(const equipements& newEquipements);

    int getRating() const;
    void setRating(int newRating);

    void hasParkingChanged()
    W_SIGNAL(hasParkingChanged)

    void parkingSpotsChanged()
    W_SIGNAL(parkingSpotsChanged)

    void parkingTypeChanged()
    W_SIGNAL(parkingTypeChanged)

    void parkingSurfaceChanged()
    W_SIGNAL(parkingSurfaceChanged)

    void terrainDescriptionChanged()
    W_SIGNAL(terrainDescriptionChanged)

    void terrainSurfaceChanged()
    W_SIGNAL(terrainSurfaceChanged)

    void equipementsChanged()
    W_SIGNAL(equipementsChanged)

    void ratingChanged()
    W_SIGNAL(ratingChanged)

    W_PROPERTY(bool, hasParking READ getHasParking WRITE setHasParking NOTIFY hasParkingChanged)
    W_PROPERTY(int, parkingSpots READ getParkingSpots WRITE setParkingSpots NOTIFY parkingSpotsChanged)
    W_PROPERTY(parkingTypes, parkingType READ getParkingType WRITE setParkingType NOTIFY parkingTypeChanged)
    W_PROPERTY(int, parkingSurface READ getParkingSurface WRITE setParkingSurface NOTIFY parkingSurfaceChanged)
    W_PROPERTY(QString, terrainDescription READ getTerrainDescription WRITE setTerrainDescription NOTIFY terrainDescriptionChanged)
    W_PROPERTY(int, terrainSurface READ getTerrainSurface WRITE setTerrainSurface NOTIFY terrainSurfaceChanged)
    W_PROPERTY(equipements, equipement READ getEquipements WRITE setEquipements NOTIFY equipementsChanged)
    W_PROPERTY(int, rating READ getRating WRITE setRating NOTIFY ratingChanged)

private:
    bool hasParking;
    int parkingSpots;
    parkingTypes parkingType;
    int parkingSurface;
    QString terrainDescription;
    int terrainSurface;
    equipements equipement;
    int rating;

    void checkCompleted() override;
};

}
}

#endif // EXTERIOR_ITEM_HPP
