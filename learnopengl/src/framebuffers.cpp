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

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Model Loading", NULL, NULL);

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

    Shader shader("shaders/vertex.vs", "shaders/blending.fs");
    Shader basicShader("shaders/basic.vs", "shaders/basic.fs");

    // vector<string> cubeTextures = {"assets/grass.png"};
    Plane window1 = Plane(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-2.0f, 0.0f, -12.0f), {"assets/window.png"});
    Plane window2 = Plane(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), {"assets/window.png"});
    Plane window3 = Plane(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(2.0f, 0.0f, -12.0f), {"assets/window.png"});
    vector<Plane> windows = {window1, window2, window3};

    // cubes
    Cube cube1 = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-0.2f, 0.0f, -14.0f), {"assets/brickwall.jpg"});
    Cube cube2 = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -12.0f), {"assets/brickwall.jpg"});
    Cube cube3 = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.0f, -14.0f), {"assets/brickwall.jpg"});

    // plane
    Plane plane = Plane(glm::vec3(1.0f));

    // defining matrices
    glm::mat4 modelMat = glm::mat4(1.0f);
    // modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // creating color attachment
    unsigned int textureColorBuffer;
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // creating depth and stencil buffer
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);

    // creating frame buffer
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl
                  << std::endl;
    }

    // unbinding buffers
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);


        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.uniformMat4("projection", projectionMat);
        shader.uniformMat4("view", camera.getMatrix());
        cube1.Draw(shader);
        cube2.Draw(shader);
        cube3.Draw(shader);
        window2.Draw(shader);

        // switching back to default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        plane.Draw(basicShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
