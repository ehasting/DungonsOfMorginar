#include "mapobject.hpp"

namespace DofM
{
MapObject::~MapObject()
{
    fmt::print("Destruct of {}\n", this->Point->ToString());
}
}
