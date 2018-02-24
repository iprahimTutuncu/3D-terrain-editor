#ifndef SYSTEMBASE_H
#define SYSTEMBASE_H

#include <vector>
#include <SFML/System/Time.hpp>

#include "../Entity.h"

namespace ECS{
    class SystemBase
    {
        public:
            SystemBase() = default;

            virtual void init(std::vector<std::shared_ptr<Entity>> entities) = 0;
            virtual void update(const sf::Time &deltaTime) = 0;

        protected:
            std::vector<std::shared_ptr<Entity>> entities;
            unsigned int mask;

    };
}
#endif // SYSTEMBASE_H
