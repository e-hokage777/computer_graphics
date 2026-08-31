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
#include "directional_light.h"
#include "scene.h"

// globals
int WIDTH = 800;
int HEIGHT = 600;
float deltaTime = 0;
Camera camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
bool blinn = false;
bool gammaCorrection = false;
bool showShadowMap = false;
Screen screen;

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
    screen.resize(width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // toggle blinn phong
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        blinn = !blinn;
    }
    // toggle gamma correction
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        // This fires exactly ONCE per physical physical click
        gammaCorrection = !gammaCorrection;
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        showShadowMap = !showShadowMap;
    }
}

glm::vec3 boxPosition = glm::vec3(0.0f);

void processInput(GLFWwindow *window, bool isCamera = true)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // moving camrea
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.move(CameraMovement::FORWARD);
        // boxPosition.z -= 0.1f * deltaTime * 100;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.move(CameraMovement::BACKWARD);
        // boxPosition.z += 0.1f * deltaTime * 100;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.move(CameraMovement::LEFT);
        // boxPosition.x -= 0.1f * deltaTime * 100;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.move(CameraMovement::RIGHT);
        // boxPosition.x += 0.1f * deltaTime * 100;
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
    glfwSetKeyCallback(window, keyCallback);
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
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CW);
    // glDepthFunc(GL_ALWAYS);
    // glEnable(GL_MULTISAMPLE);
    // glEnable(GL_FRAMEBUFFER_SRGB);

    Shader shader("shaders/vertex.vs", "shaders/shadows.fs");
    Shader shadowRenderShader("shaders/vertex.vs", "shaders/shadow_render.fs");
    Shader lightShader("shaders/vertex.vs", "shaders/light.fs");
    Shader singleTexShader("shaders/single_tex_shader.vs", "shaders/single_tex_shader.fs");

    // cubes
    // Cube cube1 = Cube(glm::vec3(2.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-0.2f, 0.0f, -10.0f), {"assets/brickwall.jpg"});
    Plane floor = Plane(glm::vec3(50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-15.0f, 0.0f, 0.0f), {"assets/wood.png"}, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    Cube cube1 = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), {"assets/brickwall.jpg"});
    Light light1 = Light(glm::vec3(4.0f, 2.0f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f);
    Light light2 = Light(glm::vec3(0.0f, 0.7f, -2.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f);
    Light light3 = Light(glm::vec3(-4.0f, 0.3f, -8.0f), glm::vec3(1.0f, 0.9f, 1.0f), 1.0f, 0.0f, 0.0f);
    
    // defining matrices
    glm::mat4 modelMat = glm::mat4(1.0f);
    // modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    
    // Setting up screen buffers
    screen.init();
    Shader screenShader = Shader("shaders/screen.vs", "shaders/screen.fs");
    
    // creating directional light
    // DirectionalLight directionalLight = DirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), cube1.position + glm::vec3(0.0f, 5.0f, 0.0f));
    DirectionalLight directionalLight = DirectionalLight(glm::vec3(4.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, 7.0f, 5.0f));
    // Light dlight = Light(directionalLight.falsePosition, glm::vec3(1.0f, 0.9f, 1.0f), 1.0f, 0.0f, 0.0f);
    Cube dlight = Cube(glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), directionalLight.falsePosition, {"assets/skybox/front.jpg"});

    // creating scene
    Scene scene = Scene();
    scene.add(&floor);
    scene.add(&cube1);
    scene.add(&light1);
    scene.add(&light2);
    scene.add(&light3);
    scene.add(&dlight);

    float lastTime = 0;

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window);

        // cube1.position = boxPosition;

        // drawing light

        // rendering shadow map
        directionalLight.renderShadowMap(shadowRenderShader, scene);

        // resetting viewport
        glViewport(0, 0, WIDTH, HEIGHT);

        // draw offscreen
        screen.activate();
        if (gammaCorrection)
        {
            glEnable(GL_FRAMEBUFFER_SRGB);
        }
        else
        {
            glDisable(GL_FRAMEBUFFER_SRGB);
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // lightShader.use();
        // lightShader.uniformMat4("projection", projectionMat);
        // lightShader.uniformMat4("view", camera.getMatrix());
        // light1.Draw(lightShader);
        // light2.Draw(lightShader);
        // light3.Draw(lightShader); // Drawing multiple lights but only one is passing info to obj shader

        shader.use();
        shader.uniformMat4("projection", projectionMat);
        shader.uniformMat4("view", camera.getMatrix());
        // shader.uniformMat4("model", modelMat);
        shader.uniformVec3("cameraPos", camera.position);
        shader.setBool("blinn", blinn);
        shader.uniformVec3("dirLight.direction", directionalLight.direction);
        shader.uniformVec3("dirLight.color", directionalLight.color);
        shader.uniformVec3("dirLight.position", directionalLight.falsePosition);

        // setting shadow map and light matrix
        shader.setTexUnit(8, directionalLight.shadowMap, "shadowMap", GL_TEXTURE_2D);
        shader.uniformMat4("lightSpaceMatrix", directionalLight.lightMatrix);

        // shader.uniformVec3("lightPositions[0]", light1.position);
        // shader.uniformVec3("lightColors[0]", light1.color);
        shader.uniformVec3("lightPositions[0]", light2.position + glm::vec3(-4.0f, 4.0f, 2.0f));
        shader.uniformVec3("lightColors[0]", light2.color);
        // shader.uniformVec3("lightPositions[2]", light3.position);
        // shader.uniformVec3("lightColors[2]", light3.color);

        // cube1.Draw(shader);
        // floor.Draw(shader);

        scene.render(shader);

        if (showShadowMap)
        {
            directionalLight.drawShadowMap(singleTexShader);
        }

        screen.deactivate();

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screen.draw(screenShader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
