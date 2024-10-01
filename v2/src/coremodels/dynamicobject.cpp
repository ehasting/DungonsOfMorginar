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
        this->UniqueNameHash = this->UniqueName.size();
        int cnt = 1;
        for (auto &n : this->UniqueName)
        {
            this->UniqueNameHash = this->UniqueNameHash + ((char)n + ((char)n*cnt));
            cnt++;
        }
        this->PrivateTypeName = tname;
        this->DynamicObjects = dynobj;
    }

    DynamicObject::~DynamicObject()
    {}
}
