#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "shader.h"
#include "game_object.h"
#include <vector>

namespace game {
    class Explosion : public GameObject {

    public:
        // Constructor and destructor
        Explosion(const glm::vec3& position, std::vector<GLuint> textures, GLint num_elements);
        ~Explosion();

        void Update(double delta_time) override;

    private:
        double time;
        std::vector<GLuint> textures;

    };


} // namespace game

#endif 
