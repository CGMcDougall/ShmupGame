#pragma once
#include "EnemyGameObject.h"
#include "timer.h"
#include <vector>


class FinalBossObject : public EnemyGameObject {

	public:
		
		FinalBossObject(const glm::vec3& position, GLuint tex[], GLint num_elements);

		void Update(double delta_time) override;

		//enemies idle actions/movements 
		virtual void idle(double delta_time);
		
		//enemie's active actions
		virtual void act(double delta_time);

		void setTimer(TimerObject*);
		
		//loads the final boss with the nessesary elements to operate
		void loadFinalBoss(TimerObject*, std::vector<GameObject*>*, EnemyGameObject*);

		//object looses hp equal to input, or 1 if no input is provided
		void loseHP(int i = 1);
		
		bool takeDamage() override;


		//mode 1 of attack, chase player
		void mode1(double delta_time);
		//mode 2 of attack, summon drones
		void mode2(double delta_time);


		//void mode3(double  delta_time);

	private:
		
		bool active;
		int mode;
		//timer to wake up
		TimerObject* wakeTimer;
		
		//store drones to spawn
		std::vector<GameObject*>* list;
		EnemyGameObject* drone;

		//array of textures to simulate "bulding" final boss
		GLuint texture[5];


		//timer for attacks
		TimerObject* timer;
		double chargeTime;
		//how many attacks
		int num_of_attacks;

		//position to attack (where player used to be)
		glm::vec3 pos_to_attack;

		//health
		double hp;

		//determines the amount of attacks
		inline int Rand(int low, int high) { return (rand() % (low + high)); }
		
		//sets texture based on time and timer
		void setTex(double delta_time);
		//resets nessesary values to their defaults
		void resetValues();
};

