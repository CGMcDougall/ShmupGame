#include "FinalBossObject.h"
#include <iostream>


FinalBossObject::FinalBossObject(const glm::vec3& position, GLuint tex[], GLint num_elements)
	: EnemyGameObject(glm::vec3(0, 0, 0), tex[0], num_elements) {

	active = false;
	mode = 0;
	wakeTimer = NULL;
	timer = NULL;
	speed = 5;
	GameObject::finalboss = true;
	timer = new TimerObject(0);
	chargeTime = 1;
	num_of_attacks = 0;
	pos_to_attack = position;
	
	//radius = 1;

	//some random number for now
	hp = 10;

	for (int i = 0; i < 5; i++) {
		texture[i] = tex[i];
	}

}

void FinalBossObject::setTimer(TimerObject* t) {
	wakeTimer = t;
}

void FinalBossObject::Update(double delta_time) {
	
	setTex(delta_time);

	if (active == true) {
		
		if (mode == 0)idle(delta_time);
		if (mode == 1)mode1(delta_time);
		if (mode == 2)mode2(delta_time);
	}

	if (wakeTimer == NULL || wakeTimer->timerDone())active = true;

	//if time != 0, then run the timer
	if(!timer->timerDone())timer->changeTime(-delta_time);

	GameObject::Update(delta_time);
	
}



void FinalBossObject::act(double delta_time) {

	if (mode == 0)mode = Rand(1,2);

	if (mode == 1) {
		num_of_attacks = Rand(2, 4);
		timer->changeTime(chargeTime);
	}
	if (mode == 2) {
		num_of_attacks = Rand(3, 5);
		timer->changeTime(chargeTime / 3);
	}

}

void FinalBossObject::idle(double delta_time) {
	if (timer->timerDone())act(delta_time);

	EnemyGameObject::idle(delta_time);

}

void FinalBossObject::mode1(double delta_time) {
	
	if (timer->timerDone()) {
		velocity_ = speed*pos_to_attack;
		timer->changeTime(chargeTime);
		num_of_attacks--;
		if (num_of_attacks <= 0)resetValues();
	}
	else {
	pos_to_attack = glm::normalize(p1_loc - position_);
	//std::cout << pos_to_attack.x <<"," <<pos_to_attack.y<< std::endl;
	}
}

void FinalBossObject::mode2(double delta_time) {
	velocity_ = glm::vec3(0, 0, 0);
	texture_ = texture[4];
	if (timer->timerDone()) {
		EnemyGameObject* goon = new EnemyGameObject(*drone);
		goon->setAngle(90);
		goon->SetPosition(position_);
		goon->setSpeed(2);
		list->push_back(goon);
		
		timer->changeTime(chargeTime);
		num_of_attacks--;
		if (num_of_attacks <= 0)resetValues();
	}
}


void FinalBossObject::resetValues() {
	timer->changeTime(chargeTime);
	num_of_attacks = 0;
	velocity_ = glm::vec3(0, 0, 0);
	mode = 0;
}

void FinalBossObject::loadFinalBoss(TimerObject* t, std::vector<GameObject*>* l, EnemyGameObject* e) {
	wakeTimer = t;
	list = l;
	drone = e;
}

void FinalBossObject::setTex(double delta_time) {

	if (wakeTimer->getTime() > 200)texture_ = texture[0];
	else if(wakeTimer->getTime() > 100)texture_ = texture[1];
	else if (wakeTimer->getTime() > 0)texture_ = texture[2];
	else if (wakeTimer->timerDone())texture_ = texture[3];
}

void FinalBossObject::loseHP(int i) {
	hp -= i;
	if (hp <= 0)GameObject::SetKill();
	std::cout << hp << std::endl;
}

bool FinalBossObject::takeDamage() {
	
	if(active == true)hp--;
	if (hp <= 0) {
		GameObject::SetKill();
		std::cout << "You win" << std::endl;
		return true;
	}
	std::cout << hp << std::endl;
}