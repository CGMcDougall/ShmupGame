#include "timer.h"
#include <iostream>

TimerObject::TimerObject(const glm::vec3& position, GLuint tex[], GLint num_elements, double t)
	: GameObject(position,tex[1],num_elements){

	//GameObject::backgound = true;
	GameObject::timer = true;
	
	for (int i = 0; i < 10; i++) {
		texture[i] = tex[i];
	}

	time = t;
	done = false;
	name = "timer";
	shouldDisplay = true;
	original_pos = position;
}

TimerObject::TimerObject(double t)
	: GameObject(glm::vec3(0, 0, 0), NULL, NULL) {
	time = t;
	done = false;
	shouldDisplay = false;
	original_pos = glm::vec3(0, 0, 0);
}

void TimerObject::Update(double delta_time) {

	time -= delta_time;
	position_ = original_pos;
	
	//temp way to track time
	//std::cout <<name<<" : "<< time << std::endl;

	if (time <= 0)done = true;
	//if (time <= -2)killFlag = true;
}

void TimerObject::changeTime(double t) {
	time += t;
}

void TimerObject::Render(Shader& shader) {
	
	if (!shouldDisplay)return;

	int t = time;
	int div = 100;
	int num = 0;
	
	int display[3];

	//find the amount of numbers that need to be displayed
	while (true) {
		num++;
		if (t % div == 0) {
			display[num - 1] = (int)t / div;
			break;
		}
		display[num - 1] = (int)t / div;
		t = t % div;
		div = div / 10;
	}

	//display (render) three numbers by swapping texture_ value and rendering them 
	for (int i = 0; i < 3; i++) {
		
		if (display[i] > 9 || display[i] < 0)display[i] = 0;

		GameObject::texture_ = texture[display[i]];
		position_.x = original_pos.x + ((float)i)/2;
		GameObject::Render(shader);
	}

}

void TimerObject::act(glm::vec3 p1_pos) {
		
	//float ang = glm::radians(p1.getAngle());
	float ang = getAngle();
	glm::vec3 e = glm::vec3(-3.5f, -3.5f , 0);
	original_pos = p1_pos + e;

}