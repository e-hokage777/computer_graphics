#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "plane.h"
#include "cube.h"
#include "camera.h"
#include <map>
#include <cstdlib>
#include "screen.h"
#include "light.h"

// globals
const int WIDTH = 800;
const int HEIGHT = 600;
float deltaTime = 0;
Camera camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
bool blinn = false;

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

    // changing lighthing
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        blinn = !blinn;
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
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    // glfwWindowHint(GLFW_SAMPLES, 4);

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
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CW);
    // glDepthFunc(GL_ALWAYS);
    // glEnable(GL_MULTISAMPLE);

    Shader shader("shaders/vertex.vs", "shaders/fragment_light.fs");
    Shader lightShader("shaders/vertex.vs", "shaders/light.fs");

    // cubes
    // Cube cube1 = Cube(glm::vec3(2.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-0.2f, 0.0f, -10.0f), {"assets/brickwall.jpg"});
    Plane floor = Plane(glm::vec3(50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), {"assets/wood.png"}, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    Light light1 = Light(glm::vec3(4.0f, 2.0f, 0. - 14.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f);
    Light light2 = Light(glm::vec3(0.0f, 2.0f, 0. - 14.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f);
    Light light3 = Light(glm::vec3(-4.0f, 2.0f, 0. - 14.0f), glm::vec3(1.0f, 0.9f, 1.0f), 1.0f, 0.0f, 0.0f);

    // defining matrices
    glm::mat4 modelMat = glm::mat4(1.0f);
    // modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // creating other screen
    Screen screen = Screen(WIDTH, HEIGHT, 4);
    Shader screenShader = Shader("shaders/screen.vs", "shaders/screen.fs");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // drawing light

        // draw offscreen
        screen.activate();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightShader.use();
        lightShader.uniformMat4("projection", projectionMat);
        lightShader.uniformMat4("view", camera.getMatrix());
        light1.Draw(lightShader);
        light2.Draw(lightShader);
        light3.Draw(lightShader); // Drawing multiple lights but only one is passing info to obj shader

        shader.use();
        shader.uniformMat4("projection", projectionMat);
        shader.uniformMat4("view", camera.getMatrix());
        // shader.uniformMat4("model", modelMat);
        shader.uniformVec3("cameraPos", camera.position);
        shader.setBool("blinn", blinn);
        shader.uniformVec3("lightPositions[0]", light1.position);
        shader.uniformVec3("lightColors[0]", light1.color);
        shader.uniformVec3("lightPositions[1]", light2.position);
        shader.uniformVec3("lightColors[1]", light2.color);
        shader.uniformVec3("lightPositions[2]", light3.position);
        shader.uniformVec3("lightColors[2]", light3.color);

        // cube1.Draw(shader);
        floor.Draw(shader);

        screen.deactivate();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screen.draw(screenShader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
