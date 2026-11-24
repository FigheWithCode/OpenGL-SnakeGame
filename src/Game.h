#pragma once

#include "Shader.h"
#include <vector>
#include "Direction.h"

class Camera;

class Game {
public:
    int src_w;
    int src_h;

    std::vector<float> horizontal_points;
    std::vector<float> vertical_points;

    int size[2];
    Shader shader;

    unsigned int VAO, VBO;

    unsigned int VAO2,VBO2;

    Game(int w = 800,int h = 600,int s1 = 30,int s2 = 30);
    void draw(Camera& cam);
    float getNearest(glm::vec3 position , Direction Current_d);
};