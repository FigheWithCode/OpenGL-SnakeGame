#pragma once

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<vector>
#include"Texture.h"
#include"Shader.h"
#include"Camera.h"

class backGround {
public:
    Shader shader;
    Texture texture;
    backGround(Texture texture);
    void draw(Camera& cam);

private:
    unsigned int vao,vbo;
    std::vector<float> vertices;
};