#ifndef MAPOBJECT_H
#define MAPOBJECT_H
#include "coremodels/location.hpp"
#include "tile.hpp"
namespace DofM
{
class MapObject
{
    std::string _MapSymbol = "░";
public:
    ~MapObject();
    typedef std::shared_ptr<MapObject> SMapObject;
    Location::SLocation Point;
    DofM::Tile Tile;
    void SetMapSymbol(std::string symb)
    {
        this->_MapSymbol = symb;
    }
    std::string MapSymbol() const
    {
        return _MapSymbol;
    }
};
}
#endif // MAPOBJECT_H
