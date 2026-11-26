#include "Snake.h"

#include "Fruit.h"
#include"Game.h"
#include"Shader.h"

Snake::Snake(Texture TEXTURE,float Speed):
    texture(TEXTURE),
    speed(Speed),
    direction(glm::vec3(-1.f,0.f,0.f)),
    future_direction(direction),
    dir(snake_LEFT),
    position(glm::vec3(0.0f,0.0f,0.0f)),
    up(glm::vec3(0.0f,1.0f,.0f)),
    down(glm::vec3(0.0f,-1.0f,.0f)),
    right(glm::vec3(1.0f,0.0f,0.0f)),
    left(glm::vec3(-1.0f,0.0f,.0f)),
    time(0.f)
{
    vertices = {
        // 第一个三角形
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        // 第二个三角形
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f
    };

    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(float),vertices.data(),GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*5,(void*)0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    lastTime = 0.f;
    for (int i = 1;i<=size;i++) {
        prev_positions.push_back(position+glm::vec3(spacing*i,0.0f,0.0f));
    }
}

void Snake::delete_resource() {
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
}

void Snake::ChageDir(Direction d) {
    switch (d) {
        case(snake_UP):
            future_direction = (direction == down)? down:up;
            break;
        case(snake_DOWN):
            future_direction = (direction == up)? up:down;
            break;
        case(snake_LEFT):
            future_direction = (direction == right)? right:left;
            break;
        case(snake_RIGHT):
            future_direction = (direction == left)? left:right;
            break;
    }
}

void Snake::Move(float dt, Game& GAME) {
    if (glfwGetTime()-lastTime<stepInterval)
        return;
    lastTime = glfwGetTime();

    direction = future_direction;
    update_Dir();

    prev_positions.insert(prev_positions.begin(),position);//旧头变为第一节新的身体
    if (prev_positions.size()>size) {
        prev_positions.pop_back();
    }
    position += direction*spacing;//头向前一格


    position.x = (position.x<-8.f)? position.x+16.f:position.x;
    position.x = (position.x>8.f)? position.x-16.f:position.x;

    position.y = (position.y<-6.f)? position.y+12.f:position.y;
    position.y = (position.y>6.f)? position.y-12.f:position.y;

}

void Snake::draw(Shader &SHADER) {
    SHADER.use();
    texture.use(0);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,position);
    SHADER.setMat("model", model);

    glDrawArrays(GL_TRIANGLES,0,6);//画的是蛇头


    for (int i = 0;i<prev_positions.size();i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,prev_positions[i]);
        SHADER.setMat("model", model);
        glDrawArrays(GL_TRIANGLES,0,6);
    }
}

void Snake::eat_collision(Fruit &FRUIT) {
    if (sqrt(pow((position.x-FRUIT.position.x),2)+pow((position.y-FRUIT.position.y),2))<.7f) {
        FRUIT.changeLoc();
        size++;
    }
}

bool Snake::hintHimself() {
    for (int i = 0;i<prev_positions.size();i++) {
        float dx = position.x-prev_positions[i].x;
        float dy = position.y-prev_positions[i].y;

        if (sqrt(dx*dx+dy*dy)<.5f) return true;
    }

    return false;
}

void Snake::update_Dir() {
    if (direction == up)
        dir = snake_UP;
    else if (direction == down)
        dir = snake_DOWN;
    else if (direction == right)
        dir = snake_RIGHT;
    else if (direction == left)
        dir = snake_LEFT;
}
