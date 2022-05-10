#include "EnemyGameObject.h"

EnemyGameObject::EnemyGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool justAttc)
	: GameObject(position, texture, num_elements) {
	
	//type must be declared as enemy
	enemy = true;

	init_pos = position;

	isIdle = true;
	justAttack = justAttc;
	Ang = 180;
	rad = 3;
	speed = 3;
	dir = -1;
	range = 1;

}

void EnemyGameObject::Update(double delta_time) {

	//to turn on player attacking, uncomment modeshift()

	modeShift();

	if (isIdle == true && justAttack == false)idle(delta_time);
	else act(delta_time);


	GameObject::Update(delta_time);
}

void EnemyGameObject::idle(double delta_time) {

	Ang += delta_time * speed * dir;
	velocity_ = rad * glm::normalize(glm::vec3(cos(Ang), sin(Ang), 0.0f));

}

void EnemyGameObject::act(double delta_time) {

	//setAngle(glm::dot(velocity_,p1_loc));
	velocity_ = speed * glm::normalize(p1_loc - position_);
	Angle = (glm::degrees(glm::atan(velocity_.y,velocity_.x)));
}

void EnemyGameObject::modeShift() {
	if (glm::distance(p1_loc, position_) < range * rad)isIdle = false;
	else {
		isIdle = true;
		init_pos = position_;
	}
}

void EnemyGameObject::setPlayerLocation(const glm::vec3& p1) {
	p1_loc = p1;
}

