#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "timer.h"

#define SINGLE_DELAY 0.1875
#define SHOTGUN_DELAY 0.5625
#define SWAP_DELAY 0.5

namespace game {

    enum fire_modes {SINGLE, SHOTGUN};

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, GLuint texture, GLint num_elements);

            // Update function for moving the player object around
            void Update(double delta_time) override;
            bool takeDamage() override;

            int getFireMode() { return fire_mode_; }
            TimerObject* getTimer() { return timer; }

            //changes shooting from single to shotgun or vice-versa
            void swapFireMode();
            
            //changes what angle to rotate the player based on input value. > 0 increases the angle, < 0 decreases and =0 stops rotation.
            void appendAngle(float);

            //sets float dir to be positive or negative based on if the input is positive or negative.
            void setAccel(float);

            //assings a timer pointer to the class
            void setTimer(TimerObject* t);
        
        protected:
            
            //variables required for acceleration
            glm::vec3 accel_;
            const float accel;
            float dir;
            const float maxAccel;

            //timer object
            TimerObject* timer;
           
            //buffer on kill of player when timer runs out
            bool playerKillBuffer = true;

            //controls min and max speeds
            const float minSpeed;
            const float maxSpeed;
    
        private:
            
            float angleGain;
            void velocityMaintenence(double);
            void accelMaintenece();
            int fire_mode_;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
