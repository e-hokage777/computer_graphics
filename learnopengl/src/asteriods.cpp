#include <iostream>
#include <cstdlib>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include <vector>

// globals
const int WIDTH = 800;
const int HEIGHT = 600;
float deltaTime = 0;
Camera camera = Camera(glm::vec3(0.0f, 0.0f, 55.0f), glm::vec3(0.0f, 0.0f, -1.0f));

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
bool firstMouse = false;
void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    // if (firstMouse)
    // {
    //     lastX = WIDTH / 2.0;
    //     lastY = HEIGHT / 2.0;
    //     firstMouse = false;
    // }

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

    Shader shader("shaders/vertex.vs", "shaders/fragment.fs");

    // Model backpack = Model("assets/backpack/backpack.obj");
    Model planet = Model("assets/planet/planet.obj");
    Model asteriod = Model("assets/rock/rock.obj");

    // defining matrices
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -10.0f));
    // modelMat = glm::mat4(1.0f);
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // preparing to draw asteriods
    unsigned int numAsteriods = 10000;
    float asteriodBeltRadius = 30.0f;

    std::vector<vector<float>> asteriodParams;
    std::vector<glm::mat4> asteriodTransforms;



    for (int i = 0; i < numAsteriods; ++i)
    {
        float position = (float)i / (float)numAsteriods * 2 * 3.142f;
        float xPos = glm::cos(position) * asteriodBeltRadius;
        float zPos = glm::sin(position) * asteriodBeltRadius - 10.0f;

        // randoms
        float xOffset = rand() % 20 - 10;
        float yOffset = rand() % 6 - 3;
        float zOffset = rand() % 20 - 10;
        float rotation = rand() % 360 - 180;
        float scale = (rand()%20) /100.0f+0.05;

        glm::mat4 transformMat = glm::mat4(1.0f);
        transformMat = glm::translate(transformMat, glm::vec3(xPos + xOffset, yOffset, zPos + zOffset));
        transformMat = glm::scale(transformMat, glm::vec3(scale));
        transformMat = glm::rotate(transformMat, glm::radians(rotation), glm::vec3(0.4f, 0.6f, 0.8f));

        asteriodTransforms.push_back(transformMat);
    }

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -10.0f));
        modelMat = glm::scale(modelMat, glm::vec3(4.0f));
        shader.uniformMat4("projection", projectionMat);
        shader.uniformMat4("view", camera.getMatrix());
        shader.uniformMat4("model", modelMat);
        planet.Draw(shader);

        for (int i = 0; i < asteriodTransforms.size(); ++i)
        {

            shader.uniformMat4("model", asteriodTransforms[i]);
            asteriod.Draw(shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
