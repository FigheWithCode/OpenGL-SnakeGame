#include "backGround.h"

backGround::backGround(Texture texture) : texture(texture),shader(Shader("Shaders/vertex","Shaders/fragment")) {
    vertices = {
        // 位置 x,y,z          UV
        -8.f, -6.f, 0.f,     0.f, 0.f,
         8.f, -6.f, 0.f,     1.f, 0.f,
         8.f,  6.f, 0.f,     1.f, 1.f,

         8.f,  6.f, 0.f,     1.f, 1.f,
        -8.f,  6.f, 0.f,     0.f, 1.f,
        -8.f, -6.f, 0.f,     0.f, 0.f,
    };
    glGenBuffers(1,&vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(),GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*5,(void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
}

void backGround::draw(Camera& cam) {
    shader.use();

    glm::mat4 model = glm::mat4(1.f);
    shader.setMat("model", model);

    glm::mat4 view = cam.GetViewMatrix();
    shader.setMat("view", view);
    glm::mat4 projection = glm::ortho(-8.f, 8.f, -6.f, 6.f, -1.f, 1.f);
    shader.setMat("projection", projection);

    shader.setInt("texture1",0);
    texture.use(0);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,6);
}
