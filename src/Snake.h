#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Direction.h"
#include "Shader.h"

#include"Texture.h"

class Game;
class Fruit;
class Shader;

class Snake {
public:

    Texture texture;
    glm::vec3 direction;
    Direction dir;
    glm::vec3 future_direction;
    glm::vec3 position;

    glm::vec3 up;
    glm::vec3 down;
    glm::vec3 right;
    glm::vec3 left;

    float speed;
    float time;
    bool moved;

    Snake(Texture TEXTURE,float speed = 3.f);
    void ChageDir(Direction d);
    void Move(float dt,Game& GAME);
    void draw(Shader& SHADER);
    void eat_collision(Fruit& FRUIT);
    void delete_resource();
    bool hintHimself();

private:

    unsigned int VAO,VBO;
    std::vector<float> vertices;
    std::vector<glm::vec3> prev_positions;
    int size = 0;
    float lastTime=0;
    void update_Dir();
};