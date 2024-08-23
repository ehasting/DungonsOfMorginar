//
// Created by eh on 05.01.2022.
//

#include "dynamicobject.hpp"
namespace DofM
{
    DynamicObject::DynamicObject()
    {
        this->PrivateTypeName = TypeName;
    }

    DynamicObject::DynamicObject(std::string uniquename, std::string tname, DynamicObjectList dynobj)
    {
        this->UniqueName = uniquename;
        this->PrivateTypeName = tname;
        this->DynamicObjects = dynobj;
    }

    DynamicObject::~DynamicObject()
    {}
}