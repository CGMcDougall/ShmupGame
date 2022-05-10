#include <glm/gtc/matrix_transform.hpp>

#include "game_object.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, GLuint texture, GLint num_elements) 
{

    // Initialize all attributes
    position_ = position;
    scale_ = 1.0;
    velocity_ = glm::vec3(0.0f, 0.0f, 0.0f); // Starts out stationary
    num_elements_ = num_elements;
    texture_ = texture;
    //type = "default";
}


void GameObject::Update(double delta_time) {

    // Update object position with Euler integration
    position_ += velocity_ * ((float) delta_time);
}

void GameObject::SetKill() {
    killFlag = true;
}


void GameObject::Render(Shader &shader) {

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    //use for rotation 
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    // TODO: Add other types of transformations
    
    // Set the transformation matrix in the shader
    shader.SetUniformMat4("transformation_matrix", transformation_matrix);
    shader.SetUniform1i("num_tiles", num_tiles_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);
}

} // namespace game
