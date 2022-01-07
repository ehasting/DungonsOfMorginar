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
namespace DofM
{
    class DynamicObject
    {
    public:
        inline static const std::string_view TypeName = "dynamicobject";
        //const std::string_view TypeName = "dynamicobject";

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

        DynamicObject(std::string tname);

        virtual ~DynamicObject();

        virtual bool Update(long tick) = 0;

        std::string_view GetTypeName() const
        {
            return this->PrivateTypeName;
        }

    private:
        std::string PrivateTypeName;

        DynamicObject();
    };
}

#endif //DUNGONSOFMORGINAR_DYNAMICOBJECT_HPP
