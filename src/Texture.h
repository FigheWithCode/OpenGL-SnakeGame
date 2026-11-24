#pragma once

#include<iostream>
#include<string>
#include"stb_image.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Texture {
public:
    Texture(std::string path,bool png);
    void use(int slot);
private:
    unsigned int ID;
};