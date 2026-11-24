#include "Game.h"
#include"Camera.h"

Game::Game(int w, int h, int s1, int s2):src_w(w),src_h(h),shader(Shader("Shaders/grid_vs","Shaders/grid_fs")) {
    size[0]=s1;
    size[1]=s2;

    float world_width = 16.f;
    float world_height = 12.f;

    for (int i=0;i<=size[0];i++) {  //包含边界
        float current_x = -world_width/2.f + (world_width/(float)size[0]*(float)i);
        float bottom_y = -world_height/2.f;
        float top_y = world_height/2.f;

        //两个端点，用于后续画线
        horizontal_points.push_back(current_x);
        horizontal_points.push_back(top_y);
        horizontal_points.push_back(current_x);
        horizontal_points.push_back(bottom_y);
    }

    for (int i = 0;i<=size[1];i++) {
        float current_y = -world_height/2.f + (world_height/(float)size[1]*(float)i);
        float right_x = world_width/2.f;
        float left_x = -world_width/2.f;

        vertical_points.push_back(left_x);
        vertical_points.push_back(current_y);
        vertical_points.push_back(right_x);
        vertical_points.push_back(current_y);
    }

    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,horizontal_points.size()*sizeof(float),horizontal_points.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1,&VBO2);
    glGenVertexArrays(1,&VAO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);

    glBufferData(GL_ARRAY_BUFFER,vertical_points.size()*sizeof(float),vertical_points.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    //unbind
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Game::draw(Camera &cam) {
    shader.use();

    glm::mat4 model = glm::mat4(1.f);
    shader.setMat("model",model);

    glm::mat4 view = cam.GetViewMatrix();
    shader.setMat("view",view);

    glm::mat4 projection = glm::ortho(-8.f, 8.f, -6.f, 6.f, -1.f, 1.f);
    shader.setMat("projection",projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES,0,horizontal_points.size()/2);

    glBindVertexArray(VAO2);
    glDrawArrays(GL_LINES,0,vertical_points.size()/2);
}

float Game::getNearest(glm::vec3 position, Direction current) {
    // Clamp into the board so wrap/edge frames still resolve a cell
    if (position.x < -8.f) position.x = -8.f;
    if (position.x >  8.f) position.x =  8.f;
    if (position.y < -6.f) position.y = -6.f;
    if (position.y >  6.f) position.y =  6.f;

    if (current == snake_LEFT || current == snake_RIGHT) {
        bool goingRight = (current == snake_RIGHT);

        for (size_t i = 0; i + 4 < horizontal_points.size(); i += 4) {
            float left = horizontal_points[i];
            float right = horizontal_points[i + 4];
            bool lastCell = (i + 8 >= horizontal_points.size());
            // Half-open [left, right); last cell includes right edge
            bool inside = lastCell
                ? (position.x >= left && position.x <= right)
                : (position.x >= left && position.x < right);
            if (inside)
                return goingRight ? right : left;
        }
    }

    if (current == snake_DOWN || current == snake_UP) {
        bool goingDown = (current == snake_DOWN);

        for (size_t i = 1; i + 4 < vertical_points.size(); i += 4) {
            float low = vertical_points[i];
            float high = vertical_points[i + 4];
            bool lastCell = (i + 8 >= vertical_points.size());
            bool inside = lastCell
                ? (position.y >= low && position.y <= high)
                : (position.y >= low && position.y < high);
            if (inside)
                return goingDown ? high : low;
        }
    }

    return 9999.f;
}
