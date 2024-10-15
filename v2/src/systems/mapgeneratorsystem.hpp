#ifndef MAPGENERATORSYSTEM_H
#define MAPGENERATORSYSTEM_H

#include "coremodels/location.hpp"
#include <vector>
#include "tools.hpp"
#include "map/tile.hpp"
#include "map/mapobject.hpp"

namespace DofM
{
class MapGeneratorSystem
{
private:
    Tools ToolsObject;
    void GenerateRoom(Location::SLocation start, Location::SLocation stop, std::vector<MapObject::SMapObject> &roomvector);
    bool NonDuplicateVectorPush(MapObject::SMapObject mo, std::vector<MapObject::SMapObject> &roomvector);
public:
    MapGeneratorSystem();
    std::vector<MapObject::SMapObject> GenerateMap(std::vector<Location::SLocation> exits);


};
}
#endif // MAPGENERATORSYSTEM_H
