#include "miniBossObject.h"
#include <iostream>

miniBossObject::miniBossObject(const glm::vec3& position, GLuint texture, GLint num_elements, std::vector<GameObject*>* l, EnemyGameObject* e)
	: EnemyGameObject(position, texture, num_elements) {

	range = 5;
	miniboss = true;
	timer = 5;

	hp = 3;
	deathNum = 0;
	finalBossShouldTakeDamage = false;

	spawnLocations[0] = glm::vec3(6.0f, 3.0f, 0);
	spawnLocations[1] = glm::vec3(-6.0f, -3.0f, 0);
	spawnLocations[2] = glm::vec3(7.0f, 6.0f, 0);
	spawnLocations[3] = glm::vec3(-6.0f, 9.0f, 0);
	spawnLocations[4] = glm::vec3(11.0f, -1.0f, 0);
	spawnLocations[5] = glm::vec3(-6.0f, -3.0f, 0);
	spawnLocations[6] = glm::vec3(9.0f, 3.0f, 0);
	spawnLocations[7] = glm::vec3(-2.0f, 5.0f, 0);
	spawnLocations[8] = glm::vec3(1.0f, -8.0f, 0);
	
	drone = e;
	list = l;
	//hp = 5;

	speed = 2;
	
}

void miniBossObject::Update(double delta_time) {

	//Angle = 270 + (glm::degrees(glm::atan(velocity_.y, velocity_.x)));

	//std::cout << takeDamage() << std::endl;

	modeShift();
	
	if (isIdle == true)idle(delta_time);
	else act(delta_time);

	GameObject::Update(delta_time);
}


void miniBossObject::idle(double delta_time) {
	velocity_ = glm::vec3(0, 0, 0);
	if (timer <= 0) {
		
		EnemyGameObject* goon = new EnemyGameObject(*drone);
		goon->setAngle(90);
		goon->SetPosition(position_);
		goon->setAlwaysAttack(false);
		list->push_back(goon);
		timer = 3;
	}
}

void miniBossObject::act(double delta_time) {
	
	//Angle = 270 + (glm::degrees(glm::atan(velocity_.y, velocity_.x)));


	if (glm::distance(p1_loc, position_) < 1 * rad) {
		//std::cout << "here1" << std::endl;
		velocity_ = -speed * glm::normalize(p1_loc - position_);
	}

	else if (glm::distance(p1_loc, position_) < range/4 * rad) {
		//std::cout << "here2" << std::endl;
		velocity_ = -speed * glm::normalize(p1_loc - position_);
		Ang -= delta_time * speed * dir;
		velocity_ += rad * glm::normalize(glm::vec3(cos(Ang), sin(Ang), 0.0f));
		
	}
	else {
		//std::cout << "here3" << std::endl;
		velocity_ = glm::vec3(0, 0, 0);
		Ang -= delta_time * speed * dir;
		velocity_ = rad * glm::normalize(glm::vec3(cos(Ang), sin(Ang), 0.0f));
	}


	if (timer <= 0) {

		//std::cout << "working?" << std::endl;

		//create a drone and add it to list
		EnemyGameObject* goon = new EnemyGameObject(*drone);
		goon->setAngle(90);
		goon->SetPosition(position_);
		list->push_back(goon);
		timer = 3;
	}

	else timer -= delta_time;

}

bool miniBossObject::takeDamage() {
	//std::cout << hp << std::endl;
	hp--;
	if (hp <= 0) { // reset and move elsewhere
		if (deathNum >= FINAL_BOSS_HEALTH) {
			GameObject::SetKill();
			return true;
		}
		else {
			hp = 3;
			position_ = spawnLocations[deathNum];
			deathNum++; 
			finalBossShouldTakeDamage = true;
			return false;
		}
	}
	return false;
}
bool miniBossObject::checkIfFinalBossDamage() {
	if (finalBossShouldTakeDamage) {
		finalBossShouldTakeDamage = false;
		return true;
	}
	return false;
}