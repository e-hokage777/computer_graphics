#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "plane.h"
#include "cube.h"
#include "camera.h"

// globals
const int WIDTH = 800;
const int HEIGHT = 600;
float deltaTime = 0;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // moving camrea
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(CameraMovement::FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(CameraMovement::BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(CameraMovement::LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(CameraMovement::RIGHT);
    }
}

double lastX = WIDTH / 2.0;
double lastY = HEIGHT / 2.0;
bool firstMouse = true;
void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float dx = xpos - lastX;
    float dy = ypos - lastY;

    camera.updateDirection(dx, dy);

    lastX = xpos;
    lastY = ypos;
}

int main()
{
    // configure glfw
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_STENCIL_TEST);
    // glDepthFunc(GL_ALWAYS);

    // shaders
    Shader shaderRed = Shader("shaders/basic.vs", "shaders/geom-shader.fs", "shaders/geom-shader.gs");
    Shader shaderBlue = Shader("shaders/ubo_vertex.vs", "shaders/ubo_fragment2.fs");

    Cube redCube = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, -3.0f));
    Cube blueCube = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, -3.0f));

    // creating poitns
    float points[] = {
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.5f, 0.3f};

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // create uniform block
    unsigned int ubo;
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // binding shaders to binding poits
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));

    unsigned int uboRed = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
    unsigned int uboBlue = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
    glUniformBlockBinding(shaderRed.ID, uboRed, 0);
    glUniformBlockBinding(shaderBlue.ID, uboBlue, 0);

    // defining matrices
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // setting projection matrix
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMat));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glm::vec3 housePositions[] = {
        glm::vec3(-0.5f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.0f, 0.0f),
    };

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // redCube.Draw(shaderRed);
        // blueCube.Draw(shaderBlue);

        shaderRed.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
