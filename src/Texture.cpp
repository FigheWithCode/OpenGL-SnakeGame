#include "Texture.h"

Texture::Texture(std::string path,bool png) {
    glGenTextures(1,&ID);
    glBindTexture(GL_TEXTURE_2D,ID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nr_channels;
    unsigned char* data = stbi_load(path.c_str(),&width,&height,&nr_channels,0);
    if (data) {
        if (png) {
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        }else {
            glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cerr<<"Error loading texture"<<std::endl;
    }
    stbi_image_free(data);
}

void Texture::use(int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D,ID);
}
