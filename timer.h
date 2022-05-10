#pragma once
#include "game_object.h"

using namespace game;

class TimerObject : public GameObject{

  public:
	//constructor for a timer object, final value is the timers starting time, which has a default value of 5
	TimerObject(const glm::vec3& position, GLuint tex[], GLint num_elements,double t = 5);
	
	//just make a timer, that does not display time, just tracks it
	TimerObject(double t = 5);

	//Update the timer object to move with the player, and count the timer
	void Update(double delta_time) override;

	//renders all number of the time seperatly (with a for loop)
	void Render(Shader& shader) ;

	//takes a double t and adds it to timer. To subtract time, input a negative number
	void changeTime(double t);

	//makes the timer stay in the bottom left corner of the screen
	void act(glm::vec3 p1_pos);

	//returns true if timer is done
	inline bool timerDone() { return (time <= 0); }
	inline double getTime() { return time; }
	
  private:
	  double time;
	  std::string name;
	  bool done;

	  bool shouldDisplay;

	  glm::vec3 original_pos;

	  //store textures for numbers 0-9
	  GLuint texture[10];


};

