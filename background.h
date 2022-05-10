#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "shader.h"
#include "game_object.h"
#include <vector>

namespace game {
    class Background : public GameObject {

    public:
        // Constructor and destructor
        Background(const glm::vec3& position, GLuint texture, GLint num_elements);
        ~Background();

        void Render(Shader& shader);


    private:
        

    }; 


} // namespace game

#endif 

