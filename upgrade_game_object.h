#ifndef UPGRADE_GAME_OBJECT_H
#define UPGRADE_GAME_OBJECT_H

#include "game_object.h"
#include "player_game_object.h"
#include <iostream>
#include <vector>

using namespace game;

// Abstract upgrade class
class UpgradeGameObject : public GameObject {

	public:

		UpgradeGameObject(const glm::vec3& position, GLuint texture, GLint num_elements);

		virtual void applyEffect() = 0;

};


// Timer upgrade class
class TimerUpgrade : public UpgradeGameObject {

	public:

		TimerUpgrade(const glm::vec3& position, GLuint texture, GLint num_elements, PlayerGameObject* player);

		void applyEffect();

	private: 

		PlayerGameObject* player_;

};


// Nuke upgrade class
class NukeUpgrade : public UpgradeGameObject {

	public:

		NukeUpgrade(const glm::vec3& position, GLuint texture, GLint num_elements, std::vector<GameObject*>& game_objects);

		void applyEffect();

	private:

		std::vector<GameObject*>& game_objects_; // reference to game_objects

};


// Slow upgrade class
class SlowUpgrade : public UpgradeGameObject {

public:

	SlowUpgrade(const glm::vec3& position, GLuint texture, GLint num_elements, std::vector<GameObject*>& game_objects);

	void applyEffect();

private:

	std::vector<GameObject*>& game_objects_; // reference to game_objects

};

#endif