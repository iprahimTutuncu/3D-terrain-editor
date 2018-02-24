#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <memory>
#include <math.h>

#include "Components/Component.h"

namespace ECS{

    class Entity
    {
        public:
            Entity()
            :mask(0){};

            template<typename T>
            void setComponent(COMPONENT target, T& comp){
                components[static_cast<unsigned int>(target)] = static_cast<Component*>(&comp);
                if(!hasComponent(target))
                    mask += 1 << static_cast<unsigned int>(target);
            }

            template<typename T>
            void setComponent(COMPONENT target, T&& comp){
                T* temp = new T(std::move(comp));
                components[static_cast<unsigned int>(target)] = static_cast<Component*>(temp);
                if(!hasComponent(target))
                    mask += 1 << static_cast<unsigned int>(target);
            }

            template<typename T>
            T* getComponent(COMPONENT target){
                return static_cast<T*>(components[static_cast<unsigned int>(target)]);
            }

            bool hasComponent(COMPONENT target){
                unsigned int value = 1 << static_cast<unsigned int>(target);

                return (value & mask) != 0 ? true : false;
            }

        private:
            Component* components[sizeof(unsigned int)];
            unsigned int mask;
    };

}

#endif // ENTITY_H


