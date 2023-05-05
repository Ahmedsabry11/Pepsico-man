//
// Created by Lenovo on 5/5/2023.
//

#include "collision.hpp"

#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/player.hpp"
#include "../components/can.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include "../application.hpp"

namespace our {
    void CollisionSystem::update(World *world, float deltaTime) {
        PlayerComponent *player;
        glm::vec3 playerPosition;
        Entity *playerEntity;
        for (auto entity: world->getEntities()) {
            // Get the player component if it exists
            playerEntity = entity;
            player = playerEntity->getComponent<PlayerComponent>();
            // If the player component exists
            if (player) {
                playerPosition =
                        glm::vec3(playerEntity->getLocalToWorldMatrix() *
                                  glm::vec4(playerEntity->localTransform.position, 1.0));
                break;
            }
        }
        if (!player) {
            return;
        }

        glm::vec3 playerStart = playerEntity->getComponent<CollisionComponent>()->start + playerPosition;
        glm::vec3 playerEnd = playerEntity->getComponent<CollisionComponent>()->end + playerPosition;

        // For each entity in the world
        for (auto entity: world->getEntities()) {
            // Get the collision component if it exists
            CollisionComponent *collision = entity->getComponent<CollisionComponent>();
            // If the collision component exists
            if (collision) {
                if (entity->getComponent<CanComponent>()) {
                    auto canPosition = entity->localTransform.position;
                    glm::vec3 canStart = collision->start + canPosition;
                    glm::vec3 canEnd = collision->end + canPosition;
                    bool collided = true;
                    for (int i = 0; i < 3; ++i) {
                        if (playerStart[i] > canEnd[i] || playerEnd[i] < canStart[i]) {
                            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE)) {
                                std::cout << i << std::endl;
                                std::cout << playerStart[i] << " " << playerEnd[i] << std::endl;
                                std::cout << canStart[i] << " " << canEnd[i] << std::endl;
                            }
                            collided = false;
                            break;
                        }
                    }
                    if (collided) {
                        std::cout << "Collided" << std::endl;
                        world->markForRemoval(entity);
                        world->deleteMarkedEntities();
                        break;
                    }
                }
            }
        }
    }
}