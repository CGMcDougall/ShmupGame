#include "upgrade_game_object.h"

using namespace game;

// UpgradeGameObject
UpgradeGameObject::UpgradeGameObject(const glm::vec3& position, GLuint texture, GLint num_elements)
	: GameObject(position, texture, num_elements) {
	upgrade = true;
}


// TimerUpgrade (add to player timer)
TimerUpgrade::TimerUpgrade(const glm::vec3& position, GLuint texture, GLint num_elements, PlayerGameObject* player) 
	: UpgradeGameObject(position, texture, num_elements) {
	player_ = player;
}


void TimerUpgrade::applyEffect() {
	player_->getTimer()->changeTime(+30);
}


// NukeUpgrade (nuke all enemies in the game) (does not include minibosses and boss)
NukeUpgrade::NukeUpgrade(const glm::vec3& position, GLuint texture, GLint num_elements, std::vector<GameObject*>& game_objects)
	: UpgradeGameObject(position, texture, num_elements), game_objects_(game_objects) {
	//game_objects_ = game_objects;
}

void NukeUpgrade::applyEffect() {

	for (int i = 0; i < game_objects_.size(); ++i) {
		if (game_objects_[i]->isEnemy() && !game_objects_[i]->isMiniBoss() && !game_objects_[i]->isFinalBoss()) { // a little bit of jankiness here since the miniBoss inherits from the enemy,
			game_objects_[i]->SetKill();																		  // which means the miniBoss's "enemy" boolean is set to true
		}
	}
}


// SlowUpgrade (reduce speed of all enemies) (does not apply to minibosses and boss)
SlowUpgrade::SlowUpgrade(const glm::vec3& position, GLuint texture, GLint num_elements, std::vector<GameObject*>& game_objects)
	: UpgradeGameObject(position, texture, num_elements), game_objects_(game_objects) {
	//game_objects_ = game_objects;
}

void SlowUpgrade::applyEffect() {

	for (int i = 0; i < game_objects_.size(); ++i) {
		if (game_objects_[i]->isEnemy() && !game_objects_[i]->isMiniBoss() && !game_objects_[i]->isFinalBoss()) { // a little bit of jankiness here since the miniBoss inherits from the enemy,
			game_objects_[i]->setSpeed(game_objects_[i]->getSpeed() / 2);										  // which means the miniBoss's "enemy" boolean is set to true
		}
	}
}