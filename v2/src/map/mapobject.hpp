#ifndef MAPOBJECT_H
#define MAPOBJECT_H
#include "coremodels/location.hpp"
#include "tile.hpp"
namespace DofM
{
class MapObject
{
public:
    typedef std::shared_ptr<MapObject> SMapObject;
    Location::SLocation Point;
    std::string MapSymbol = "░";
    DofM::Tile Tile;
};
}
#endif // MAPOBJECT_H
