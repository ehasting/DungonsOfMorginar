//
// Created by eh on 05.01.2022.
//

#ifndef DUNGONSOFMORGINAR_DYNAMICOBJECT_HPP
#define DUNGONSOFMORGINAR_DYNAMICOBJECT_HPP
#include <string>
#include <memory>
#include <iostream>
#include <string_view>
#include "fmt/core.h"
#include <vector>
#include "tools.hpp"
namespace DofM
{
    class DynamicObject
    {
    public:
        typedef std::shared_ptr<std::vector<std::shared_ptr<DynamicObject> > > DynamicObjectList;
        inline static const std::string_view TypeName = "dynamicobject";
        //const std::string_view TypeName = "dynamicobject";
        std::string UniqueName;
        template<class T>
        T *GetRealObject()
        {
            if (T::TypeName != this->GetTypeName())
            {
                throw std::invalid_argument(fmt::format("Trying to cast to wrong type! I am: {}, trying to cast to: {}",
                                                        this->GetTypeName(), T::TypeName));
            }
            return dynamic_cast<T *>(this);
            //return std::dynamic_pointer_cast<T>(std::make_shared<T>(this));
        }

        bool operator==(const DynamicObject& b) const {
            return this->GetTypeName() == b.GetTypeName();
        }

        DynamicObject(std::string uniquename, std::string tname, DynamicObjectList dynobj);
        virtual ~DynamicObject();
        virtual bool Update(long long int tick) = 0;
        virtual const std::string GetDescriptionLine() = 0;
        const std::string GetTypeName() const
        {
            return this->PrivateTypeName;
        }
        DynamicObjectList DynamicObjects;
        bool IsSameObject(DynamicObject &obj)
        {
            return this->UniqueName == obj.UniqueName;
        }
    protected:
        Tools LocalToolsObject;
    private:
        std::string PrivateTypeName;

        DynamicObject();
    };
}

#endif //DUNGONSOFMORGINAR_DYNAMICOBJECT_HPP
