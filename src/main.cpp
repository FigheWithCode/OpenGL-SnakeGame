#include<iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Texture.h"
#include"Camera.h"
#include "Snake.h"
#include "Fruit.h"
#include "Game.h"

const float width = 800;
const float height = 600;

float deltaTime = 0.f;
float lastFrame = 0.f;

void framebuffer_size_callback(GLFWwindow* window,int width, int height) {
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window, Snake &PLAYER) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        PLAYER.ChageDir(snake_UP);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        PLAYER.ChageDir(snake_LEFT);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        PLAYER.ChageDir(snake_DOWN);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS||glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        PLAYER.ChageDir(snake_RIGHT);
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Happy Sname Game", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    const char* vertexShaderPath = "Shaders/vertex";
    const char* fragmentShaderPath = "Shaders/fragment";
    Shader shader(vertexShaderPath,fragmentShaderPath);

    Texture container_texture("Assets/container.jpg",false);
    Texture face_texture("Assets/awesomeface.png",true);

    Camera camera=Camera();
    Snake player = Snake(face_texture);
    Game game = Game();
    Fruit fruit = Fruit(container_texture);

    shader.use();
    shader.setInt("texture1",0);
    shader.setInt("texture2",1);


    while (!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime()-lastFrame;
        lastFrame = glfwGetTime();

        processInput(window,player);

        glClearColor(.2f,.5f,.3f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        game.draw(camera);

        shader.use();

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat("view",view);

        glm::mat4 projection = glm::ortho(-8.f, 8.f, -6.f, 6.f, -1.f, 1.f);
        shader.setMat("projection",projection);

        fruit.draw(shader);

        player.Move(deltaTime,game);
        player.draw(shader);
        player.eat_collision(fruit);

        if (deltaTime > 0.f)
            std::cout << "Position:" << player.position.x << " , " << player.position.y << " Frames per second " << 1.f / deltaTime << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    player.delete_resource();
    fruit.delete_source();
    glfwTerminate();
    return 0;
}