#include <iostream>

#include "explosion.h"

namespace game {

    
    Explosion::Explosion(const glm::vec3& position, std::vector<GLuint> textures, GLint num_elements) : GameObject(position, textures[0], num_elements)
    {
        this->textures = textures;//list of explosion textures for animation
        time = 0;
        explosion = true; 
    }


    Explosion::~Explosion()
    {
    }

    void Explosion::Update(double delta_time) {
        time += delta_time;

        // Change explosion texture over time, then kill it

        if (time > 0.07) {
            this->texture_ = textures[1];
        }

        if (time > 0.15) {
            this->texture_ = textures[2];
        }

        if (time > 0.75) {
            this->SetKill();
        }
    }



} // namespace game