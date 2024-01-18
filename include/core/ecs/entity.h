#ifndef ENTITY_H
#define ENTITY_H

#include <stdexcept>
#include <windows.h>
#include <cstdint>
#include <queue>
#include <bitset>
#include <array>

const int MAX_COMPONENTS = 32;
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 10000;
using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
    EntityManager() {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
            available_entities.push(entity);
        }
    }

    Entity create_entity() {
        if (living_entity_count >= MAX_ENTITIES)
            throw std::runtime_error("Too many entities in existence.");

        Entity id = available_entities.front();
        available_entities.pop();
        ++living_entity_count;

        return id;
    }

    void destroy_entity(Entity entity) {
        if (entity >= MAX_ENTITIES)
            throw std::runtime_error("Entity out of range.");

        signatures[entity].reset(); // Clear the signature
        available_entities.push(entity); // Recycle the entity ID
        --living_entity_count;
    }

    void set_signature(Entity entity, Signature signature) {
        if (entity >= MAX_ENTITIES)
            throw std::runtime_error("Entity out of range.");

        signatures[entity] = signature;
    }

    Signature get_signature(Entity entity) {
        if (entity >= MAX_ENTITIES)
            throw std::runtime_error("Entity out of range.");

        return signatures[entity];

    }
private:
    std::queue<Entity> available_entities; // Pool of reusable entity IDs
    std::array<Signature, MAX_ENTITIES> signatures; // Array of signatures
    uint32_t living_entity_count = 0; // Total number of active entities
};


#endif
