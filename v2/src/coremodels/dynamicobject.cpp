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

    DynamicObject::DynamicObject(std::string tname)
    {
        this->PrivateTypeName = tname;
    }

    DynamicObject::~DynamicObject()
    {}
}