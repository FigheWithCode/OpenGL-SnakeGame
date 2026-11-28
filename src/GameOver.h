#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<vector>
#include"Shader.h"
#include"Texture.h"
#include"Camera.h"

class GameOver {
public:
    Shader shader;
    Texture texture;
    GameOver(Texture texture);
    void draw(Camera& cam);
private:
    unsigned int vao,vbo;
    std::vector<float> vertices;
};