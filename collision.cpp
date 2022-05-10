#include <iostream>

#include "collision.h"
#include "game.h"
//#include "bullet.h"
#include <cmath>

namespace game {


    void checkCollisions(std::vector<GameObject*>& game_objects_, GameObject*& current_game_object){
        // Check for collision with other game objects
         
        // Logic behind this: we only want to check collisions that include a PLAYER or an ENEMY (as of now)
        // Why?
        // Because this covers all necessary collisions
        // Ex., for the PLAYER, we are only checking PLAYER-ENEMY or PLAYER-MINIBOSS collisions
        // This can be expanded on, of course

        if (current_game_object->isPlayer() || current_game_object->isEnemy()) {

            for (int j = 0; j < game_objects_.size(); ++j) {

                if (current_game_object->isPlayer()) {//things player should collide with

                    if (game_objects_[j]->isEnemy() || game_objects_[j]->isMiniBoss() || game_objects_[j]->isUpgrade()) {

                        GameObject* other_game_object = game_objects_[j];

                        // Collision w/ either an ENEMY or MINIBOSS is handled by Circle-Circle collision
                        isColliding(current_game_object, other_game_object);
                    }
                }
                else if (current_game_object->isEnemy()) {//things enemies should collide with

                    if (game_objects_[j]->isBullet()) {

                        GameObject* other_game_object = game_objects_[j];

                        // Only need to check for an ENEMY-BULLET collision (PLAYER-ENEMY is already handled above)
                        isRayColliding(other_game_object, current_game_object);
                    }
                }
            }
        }
    }

    // Checks for a Circle-Circle collision
    void isColliding(GameObject* curr, GameObject* other) {
        float distance = glm::length(curr->GetPosition() - other->GetPosition());
        if (distance < curr->getRadius() + other->getRadius()) {// < radius 1 plus radius 2 

            if (other->isUpgrade()) {
                handleUpgradeCollision(other);
                return;
            }

            handleCollision(curr, other);
        }
    }

    // Checks for Ray-Circle collision
    void isRayColliding(GameObject* ray, GameObject* circle) {

        bool collision_detected = false;

        double Px = ray->getOrigin().x;
        double Py = ray->getOrigin().y;
        double dx = BULLET_SPEED * ray->getAccelDir().x; // could also do getVelocity perhaps
        double dy = BULLET_SPEED * ray->getAccelDir().y;
        double h = circle->GetPosition().x;
        double k = circle->GetPosition().y;
        double r = circle->getRadius();

        // Obtained these values by doing a bit of math :-)

        double a = pow(dx, 2) + pow(dy, 2);

        double b = (2.0 * Px * dx) - (2.0 * dx * h) + (2.0 * Py * dy) - (2.0 * dy * k);

        double c = pow(Px, 2) - (2.0 * Px * h) + pow(h, 2) + pow(Py, 2) - (2.0 * Py * k) + pow(k, 2) - pow(r, 2);

        // After finding a, b, and c, we will calculate the discriminant which will tell us if there is/will be/was a collision

        double discriminant = pow(b, 2) - 4 * a * c;

        // We will count the unlikely case where there is only one intersection as a collision (hence we are including the case when the discriminant = 0)
        if (discriminant >= 0) {

            // If there was an intersection, we will calculate when it happened/will happen by applying the quadratic formula
            double t1 = (-b + pow(discriminant, 0.5)) / (2.0 * a);
            double t2 = (-b - pow(discriminant, 0.5)) / (2.0 * a);

            t1 = -t1;
            t2 = -t2;

            // If our active time is anywhere btwn t1 and t2, we have a collision
            double active_time = glfwGetTime() - ray->getSpawnTime();

            // Check which of the two is the biggest before checking for collision
            if (t1 >= t2) {

                if (active_time >= t2 && active_time <= t1) {
                    // COLLISION
                    collision_detected = true;
                }

            }
            else {

                if (active_time <= t2 && active_time >= t1) {
                    // COLLISION
                    collision_detected = true;
                }
            }
        }

        if (collision_detected) {
            handleCollision(ray, circle);
        }
    }


    // In the event of a collision, sets both objects to be killed, unless there are special conditions
    void handleCollision(GameObject* curr, GameObject* other) {
        if (curr->isPlayer()) {
            if (curr->takeDamage()) {
                curr->SetKill(); //disable for testing
            }
            if(!other->isFinalBoss() && !other->isMiniBoss())other->SetKill();
            return;
        }
        else if (curr->isFinalBoss())curr->takeDamage();
        else if (curr->isMiniBoss())curr->takeDamage();
        else curr->SetKill();

        if (other->isFinalBoss())other->takeDamage();
        else if (other->isMiniBoss())other->takeDamage();
        else other->SetKill();
    }
    

    void handleUpgradeCollision(GameObject* upgrade) {

        // apply effect, set upgrade to be deleted

        upgrade->applyEffect();
        upgrade->SetKill();

    }

} // namespace game
