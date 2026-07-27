#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "plane.h"
#include "cube.h"
#include "camera.h"
#include <map>
#include <cstdlib>

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    // glDepthFunc(GL_ALWAYS);

    Shader shader("shaders/vertex.vs", "shaders/blending.fs");

    // vector<string> cubeTextures = {"assets/grass.png"};
    Plane window1 = Plane(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 0.0f, -12.0f), {"assets/window.png"});
    Plane window2 = Plane(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), {"assets/window.png"});
    Plane window3 = Plane(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.0f, -12.0f), {"assets/window.png"});
    vector<Plane> windows = {window1, window2, window3};

    // cubes
    Cube cube1 = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-0.2f, 0.0f, -14.0f), {"assets/brickwall.jpg"});
    Cube cube2 = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -12.0f), {"assets/brickwall.jpg"});
    Cube cube3 = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.0f, -14.0f), {"assets/brickwall.jpg"});

    // defining matrices
    glm::mat4 modelMat = glm::mat4(1.0f);
    // modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.uniformMat4("projection", projectionMat);
        shader.uniformMat4("view", camera.getMatrix());
        // shader.uniformMat4("model", modelMat);

        // sort windows
        // std::system("cls");
        // std::map<float, Plane> sortedPlanes;
        // for (int i = 0; i < windows.size(); i++)
        // {
        //     float distance = glm::length(camera.position - windows[i].position);

        //     std::cout << "Distance: " << distance << std::endl;
        //     sortedPlanes[distance] = windows[i];
        // }

        cube1.Draw(shader);
        cube2.Draw(shader);
        cube3.Draw(shader);

        // for (std::map<float, Plane>::reverse_iterator it = sortedPlanes.rbegin(); it !=
        //                                                                           sortedPlanes.rend();
        //      ++it)
        // {
        //     it->second.Draw(shader);
        // }

        // window1.Draw(shader);
        // window2.Draw(shader);
        // window3.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
