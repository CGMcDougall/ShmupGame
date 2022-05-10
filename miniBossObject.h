#pragma once
#include "EnemyGameObject.h"
#include <vector>

using namespace game;

class miniBossObject : public EnemyGameObject {

	public:
		//Takes in addition to standard requirements, a pointer to the game_objects list and enemyGameObject type
		miniBossObject(const glm::vec3& position, GLuint texture, GLint num_elements, std::vector<GameObject*>*, EnemyGameObject*);

		//idle actions for object
		void idle(double delta_time) override;
		//reduce hp by one, if at 0 kill object
		bool takeDamage() override;


		bool checkIfFinalBossDamage();

		//summons a drone to attack the player every x seconds, were x is the value of timer
		void act(double delta_time) override;

		void Update(double delta_time)override;

		//bool takeDamage() override;

	protected:
		//true/false if the final boss should take damage because of objects death
		bool finalBossShouldTakeDamage;
		float timer;
		int hp;
		int deathNum;
#define FINAL_BOSS_HEALTH 9

		glm::vec3 spawnLocations[FINAL_BOSS_HEALTH];

		std::vector<GameObject*> *list;
		EnemyGameObject* drone;
		
};

