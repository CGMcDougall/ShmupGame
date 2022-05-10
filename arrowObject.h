#ifndef ARROW_OBJECT_H
#define ARROW_OBJECT_H

#include "game_object.h"


using namespace game;


class ArrowObject : public GameObject {


	public:
		ArrowObject(const glm::vec3& position, GLuint texture, GLint num_elements, glm::vec3& ppos, glm::vec3& bpos);
		
		void Update(double delta_time) override;

		void setBossLocation(const glm::vec3&);
		void setPlayerLocation(const glm::vec3&);

	protected:
		
		//location of objects
		glm::vec3 boss_loc;
		glm::vec3 pl_loc;

		//distance from player
		glm::vec3 offput;
	
};


#endif

