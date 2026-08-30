#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"


glm::vec3 boxPosition = glm::vec3(0.0f);

void framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos){
    
}

float lastX = 0.0;
float lastY = 0.0;
float deltaTime = 0.0f;
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        boxPosition.y += 0.1f * deltaTime * 100;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        boxPosition.y -= 0.1f * deltaTime * 100;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        boxPosition.x -= 0.1f * deltaTime * 100;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        boxPosition.x += 0.1f * deltaTime * 100;
    }
}

int main(){
 
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Model Loading", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // moving data to vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // creating shader
    Shader shader = Shader("shaders/vertex.vs", "shaders/basic.fs");

    // creating matrices
    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    shader.use();
    shader.uniformMat4("projection", projection);
    shader.uniformMat4("view", view);
    
    float lastTime = 0.0f;
    while(!glfwWindowShouldClose(window)){
        // computing delta time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, boxPosition);
        shader.uniformMat4("model", model);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);    

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}