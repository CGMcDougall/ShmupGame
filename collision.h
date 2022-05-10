#ifndef COLLISION_H
#define COLLISION_H

#include "shader.h"
#include "game_object.h"
#include "bullet_game_object.h"
//#include "Bullet.h"
#include <iostream>
#include <vector>

namespace game {

    void checkCollisions(std::vector<GameObject*>& game_objects_, GameObject*& current_game_object);
    //void bulletCollisions(std::vector<GameObject*>& game_objects_, Bullet*& bullet);

    void isColliding(GameObject* curr, GameObject* other);
    void isRayColliding(GameObject* curr, GameObject* other);
    void handleCollision(GameObject* curr, GameObject* other);
    void handleUpgradeCollision(GameObject* upgrade);



    
    /*
    // A class for generating collisions
    class Collision : public GameObject {

    public:
        // Constructor and destructor
        Collision(const glm::vec3& position, GLuint texture, GLint num_elements);
        ~Collision();

        void Update(double delta_time) override;
        double getTime();

        void SetPosition(const glm::vec3& position) override;

        void ResetTime();


    private:
        double time;

    }; */
    

} // namespace game

#endif 
