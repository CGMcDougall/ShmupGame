#include <iostream>

#include "background.h"

namespace game {

    Background::Background(const glm::vec3& position, GLuint texture, GLint num_elements) : GameObject(position, texture, num_elements)
    {
        background = true;
        num_tiles_ = 1000;
    }


    Background::~Background()
    {
    }





} // namespace game