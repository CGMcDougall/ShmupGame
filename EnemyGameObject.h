#ifndef ENEMY_GAME_OBJ_H
#define ENEMY_GAME_OBJ_H

#include "game_object.h"


using namespace game;


class EnemyGameObject : public GameObject {


	public:
		EnemyGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool justAttc = false);
		
		void Update(double delta_time) override;

		// get and set enemy speed
		float getSpeed() { return speed; }
		void setSpeed(float new_speed) { speed = new_speed; }


		//enemies idle actions/movements 
		virtual void idle(double delta_time);

		//enemie's active actions
		virtual void act(double delta_time);

		//void trackPos(glm::vec3& position);

		//changes isIdle based on coditions
		void modeShift(void);

		void setPlayerLocation(const glm::vec3&);

		inline void setAlwaysAttack(bool b) { justAttack = b; }
		

	private:
		bool justAttack;

	protected:
		
		//location of player object
		glm::vec3 p1_loc;

		//initial location of objectssss
		glm::vec3 init_pos;
		
		bool isIdle;

		//if this bool is true, dont ever be idle, just attack the player
		

		float Ang;
		float rad;
		float range;
		float speed;
		float dir;

	
};


#endif

