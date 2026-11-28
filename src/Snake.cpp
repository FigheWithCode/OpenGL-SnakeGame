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

    head.from = head.to = glm::vec3(0.0f,0.0f,0.0f);
    position = head.to;

    body.clear();
    for (int i = 1;i<=size;i++) {
        glm::vec3 p = head.to+glm::vec3(spacing*i,0.0f,0.0f);
        body.push_back({p,p});
    }
    lastTime = glfwGetTime();
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
    direction = future_direction;
    update_Dir();

    if (glfwGetTime()-lastTime<stepInterval)
        return;
    lastTime = glfwGetTime();
    std::vector<glm::vec3> oldLogic;
    oldLogic.push_back(head.to);
    for (auto& s: body) oldLogic.push_back(s.to);

    glm::vec3 newHead = head.to+direction*spacing;

    if (newHead.x<-8.f) {
        newHead.x=8.f;
    }
    if (newHead.x>8.f) {
        newHead.x=-8.f;
    }
    if (newHead.y<-6.f) {
        newHead.y=6.f;
    }
    if (newHead.y>6.f) {
        newHead.y = -6.f;
    }

    std::vector<glm::vec3> newLogic;
    newLogic.push_back(newHead);
    for (auto i=0;i<oldLogic.size() && newLogic.size()<size+1;i++) newLogic.push_back(oldLogic[i]);//整个链的长度为size+1（包含了头）

    //设置插值点位
    head.from = oldLogic[0];
    head.to = newLogic[0];
    position = head.to;

    body.clear();
    for (auto i = 1;i<newLogic.size();i++) {
        Segment s;
        s.to = newLogic[i];
        s.from = (i<oldLogic.size())? oldLogic[i]: newLogic[i];
        body.push_back(s);
    }
}

glm::vec3 Snake::visualPos(const Segment s, float t) const {
    bool Crossed = std::abs(s.to.x-s.from.x) > 8.f || std::abs(s.to.y-s.from.y) > 6.f;
    if (Crossed)
        return s.to;//穿墙的话直接闪现，避免横穿全屏
    t = glm::clamp(t,0.f,1.f);
    return glm::mix(s.from,s.to,t);
}

void Snake::draw(Shader &SHADER) {
    float t = (glfwGetTime()-lastTime)/stepInterval;

    SHADER.use();
    texture.use(0);
    glBindVertexArray(VAO);

    auto drawAt = [&](const glm::vec3& p) {
        glm::mat4 model(1.f);
        model = glm::translate(model,p);
        SHADER.setMat("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    };

    drawAt(visualPos(head,t));
    for (const auto& s:body) drawAt(visualPos(s,t));;

}

void Snake::eat_collision(Fruit &FRUIT) {
    if (abs(position.x-FRUIT.position.x)>0.1f || abs(position.y-FRUIT.position.y)>0.1f) return;

    std::vector<glm::vec3> freeCells;
    for (int x = -7;x<=7;x++) {
        for (int y = -5;y<=5;y++) {
            glm::vec3 p{(float)x,(float)y,0.1f};

            if (abs(p.x-position.x)<.1f && abs(p.y-position.y)<.1f) continue;//避开开头

            bool onBody = false;
            for (auto& s: body) {
                if (abs(p.x-s.to.x)<.1f && abs(p.y-s.to.y)<.1f) {
                    onBody = true;
                    break;
                }
            }

            if (!onBody) freeCells.push_back(p);
        }
    }

    if (freeCells.empty()) {
        size++;
        return;
    }

    FRUIT.position = freeCells[rand()%freeCells.size()];
    size++;
}

bool Snake::hintHimself() {
    for (int i = 0;i<body.size();i++) {
        float dx = position.x-body[i].to.x;
        float dy = position.y-body[i].to.y;

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
