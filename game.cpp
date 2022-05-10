#include <stdexcept>
#include <string>
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>

#include <path_config.h>
#include <iostream>

#include "explosion.h"
#include "collision.h"
#include "shader.h"
#include "player_game_object.h"
#include "EnemyGameObject.h"
#include "game.h"
#include "miniBossObject.h"
#include "bullet_game_object.h"
#include "upgrade_game_object.h"


#include "timer.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;
TimerObject* Game::timer = NULL;
FinalBossObject* Game::Boss = NULL;
//miniBossObject* Game::miniBoss = NULL;
//ArrowObject* Game::arrow = NULL;

Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set window to not resizable
    // Required or else the calculation to get cursor pos to screenspace will be incorrect
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set up square geometry
    size_ = CreateSprite();

    // Initialize shader
    shader_.Init((resources_directory_g+std::string("/vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/fragment_shader.glsl")).c_str());
    shader_.Enable();

    // Set up z-buffer for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Game::~Game()
{

    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::Setup(void)
{

    // Load textures
    SetAllTextures();

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    player = new PlayerGameObject(glm::vec3(0.0f, -2.0f, 0.0f), tex_[0], size_);
    game_objects_.push_back(player);

    // TEST UPGRADE
    //game_objects_.push_back(new TimerUpgrade(glm::vec3(0.0f, -5.0f, 0.0f), tex_[9], size_, (PlayerGameObject*)game_objects_[0]));
    //game_objects_.push_back(new NukeUpgrade(glm::vec3(0.0f, 5.0f, 0.0f), tex_[10], size_, game_objects_));
    //game_objects_.push_back(new SlowUpgrade(glm::vec3(5.0f, 5.0f, 0.0f), tex_[11], size_, game_objects_));

    // Setup other objects
    game_objects_.push_back(new EnemyGameObject(glm::vec3(-1.0f, 1.0f, 0.0f), tex_[1], size_));
    
    //timer Object
    game_objects_.push_back(timer = new TimerObject(glm::vec3(-1.0f, 2.0f, 0.0f), nums_, size_, 5*60));
    player->setTimer(timer);
    
    //game_objects_.push_back(new TimerObject(4 * 60));

    EnemyGameObject* e = new EnemyGameObject(glm::vec3(-1.0f, 1.0f, 0.0f), tex_[1], size_,true);
    miniBoss = new miniBossObject(glm::vec3(-3.0f, 1.0f, 0), tex_[2], size_, &game_objects_, e);
    game_objects_.push_back(miniBoss);

    //final Boss Object
    game_objects_.push_back(Boss = new FinalBossObject(glm::vec3(-1.0f, 2.0f, 0.0f), fbtex_, size_));
    Boss->SetScale(2);
    Boss->loadFinalBoss(timer, &game_objects_, e);

    game_objects_.push_back(arrow = new ArrowObject(glm::vec3(3.5f, -3.5f, 0.0f), tex_[8], size_, player->GetPosition(), miniBoss->GetPosition()));


    // Setup background
    Background *background = new Background(glm::vec3(0.0f, 0.0f, 0.0f), tex_[3], size_);
    background->SetScale(1000.0);
    background_objects_.push_back(background);
    //game_objects_.push_back(background);
}


void Game::MainLoop(void)
{

    // Loop while the user did not close the window
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Clear background
        glClearColor(viewport_background_color_g.r,
                     viewport_background_color_g.g,
                     viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view to zoom out, centered by default at 0,0
        float cameraZoom = 0.25f;
        glm::mat4 vscale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom));
        glm::mat4 vtranslation_matrix = glm::translate(glm::mat4(1.0f), -player->GetPosition());
        glm::mat4 view_matrix = vscale_matrix * vtranslation_matrix;
        shader_.SetUniformMat4("view_matrix", view_matrix);

        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the game
        Update(deltaTime);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


// Create the geometry for a sprite (a squared composed of two triangles)
// Return the number of array elements that form the square
int Game::CreateSprite(void)
{
    // The face of the square is defined by four vertices and two triangles

    // Number of attributes for vertices and faces
    // const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
    // const int face_att = 3; // Vertex indices (3)

    GLfloat vertex[]  = {
        // Four vertices of a square
        // Position      Color                Texture coordinates
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
    };

    // Two triangles referencing the vertices
    GLuint face[] = {
        0, 1, 2, // t1
        2, 3, 0  //t2
    };

    // OpenGL buffers
    GLuint vbo, ebo;

    // Create buffer for vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Create buffer for faces (index buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

    // Return number of elements in array buffer (6 in this case)
    return sizeof(face) / sizeof(GLuint);
}


void Game::SetTexture(GLuint w, const char *fname, bool tileable)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if (tileable) {
        // Texture Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//should be GL_REPEAT at end for background
    }
    else {
        // Texture Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::SetAllTextures(void)
{
    // Load all textures that we will need
    glGenTextures(NUM_TEXTURES, tex_);
    SetTexture(tex_[0], (resources_directory_g+std::string("/textures/spaceship.png")).c_str(), false);
    SetTexture(tex_[1], (resources_directory_g+std::string("/textures/drone.png")).c_str(), false);
    SetTexture(tex_[2], (resources_directory_g+std::string("/textures/miniboss.png")).c_str(), false);
    //SetTexture(tex_[7], (resources_directory_g + std::string("/textures/finalboss.png")).c_str(), false);

    SetTexture(tex_[3], (resources_directory_g + std::string("/textures/space.png")).c_str(), true);
    SetTexture(tex_[8], (resources_directory_g + std::string("/textures/arrow.png")).c_str(), false);
    

    // Explosion textures
    SetTexture(tex_[4], (resources_directory_g+std::string("/textures/explosion0.png")).c_str(), false);
    SetTexture(tex_[5], (resources_directory_g+std::string("/textures/explosion1.png")).c_str(), false);
    SetTexture(tex_[6], (resources_directory_g+std::string("/textures/explosion2.png")).c_str(), false);

    // Bullet texture
    SetTexture(tex_[7], (resources_directory_g + std::string("/textures/bullet.png")).c_str(), false);

    // Upgrade textures
    SetTexture(tex_[9], (resources_directory_g + std::string("/textures/powerup/0.png")).c_str(), false);
    SetTexture(tex_[10], (resources_directory_g + std::string("/textures/powerup/1.png")).c_str(), false);
    SetTexture(tex_[11], (resources_directory_g + std::string("/textures/powerup/2.png")).c_str(), false);

    

    glBindTexture(GL_TEXTURE_2D, tex_[0]);

    //textures that need arrays (ie finalboss, space, timer numbers)
    glGenTextures(10, nums_);
    for (int i = 0; i < 10; i++) {

        SetTexture(nums_[i], (resources_directory_g + std::string("/textures/0-9/" +std::to_string(i)+ ".png")).c_str(), false);
    }
    
    glGenTextures(4, back_);
    for (int i = 0; i < 4; i++) {
        SetTexture(back_[i], (resources_directory_g + std::string("/textures/space/space" + std::to_string(i) + ".png")).c_str(), false);
    }

    glGenTextures(5, fbtex_);
    for (int i = 0; i < 5; i++) {
        SetTexture(fbtex_[i], (resources_directory_g + std::string("/textures/FinalBoss/" + std::to_string(i) + ".png")).c_str(), false);
    }
}


void Game::Controls(void)
{

    // Stores the time when Controls() was previously ran
    static double prev_time = glfwGetTime();
    static double prev_swap_time = glfwGetTime();

    // Get player game object
    //GameObject *player = game_objects_[0];
    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];
    glm::vec3 curpos = player->GetPosition();

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        
        player->setAccel(1);
    }
    else if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->setAccel(-1);
    }
    else {
        player->setAccel(0);
    }

    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        //player->setAngle(player->getAngle() - 0.1);
        player->appendAngle(-1);
    }
    else if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        //player->setAngle(player->getAngle() + 0.1);
        player->appendAngle(1);
    }
    else {
        player->appendAngle(0);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
           //shoot bullets
    }

    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }

    if (glfwGetKey(window_, GLFW_KEY_R) == GLFW_PRESS) {
        if (glfwGetTime() - prev_swap_time > SWAP_DELAY) {
            player->swapFireMode();
            prev_swap_time = glfwGetTime();
        }
            
    }

    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        // shoot a bullet

        if (player->getFireMode() == fire_modes::SINGLE) {

            if (glfwGetTime() - prev_time > SINGLE_DELAY) {
                game_objects_.insert(game_objects_.begin() + 1, new BulletGameObject(game_objects_[0]->GetPosition(), tex_[7], size_, game_objects_[0]->getAngle(), SINGLE));
                game_objects_[1]->SetScale(0.5);
                prev_time = glfwGetTime();
            }

        }
        else if (player->getFireMode() == fire_modes::SHOTGUN) {
            //scale down for difference
            if (glfwGetTime() - prev_time > SHOTGUN_DELAY) {
                game_objects_.insert(game_objects_.begin() + 1, new BulletGameObject(game_objects_[0]->GetPosition(), tex_[7], size_, game_objects_[0]->getAngle() - 30, SHOTGUN));
                game_objects_[1]->SetScale(0.5);
                game_objects_.insert(game_objects_.begin() + 1, new BulletGameObject(game_objects_[0]->GetPosition(), tex_[7], size_, game_objects_[0]->getAngle(), SHOTGUN));
                game_objects_[1]->SetScale(0.5);
                game_objects_.insert(game_objects_.begin() + 1, new BulletGameObject(game_objects_[0]->GetPosition(), tex_[7], size_, game_objects_[0]->getAngle() + 30, SHOTGUN));
                game_objects_[1]->SetScale(0.5);
                prev_time = glfwGetTime();
            }
        }
    }
}


void Game::Update(double delta_time)
{

    // Handle user input
    Controls();

    PlayerGameObject* player = (PlayerGameObject*)game_objects_[0];

    // Update and render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // If the object is set to be killed, do this here
        if (current_game_object->getKill()) {
            // This if-statement is for debugging (prevents the player character from being killed on collision)
            if (i == 0) {
                glfwSetWindowShouldClose(window_, true);
            }
            else{
                glm::vec3 curr_pos = current_game_object->GetPosition();
                bool create_explosion = !current_game_object->isExplosion(); // Only spawn an explosion if the killed object is NOT an explosion

                // If the object that is set to be killed is an enemy, roll a dice and try to spawn an upgrade
                if (current_game_object->isEnemy()) {

                    int rand_val = rand() % 10;

                    // 20% chance to spawn an upgrade
                    if (rand_val < 2) {

                        //std::cout << "spawn an upgrade" << std::endl;

                        int upgrade_type = rand() % 3;

                        if (upgrade_type == 0) {
                            // spawn timer
                            //std::cout << "spawn timer" << std::endl;
                            game_objects_.insert(game_objects_.begin() + i + 1, new TimerUpgrade(curr_pos, tex_[9], size_, (PlayerGameObject*)game_objects_[0]));
                        }
                        else if (upgrade_type == 1) {
                            // spawn nuke
                            //std::cout << "spawn nuke" << std::endl;
                            game_objects_.insert(game_objects_.begin() + i + 1, new NukeUpgrade(curr_pos, tex_[10], size_, game_objects_));
                        }
                        else if (upgrade_type == 2) {
                            // spawn slow
                            //std::cout << "spawn slow" << std::endl;
                            game_objects_.insert(game_objects_.begin() + i + 1, new SlowUpgrade(curr_pos, tex_[11], size_, game_objects_));
                        }
                    }
                }

                delete game_objects_[i]; // Since it's dynamically allocated, we should delete the memory first

                if (create_explosion) {
                    std::vector<GLuint> expl_textures;

                    // *IMPORTANT* If any textures are added to tex_, make sure to adjust this loop if necessary
                    for (int j = 4; j < 7; ++j) {
                        expl_textures.push_back(tex_[j]);
                    }

                    Explosion* explosion;
                    explosion = new Explosion(curr_pos, expl_textures, size_);

                    game_objects_[i] = (GameObject*)explosion;
                }
                else { // The only reason we would not create an explosion is if the current obj IS an explosion. Simply remove it from the vector
                    game_objects_.erase(game_objects_.begin() + i);
                    --i;
                }

                continue;
            }
        }

        //special conditions
        if (current_game_object->isEnemy()) {//make sure enemies know where player is
            EnemyGameObject* g = (EnemyGameObject*)current_game_object;
            g->setPlayerLocation(player->GetPosition());
        }   
        if (current_game_object == timer) {//update timer
            timer->act(player->GetPosition());
        }
        if (current_game_object == arrow) {//update where arrow pointing to
            if (timer->timerDone()) {
                arrow->setBossLocation(Boss->GetPosition());
            }
            else {
                arrow->setBossLocation(miniBoss->GetPosition());
            }
            arrow->setPlayerLocation(player->GetPosition());
        }
        if (current_game_object == miniBoss) {//damage final boss if miniboss killed
            
            if (miniBoss->checkIfFinalBossDamage()) {
                std::cout << "miniboss killed" << std::endl;
                Boss->loseHP(1);
            }
        }


        // Update the current game object
        current_game_object->Update(delta_time);

        // Check for collision with other game objects

        checkCollisions(game_objects_, current_game_object);

        //for (int j = i + 1; j < game_objects_.size(); j++) {
        //    GameObject* other_game_object = game_objects_[j];

        //    float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
        //    if (distance < 0.1f) {
        //        // This is where you would perform collision response between objects
        //    }
        //}

        // Render game object
        if (current_game_object == timer)timer->Render(shader_);
        else current_game_object->Render(shader_);
    }
    //update background
    for (int i = 0; i < background_objects_.size(); i++) {
        GameObject* current_game_object = background_objects_[i];
        current_game_object->Update(delta_time);
        current_game_object->Render(shader_);
    }

}
       
} // namespace game
