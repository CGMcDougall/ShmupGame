#include "player_game_object.h"
#include <iostream>

namespace game {

	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	PlayerGameObject::PlayerGameObject(const glm::vec3& position, GLuint texture, GLint num_elements)
		: GameObject(position, texture, num_elements), maxSpeed(3.5), minSpeed(1.5), accel(0.01) , maxAccel(3) {
		accel_ = glm::vec3(0.0f, 0.0f, 0.0f);
		angleGain = 0.1;
		player = true;
		fire_mode_ = fire_modes::SINGLE;
		timer = NULL;
	}

	// Update function for moving the player object around
	void PlayerGameObject::Update(double delta_time) {

		int friction = 300;
		velocity_ -= glm::vec3(velocity_.x / friction * delta_time, velocity_.y / friction * delta_time, 0);

		// Special player updates go here
		setAngle(getAngle() + (angleGain * (float)delta_time));
		velocityMaintenence(delta_time);
		position_ += velocity_ * (float)delta_time;
		
		// Call the parent's update method to move the object in standard way, if desired
		//GameObject::Update(delta_time);
	}

	//sets float dir to be positive or negative based on if the input is positive or negative.
	void PlayerGameObject::setAccel(float x) {
		
		

		if (x > 0)dir = 1;
		else if (x == 0)dir = 0;
		else dir = -1;

		//std::cout << x << " --- " << dir << std::endl;

		//if no acceleration is being added, acceleration is 0
		if (x == 0)accel_ = glm::vec3(0, 0, 0);
		
		else {
			
			float ang = glm::radians(Angle);
			
			accel_ += (dir * accel) * glm::normalize(glm::vec3(-glm::sin(ang), glm::cos(ang), 0));
			//std::cout << accel_.x << std::endl;

		}
			
	}

	//makes sure velocity doesnt exceed min/max, and modifies velocity according to acceleration 
	void PlayerGameObject::velocityMaintenence(double delta_time) {

		accelMaintenece();
		float factor = 2;

		velocity_ += accel_ * (float)delta_time;
		//std::cout << velocity_.x << " - " << velocity_.y << std::endl;

		//Max speed check
		if (glm::length(velocity_) > maxSpeed) {
			velocity_ = maxSpeed * glm::normalize(velocity_);
		}
		//Min speed check
		if (glm::length(velocity_) < 0.5 && glm::length(accel_) != 0) {
			
			velocity_ = minSpeed * glm::normalize(velocity_);
		}

	

		//auto stop feature
		if (glm::length(velocity_) < 0.05f) {
			velocity_ = glm::vec3(0, 0, 0);
		}
		
	}

	//maintains max accel, and other acceleration details
	void PlayerGameObject::accelMaintenece() {

		//max accel
		if (glm::length(accel_) > maxAccel) {
			accel_ = maxAccel * glm::normalize(accel_);
		}

	//	std::cout << glm::length(accel_) << std::endl;

		//makes movement better for x
		//if (velocity_.x != 0 && accel_.x / velocity_.x < 0) {
		//	accel_.x *= 1.02;
		//}
		//makes movement better for y
		//if (velocity_.y != 0 && accel_.y / velocity_.y < 0) {
		//	accel_.y *= 1.02;
		//}

		if (glm::length(accel_) == 0 && glm::length(velocity_) == 0) {
			accel_ = glm::vec3(0, 0, 0);
			
		}
		//auto slow function
		if (glm::length(accel_) == 0) {
			//std::cout << "working" << std::endl;
			velocity_ += -0.005f * velocity_;
		}
	}

	
	void PlayerGameObject::appendAngle(float dir) {
		float ang = 170;
		if (dir > 0)angleGain = ang;
		else if (dir < 0) angleGain = -ang;
		else angleGain = 0;
	}

	void PlayerGameObject::swapFireMode() {
		if (fire_mode_ == fire_modes::SINGLE) {
			fire_mode_ = fire_modes::SHOTGUN;
			return;
		}

		if (fire_mode_ == fire_modes::SHOTGUN) {
			fire_mode_ = fire_modes::SINGLE;
			return;
		}
	}



	void PlayerGameObject::setTimer(TimerObject* t){
		timer = t;
	}

	bool PlayerGameObject::takeDamage() {
		if (timer != NULL)timer->changeTime(-100);
		if (playerKillBuffer && timer->timerDone()) {
			playerKillBuffer = false;
			return false;
		}
		else if (!playerKillBuffer && timer->timerDone()){
			return true;
		}
		else {
			return false;
		}
	}



} // namespace game
