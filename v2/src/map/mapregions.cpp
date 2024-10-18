//
// Created by eh on 07.01.2022.
//

#include "mapregions.hpp"
//
// Created by eh on 07.01.2022.
//

namespace DofM
{

MapRegions::MapRegions(std::string name, std::vector<Location::SLocation> exits)
{
    int max_retry = 100;
    this->UniqueName = name;
    this->TileList = this->MapGenerator.GenerateMap(exits);

}

void MapRegions::AddWallsToRoom()
{


}

bool MapRegions::CanMove(Location::SLocation const& TargetLocation)
{
    // we are inside the walls, lets check if we are crashing with objects.
    for (auto &n : this->TileList)
    {
        if (n->Point->IsSame(TargetLocation))
        {
            return true;
        }
    }
    return false;
}

int MapRegions::GetMoveCost(Location::SLocation const& TargetLocation)
{
    return 1; //MainTile.GetTileCost();
}


unsigned long MapRegions::Width()
{
    return 0;
}

unsigned long MapRegions::Height()
{
    return 0;
}



Location::SLocation MapRegions::GetCenter()
{
    unsigned long X = 0;
    unsigned long Y = 0;
    unsigned long Z = 0;
    return std::make_shared<Location>(X,Y,Z);
}

Location::SLocation MapRegions::GetRandomLocationInsideRoom()
{
    auto rval = this->TileList.at(this->ToolsObject.RndRange(0, this->TileList.size()-1))->Point;
    return rval;
}

void MapRegions::DrawMap(std::shared_ptr<NonBlockingTerminal> term, ScreenPos &drawstart)
{
    term->WriteToBuffer(fmt::format("Room: {} (room objects: {})", this->UniqueName, this->TileList.size()), ScreenPos(0,0).AddOffset(drawstart), {255,0,64,255});
    drawstart.IncreaseRow();
    term->WriteToBuffer(fmt::format("Description: {}", "Nothing right now"), ScreenPos(0,0).AddOffset(drawstart), {128,200,64,255});
    drawstart.IncreaseRow();
    drawstart.IncreaseRow();

    for (auto &n : this->TileList)
    {
        if (n->Tile.TileType == Tile::TileTypes::EMPTY)
        {
            term->WriteToBuffer("E", n->Point->ReturnAsScreenPos().AddOffset(drawstart));
        }
        else
        {

            term->WriteToBuffer(n->MapSymbol, n->Point->ReturnAsScreenPos().AddOffset(drawstart));
        }
    }
}
}

