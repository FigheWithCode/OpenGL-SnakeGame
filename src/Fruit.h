#pragma once

#include<vector>
#include"Texture.h"
#include <glm/glm.hpp>
#include"Shader.h"

class Fruit {
public:
    Texture texture;
    glm::vec3 position;

    Fruit(Texture TEXTURE);
    void draw(Shader &SHADER);
    void delete_source();
    void changeLoc();

private:
    unsigned int VAO,VBO;
    std::vector<float> vertices;
};