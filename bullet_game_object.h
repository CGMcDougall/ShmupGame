#ifndef BULLET_GAME_OBJECT_H_
#define BULLET_GAME_OBJECT_H_

#include "game_object.h"
#include "player_game_object.h"
#include <glm/gtc/matrix_transform.hpp>

// Bullet related constants
const double BULLET_SPEED = 10.0;
const double SINGLE_LIFE = 3;
const double SHOTGUN_LIFE = 0.5;

namespace game {

	class BulletGameObject : public GameObject {

	public:

		BulletGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, float angle, int source);

		double getSpawnTime(void) { return spawn_time_; }
		glm::vec3 getOrigin(void) { return origin_; }
		glm::vec3 getAccelDir(void) { return accel_dir_; }

		void Update(double delta_time);


	protected:

		glm::vec3 origin_;
		double spawn_time_;
		glm::vec3 accel_dir_;
		int source_;
	};


}



#endif