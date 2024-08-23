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
            unsigned long Current;
            unsigned long Max;
        public:
            void SetStats(unsigned long max, unsigned long current)
            {
                this->Current = current;
                this->Max = max;
            }
            void RemoveStats(unsigned long num = 1)
            {
                if (Current < num)
                {
                    Current = 0;
                    return;
                }
                if (Current > 0)
                {
                    Current = Current - num;
                }
            }

            void AddStats(unsigned long num = 1)
            {
                if (Current + num > Max)
                {
                    Current = Max;
                    return;
                }
                if (Current < Max)
                {
                    Current = Current + num;
                }
            }
            unsigned long GetCurrent()
            {
                return Current;
            }
            unsigned long GetMax()
            {
                return Max;
            }
        };

        StatNumber Stamina;
        StatNumber Health;
    };
}

#endif //DUNGONSOFMORGINAR_LIVINGSTATS_HPP
