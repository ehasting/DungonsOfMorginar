//
// Created by eh on 1/9/22.
//

#ifndef DUNGONSOFMORGINAR_LIVINGSTATS_HPP
#define DUNGONSOFMORGINAR_LIVINGSTATS_HPP

namespace DofM
{
    class LivingStats
    {
    public:
        class StatNumber
        {
        public:
            long Current;
            long Max;
        };

        StatNumber Stamina;
        StatNumber Health;
    };
}

#endif //DUNGONSOFMORGINAR_LIVINGSTATS_HPP
