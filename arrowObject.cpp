#include "arrowObject.h"

ArrowObject::ArrowObject(const glm::vec3& position, GLuint texture, GLint num_elements, glm::vec3& ppos, glm::vec3& bpos)
	: GameObject(position, texture, num_elements) {

	offput = position;
	boss_loc = bpos;
	pl_loc = ppos;

}

void ArrowObject::Update(double delta_time) {

	//position should be relative to player, 3.5 -3.5 0
	position_ = pl_loc + offput;

	//angle should be towards known boss position
	glm::vec3 dir = glm::normalize(boss_loc - pl_loc);

	Angle = glm::degrees(glm::atan(dir.y, dir.x));



	GameObject::Update(delta_time);
}


void ArrowObject::setBossLocation(const glm::vec3& b) {
	boss_loc = b;
}

void ArrowObject::setPlayerLocation(const glm::vec3& pl) {
	pl_loc = pl;
}

