#include "bullet_game_object.h"
#include <GLFW/glfw3.h>
#include <iostream>

namespace game {

	BulletGameObject::BulletGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, float angle, int source)
		: GameObject(position, texture, num_elements) {

		bullet = true;
		Angle = angle;
		origin_ = position;
		spawn_time_ = glfwGetTime();
		source_ = source;

		// Derive an acceleration from the Angle property
		glm::mat4 bearing_matrix = glm::rotate(glm::mat4(1.0f), Angle, glm::vec3(0, 0, 1));
		glm::vec4 new_acceleration = glm::normalize(bearing_matrix * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)); // Get new acceleration (direction) vector

		accel_dir_ = glm::vec3(new_acceleration.x, new_acceleration.y, 0);
	}

	void BulletGameObject::Update(double delta_time) {


		velocity_ = (float)BULLET_SPEED * -accel_dir_;

		position_ += velocity_ * (float)delta_time; // Increase bullet's position

		// Check if the bullet has exceeded its lifespan
		if (source_ == SINGLE) {
			if (glfwGetTime() - spawn_time_ > SINGLE_LIFE) {
				SetKill();
			}
		}
		else if (source_ == SHOTGUN) {
			if (glfwGetTime() - spawn_time_ > SHOTGUN_LIFE) {
				SetKill();
			}
		}
	}
}