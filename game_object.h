#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include "shader.h"
#include <string>

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of objects in the game world
        The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3 &position, GLuint texture, GLint num_elements);

            // Update the GameObject's state. Can be overriden for children
            virtual void Update(double delta_time);

            // Renders the GameObject using a shader
            void Render(Shader &shader);

            // Getters
            inline glm::vec3& GetPosition(void) { return position_; }
            inline float GetScale(void) { return scale_; }
            inline glm::vec3& GetVelocity(void) { return velocity_; }
            inline float getRadius() { return radius; }
            inline float getAngle() { return Angle; }
            virtual float getSpeed() { return 0.0f; } // overwritten in EnemyGameObject

            // These are only used in bullet_game_object.h/.cpp, however need a reference to it in game_object.h
            virtual inline glm::vec3 getOrigin() { return glm::vec3(0, 0, 0); }
            virtual inline glm::vec3 getAccelDir() { return glm::vec3(0, 0, 0); }
            virtual inline double getSpawnTime() { return 0; }
            virtual inline bool takeDamage() { return false; }

            inline bool isBackground() { return background; };
            inline bool isBullet() { return bullet; };
            inline bool isExplosion() { return explosion; };
            inline bool isEnemy() { return enemy; }
            inline bool isMiniBoss() { return miniboss; }
            inline bool isPlayer() { return player; }
            inline bool isUpgrade() { return upgrade; }
            inline bool isFinalBoss() { return finalboss; }

            inline bool getKill() { return killFlag; }

            // Setters
            virtual void SetKill();

            virtual void setSpeed(float new_speed) {} // overwritten in EnemyGameObject

            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(float scale) { scale_ = scale; }

            inline void SetVelocity(const glm::vec3& velocity) { velocity_ = velocity; }
            inline void setAngle(float ang) { Angle = ang; }

            virtual void applyEffect() {} // overwritten by UpgradeGameObject

        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            float scale_; 
            glm::vec3 velocity_;

            // Object's details
            GLint num_elements_;

            // Object's texture reference
            GLuint texture_;

            //if tileable change
            int num_tiles_ = 1;

            //UPDATE THE FOLLOWING IN CONSTRUCTORS IF NECESSARY

            // Default radius
            float radius = 0.5f;

            //If true, GameObject is deleted on the next update
            bool killFlag = false;


            //defines what type of object it is
            bool background = false;
            bool bullet = false;
            bool explosion = false;
            bool player = false;
            bool enemy = false;
            bool miniboss = false;
            bool finalboss = false;
            bool timer = false;
            bool upgrade = false;

            //Angle that GameObject is pointed at
            float Angle = 0;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
