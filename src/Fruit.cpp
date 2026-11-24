#include "Fruit.h"

Fruit::Fruit(Texture TEXTURE):texture(TEXTURE) {

    vertices = {
        // 第一个三角形
        -0.3f, -0.3f, 0.0f,  0.0f, 0.0f,
         0.3f, -0.3f, 0.0f,  1.0f, 0.0f,
         0.3f,  0.3f, 0.0f,  1.0f, 1.0f,
        // 第二个三角形
         0.3f,  0.3f, 0.0f,  1.0f, 1.0f,
        -0.3f,  0.3f, 0.0f,  0.0f, 1.0f,
        -0.3f, -0.3f, 0.0f,  0.0f, 0.0f
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    srand(time(0));
    float x1 = (float)((rand()%1001)/1000.f);
    float y1 = (float)((rand()%1001)/1000.f);

    x1 = x1*16.f-8.f;
    y1 = y1*12.f-6.f;

    position = glm::vec3(x1, y1, 0.1f);
}

void Fruit::changeLoc() {

    float x1 = (float)((rand()%1001)/1000.f);
    float y1 = (float)((rand()%1001)/1000.f);

    x1 = x1*16.f-8.f;
    y1 = y1*12.f-6.f;

    position = glm::vec3(x1, y1, .1f);
}

void Fruit::delete_source() {
   glDeleteBuffers(1, &VBO);
   glDeleteVertexArrays(1, &VAO);
}

void Fruit::draw(Shader &SHADER) {
 SHADER.use();
 texture.use(0);
 glBindVertexArray(VAO);

 glm::mat4 model = glm::mat4(1.0f);
 model = glm::translate(model,position);
 SHADER.setMat("model", model);

 glDrawArrays(GL_TRIANGLES, 0, 6);
}
