//
// Created by eh on 1/5/22.
//

#ifndef DUNGONSOFMORGINAR_MOUSE_HPP
#define DUNGONSOFMORGINAR_MOUSE_HPP
#include "coremodels/dynamicobject.hpp"
namespace DofM
{
    class Mouse : public DynamicObject
    {
    public:
        inline static const std::string TypeName = "mouse";

        Mouse() : DynamicObject(TypeName)
        {
        }

        virtual bool Update(long tick)
        {
            //std::cout << "Update from " << this->GetTypeName() << std::endl;
            return true;
        };

        std::string Moustrubate()
        {
            return "I am MousTrubating";
        }
    };
}

#endif //DUNGONSOFMORGINAR_MOUSE_HPP
